#include "../include/Server.h"
#include "../include/Log.h"
#include "../include/StartupOptions.h"
#include "../include/ProgramPackets.h"

#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>


Server::Server() {
    options = StartupOptions::GetInstance();
    SetupServerNet();
}

Server::~Server() {
    close(sfd);
}

void Server::Run() {
    std::cout << std::endl;
    Log::MessageBlue("Server is running...");
    std::cout << std::endl;
    ReciveMessage();
}

void Server::SetupServerNet() {
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(atoi(options->GetServerPort().c_str()));
    sfd = socket(PF_INET, SOCK_DGRAM, 0);
    bind(sfd, (struct sockaddr*) &saddr, sizeof(saddr));
}

void Server::ReciveMessage() {
    int rc;
    socklen_t sl;
    char buf[50];
    struct sockaddr_in caddr;

    while(1) {
        memset(&caddr, 0, sizeof(caddr));
        memset(&buf, 0, sizeof(buf));
        sl = sizeof(caddr);
        ssize_t recv_bytes = recvfrom(sfd, buf, sizeof(buf) - 1, 0, (struct sockaddr*) &caddr, &sl);
        if (rc < 0) {
            Log::MessageRed("Error receiving message !");
            return;
        }

        buf[recv_bytes] = '\0';
        
        // Deserialize the packet
        std::string data(buf);
        size_t pos1 = data.find(";");
        size_t pos2 = data.find(";", pos1 + 1);

        uint16_t type = static_cast<uint16_t>(std::stoi(data.substr(0, pos1)));
        std::string ip_address = data.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string mac_address = data.substr(pos2 + 1);

        ProgramPackets::InfoPacketStruct infoPacket;
        infoPacket.type = static_cast<ProgramPackets::EntryType>(type);
        infoPacket.ip_address = ip_address;
        infoPacket.mac_address = mac_address;

        Log::MessageBlue("Info packet received from " + std::string(inet_ntoa(caddr.sin_addr)) + ":" + std::to_string(ntohs(caddr.sin_port)));
        PrintArpEntry(infoPacket.ip_address, infoPacket.mac_address, infoPacket.type);
    }
}


void Server::PrintArpEntry(const std::string& ip, const std::string& mac, ProgramPackets::EntryType type){
    if (type == ProgramPackets::EntryType::new_entry)
    {
        Log::MessageGreen("New entry: IP = " + ip + ", MAC = " + mac);
    }
    else if (type == ProgramPackets::EntryType::modified_entry)
    {
        Log::MessageYellow("Modified entry: IP = " + ip + ", MAC changed to " + mac);
    }
    else if (type == ProgramPackets::EntryType::deleted_entry)
    {
        Log::MessageRed("Removed entry: IP = " + ip + ", MAC = " + mac);
    }
    std::cout<<std::endl;
}