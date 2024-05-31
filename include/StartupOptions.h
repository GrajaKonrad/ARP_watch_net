#pragma once

#include <bitset>

class StartupOptions
{
public:
    /// @brief Return instance of options
    /// @return startup options instance
    static StartupOptions* GetInstance();

    /// @brief Set client mode flag
    void ClientMode() { options.set(0); }

    /// @brief Set server mode flag
    void ServerMode() { options.set(1); }

    /// @brief Verbose output
    void VerboseOutput() { options.set(2); }

    /// @brief CLIENT Only: Specified own ARP Path
    void CustomARPPath() { options.set(3); }

    /// @brief CLIENT Only: Specified own ARP watch arp-file-read interval
    void CustomARPInterval() { options.set(4); }

    /// @brief Use ARP file to track changes in ARP table
    void FileModeSelected() { options.set(5); }

    /// @brief Use Packet Capture to identify new ARP entries
    void PacketModeSelected() { options.set(6); }

    /// @brief Check if client mode is set
    /// @return true if client mode is set
    bool IsClient() { return options.test(0); }

    /// @brief Check if server mode is set
    /// @return true if server mode is set
    bool IsServer() { return options.test(1); }

    /// @brief Check if verbose mode is set
    /// @return true if verbose mode is set
    bool IsVerbose() { return options.test(2); }

    /// @brief Check if file mode is selected
    /// @return true if file mode is selected
    bool IsFileMode() { return options.test(5); }

    /// @brief Check if packet mode is selected
    /// @return true if packet mode is selected
    bool IsPacketMode() { return options.test(6); }

    /// @brief Set Server IP
    /// @param ip Server IP
    void SetServerIP(const std::string& ip) { serverIP = ip; }

    /// @brief Get Server IP
    /// @return Server IP
    std::string GetServerIP() { return serverIP; }

    /// @brief Set Server Port
    /// @param port Server Port
    void SetServerPort(const std::string& port) { serverPort = port; }

    /// @brief Get Server Port
    /// @return Server Port
    std::string GetServerPort() { return serverPort; }

    /// @brief Set ARP Path
    /// @param path Path to ARP file
    void SetARPPath(const std::string& path) { arpPath = path; }

    /// @brief Get ARP Path
    /// @return Path to ARP file
    std::string GetARPPath() { return arpPath; }

    /// @brief Set Interface
    /// @param iface Interface to monitor
    void SetInterface(const std::string& iface) { interface = iface; }

    /// @brief Get Interface
    /// @return Interface to monitor
    std::string GetInterface() { return interface; }

    /// @brief Set ARP watch interval
    /// @param interval ARP watch interval
    void SetARPInterval(int interval) { arpInterval = interval; }

    /// @brief Get ARP watch interval
    /// @return ARP watch interval
    int GetARPInterval() { return arpInterval; }

private:
    /// @brief Private constructor to prevent creating options instance
    StartupOptions();


private:
    /// @brief Instance of startup options
    static StartupOptions* options_instance;

    /// @brief Bitset to store startup options
    std::bitset<7> options;

    /// @brief Server IP
    std::string serverIP;

    /// @brief Server port
    std::string serverPort;

    /// @brief Interface to monitor
    std::string interface;

    /// @brief Path to ARP file
    std::string arpPath;

    /// @brief ARP watch interval
    int16_t arpInterval;
};