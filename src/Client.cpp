#include "../include/Client.h"
#include "../include/StartupOptions.h"
#include "../include/Log.h"
#include "../include/ProgramPackets.h"

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <cstring>
#include <ctype.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/filter.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>


Client::Client() {
    options = StartupOptions::GetInstance();
    arpMap = ParseArpFile();
    if(options->IsVerbose())
    {
        PrintArpMap(arpMap);
    }
    SetupClientNet();
    if (options->IsPacketMode()) {
        SetupPacketModeNet();
    }
}


void Client::SetupClientNet() {
    struct sockaddr_in caddr;
    struct hostent* addrent;

    addrent = gethostbyname(options->GetServerIP().c_str());
    memset(&caddr, 0, sizeof(caddr));
    caddr.sin_family = AF_INET;
    caddr.sin_addr.s_addr = INADDR_ANY;
    caddr.sin_port = 0;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    memcpy(&saddr.sin_addr.s_addr, addrent->h_addr, addrent->h_length);
    saddr.sin_port = htons(atoi(options->GetServerPort().c_str()));
    sfd = socket(PF_INET, SOCK_DGRAM, 0);
    bind(sfd, (struct sockaddr*) &caddr, sizeof(caddr));
}

void Client::SetupPacketModeNet() {
    pfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    setsockopt(pfd, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf));
    strncpy(ifr.ifr_name, options->GetInterface().c_str(), IFNAMSIZ);
    ioctl(pfd, SIOCGIFFLAGS, &ifr);
    ifr.ifr_flags |= IFF_PROMISC;
    ioctl(pfd, SIOCSIFFLAGS, &ifr);
}


Client::~Client() {
    arpMap.clear();
    close(sfd);

    if (options->IsPacketMode()) {
        ifr.ifr_flags &= ~IFF_PROMISC;
        ioctl(pfd, SIOCSIFFLAGS, &ifr);
        close(pfd);
    }
}


void Client::Run() {
    std::cout << std::endl;
    Log::MessageBlue("Client is running...");
    std::cout << std::endl;
    if(options->IsFileMode())
    {
        MonitorArpFile();
    }
    else if(options->IsPacketMode())
    {
        MonitorPackets();
    }
}


std::unordered_map<std::string, std::string> Client::ParseArpFile() {
    
    std::unordered_map<std::string, std::string> arpMap;
    std::ifstream arpFile(options->GetARPPath());
    std::string line;

    if (!arpFile.is_open()) {
        Log::MessageRed("Error opening " + options->GetARPPath() + " file");
        return arpMap;
    }

    // Skip the first line (header)
    std::getline(arpFile, line);

    while (std::getline(arpFile, line)) {
        std::istringstream iss(line);
        std::string ip, hwType, flags, mac, mask, device;
        iss >> ip >> hwType >> flags >> mac >> mask >> device;
        arpMap[ip] = mac;
    }

    arpFile.close();
    return arpMap;
}


void Client::MonitorArpFile() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));

        std::unordered_map<std::string, std::string> newArpMap = ParseArpFile();
        std::vector <ProgramPackets::InfoPacketStruct> infoPacket;

        // Check for changes
        bool changed = false;

        // Check for new entries or modified entries
        for (const auto& [ip, mac] : newArpMap) {
            if (arpMap.find(ip) == arpMap.end()) {
                PrintArpEntry(ip, mac, ProgramPackets::EntryType::new_entry);
                infoPacket.push_back({ProgramPackets::EntryType::new_entry, ip, mac});
                changed = true;
            } else if (arpMap[ip] != mac) {
                PrintArpEntry(ip, mac, ProgramPackets::EntryType::modified_entry);
                infoPacket.push_back({ProgramPackets::EntryType::modified_entry, ip, mac});
                changed = true;
            }
        }

        // Check for removed entries
        for (const auto& [ip, mac] : arpMap) {
            if (newArpMap.find(ip) == newArpMap.end()) {
                PrintArpEntry(ip, mac, ProgramPackets::EntryType::deleted_entry);
                infoPacket.push_back({ProgramPackets::EntryType::deleted_entry, ip, mac});
                changed = true;
            }
        }

        if (changed) {
            arpMap = newArpMap;

            if(options->IsVerbose())
            {
                PrintArpMap(arpMap);
                std::cout << std::endl;
            }
        
            for (const auto& packet : infoPacket) {
                SendInfoPacket(packet);
            }
        }
    }
}


