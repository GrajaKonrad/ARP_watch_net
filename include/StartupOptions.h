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

    /// @brief Check if client mode is set
    /// @return true if client mode is set
    bool IsClient() { return options.test(0); }

    /// @brief Check if server mode is set
    /// @return true if server mode is set
    bool IsServer() { return options.test(1); }

    /// @brief Check if verbose mode is set
    /// @return true if verbose mode is set
    bool IsVerbose() { return options.test(2); }

    /// @brief Set ARP Path
    /// @param path Path to ARP file
    void SetARPPath(const std::string& path) { arpPath = path; }

    /// @brief Get ARP Path
    /// @return Path to ARP file
    std::string GetARPPath() { return arpPath; }

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
    std::bitset<5> options;

    /// @brief Path to ARP file
    std::string arpPath;

    /// @brief ARP watch interval
    int16_t arpInterval;
};