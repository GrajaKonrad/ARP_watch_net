#pragma once

#include "StartupOptions.h"
#include "ProgramPackets.h"

#include <unordered_map>
#include <netinet/in.h>
#include <linux/filter.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/ethernet.h>
       

class Client
{

public:
    /// @brief Client constructor
    Client();

    /// @brief Run client
    void Run();

    /// @brief Client destructor
    ~Client();

private:
    /// @brief Stores ARP file contets parsed to unordered map
    std::unordered_map<std::string, std::string> arpMap;

    /// @brief Application startup options
    StartupOptions* options;

    /// @brief Socket file descriptors for client and packet monitoring
    int sfd, pfd;

    /// @brief Server address
    struct sockaddr_in saddr;

    /// @brief Packet address
    struct ifreq ifr;

    /// @brief Packet frame containing captured packet
    char frame[ETH_FRAME_LEN];

    /// @brief packet header information
    struct sockaddr_ll sall;

    /// @brief Struct to filter out ARP packets generated by using: tcpdump -dd arp 
    struct sock_filter arp_filter[4] = {
        { 0x28, 0, 0, 0x0000000c },
        { 0x15, 0, 1, 0x00000806 },
        { 0x06, 0, 0, 0x0000ffff },
        { 0x06, 0, 0, 0x00000000 }
    };

    /// @brief Struct to store BPF filter settings
    const struct sock_fprog bpf = {
        .len = (sizeof(arp_filter) / sizeof(arp_filter[0])),
        .filter = arp_filter
    };

    /// @brief Struct to store MAC and IP address
    struct packet_mac_ip{
        char mac[30];
        char ip[15];
    };

    /// @brief Setup client network settings (socket, bind)
    void SetupClientNet();

    /// @brief Setup packet mode network settings (socket, bind, promiscuous mode)
    void SetupPacketModeNet();

    /// @brief Parse ARP file to unordered map
    /// @return ARP file content as unordered map
    std::unordered_map<std::string, std::string> ParseArpFile();

    /// @brief Monitor ARP file for changes
    void MonitorArpFile();

    /// @brief Monitor ARP packets on interface
    void MonitorPackets();

    /// @brief Analyze frame and extract MAC and IP address
    /// @param frame Captured frame
    /// @param sall Packet header information
    /// @return Struct containing MAC and IP address
    packet_mac_ip AnalyzeFrame(char* frame, struct sockaddr_ll* sall);

    /// @brief Compare retrived data against ARP entries
    /// @param data Struct containing MAC and IP address
    void CompareArpEntries(const struct packet_mac_ip& data);

    /// @brief Send Info packet
    /// @param ProgramPackets::InfoPacketStruct infoPacket Info packet to send
    void SendInfoPacket(const ProgramPackets::InfoPacketStruct& infoPacket);

    /// @brief Print ARP map
    /// @param arpMap ARP map to print
    void PrintArpMap(const std::unordered_map<std::string, std::string>& arpMap);

    /// @brief Print ARP entry
    /// @param ip IP address
    /// @param mac MAC address
    /// @param type ARP entry type
    void PrintArpEntry(const std::string& ip, const std::string& mac, ProgramPackets::EntryType type);


};