void Client::MonitorPackets() {
    socklen_t sl;
    ssize_t len;
    struct packet_mac_ip data;
    Log::MessageBlue("Started capturing ARP packets...");
    std::cout << std::endl;

    sl = sizeof(struct sockaddr_ll);
    while(1) {
        len = recvfrom(pfd, frame, ETH_FRAME_LEN, 0, (struct sockaddr*)&sall, &sl);
        try{
            data = AnalyzeFrame(frame, &sall); 
            CompareArpEntries(data);
        }
        catch(const std::runtime_error& e){
            continue;
        }
    }
}

void Client::CompareArpEntries(const struct packet_mac_ip& data)
{
    std::string ip = data.ip;
    std::string mac = data.mac;
    std::vector <ProgramPackets::InfoPacketStruct> infoPacket;

    if (arpMap.find(ip) == arpMap.end()) {
        PrintArpEntry(ip, mac, ProgramPackets::EntryType::new_entry);
        infoPacket.push_back({ProgramPackets::EntryType::new_entry, ip, mac});
    } else if (arpMap[ip] != mac) {
        PrintArpEntry(ip, mac, ProgramPackets::EntryType::modified_entry);
        infoPacket.push_back({ProgramPackets::EntryType::modified_entry, ip, mac});
    }
    
    if (!infoPacket.empty()) {
        arpMap[ip] = mac;
        if(options->IsVerbose())
        {
            PrintArpMap(arpMap);
            std::cout << std::endl;
        }
        for (const auto& packet : infoPacket) {
            SendInfoPacket(packet);
        }
    }
}

Client::packet_mac_ip Client::AnalyzeFrame(char* frame, struct sockaddr_ll* sall) {
    int i, j;
    unsigned char ch;
    struct ethhdr* fhead;
    struct ProgramPackets::arphdr* arpProto;
    struct packet_mac_ip retrivedData;

    // Make sure that packet uses ARP protocol
    if ((ntohs(sall->sll_protocol) & 0xFFFFU) !=  ETH_P_ARP){
        throw std::runtime_error("Packet is not ARP");
    }
    if (sall->sll_pkttype == PACKET_OUTGOING){
        throw std::runtime_error("Packet is not ARP");
    }
    if (options->IsVerbose())
        Log::MessageBlue("ARP packet received");

    arpProto = (struct ProgramPackets::arphdr*) (frame + ETHER_HDR_LEN);
    sprintf(retrivedData.mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            arpProto->ar_sha[0], arpProto->ar_sha[1], arpProto->ar_sha[2],
            arpProto->ar_sha[3], arpProto->ar_sha[4], arpProto->ar_sha[5]);

    sprintf(retrivedData.ip, "%d.%d.%d.%d",
        arpProto->ar_sip[0], arpProto->ar_sip[1], arpProto->ar_sip[2], arpProto->ar_sip[3]);

    return retrivedData;
}


void Client::SendInfoPacket(const ProgramPackets::InfoPacketStruct& infoPacket) {
    // Serialize the packet
    uint16_t type = static_cast<uint16_t>(infoPacket.type);
    std::string data = std::to_string(type) + ";" + infoPacket.ip_address + ";" + infoPacket.mac_address;

    sendto(sfd, data.c_str(), data.size(), 0, (struct sockaddr*) &saddr, sizeof(saddr));

    Log::MessageBlue("Info packet sent to " + options->GetServerIP() + ":" + options->GetServerPort());
}


void Client::PrintArpMap(const std::unordered_map<std::string, std::string> &arpMap){
    for (const auto& [ip, mac] : arpMap) {
        PrintArpEntry(ip, mac, ProgramPackets::EntryType::new_entry);
    }
}


void Client::PrintArpEntry(const std::string& ip, const std::string& mac, ProgramPackets::EntryType type){
    if (type == ProgramPackets::EntryType::new_entry)
    {
        Log::MessageGreen("New entry: IP = " + ip + ", MAC = " + mac);
    }
    else if (type == ProgramPackets::EntryType::modified_entry)
    {
        Log::MessageYellow("Modified entry: IP = " + ip + ", MAC changed from " + arpMap[ip] + " to " + mac);
    }
    else if (type == ProgramPackets::EntryType::deleted_entry)
    {
        Log::MessageRed("Removed entry: IP = " + ip + ", MAC = " + mac);
    }
}
