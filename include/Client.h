#pragma once

#include "StartupOptions.h"
#include "ProgramPackets.h"

#include <unordered_map>
#include <netinet/in.h>

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

    /// @brief Socket file descriptor
    int sfd;

    /// @brief Server address
    struct sockaddr_in saddr;

    /// @brief Setup client network settings (socket, bind)
    void SetupClientNet();

    /// @brief Parse ARP file to unordered map
    /// @return ARP file content as unordered map
    std::unordered_map<std::string, std::string> ParseArpFile();

    /// @brief Monitor ARP file for changes
    void MonitorArpFile();

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
