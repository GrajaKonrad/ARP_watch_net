#pragma once

#include "StartupOptions.h"

#include <unordered_map>

/// @brief ARP entry type
enum class EntryType : uint16_t {
    new_entry = 0,
    modified_entry = 1,
    deleted_entry = 2,
};


class Client
{

public:
    Client();
    void Run();
    ~Client();

private:
    std::unordered_map<std::string, std::string> arpMap;
    StartupOptions* options;

    std::unordered_map<std::string, std::string> ParseArpFile();
    void MonitorArpFile();
    void PrintArpMap(const std::unordered_map<std::string, std::string>& arpMap);
    void PrintArpEntry(const std::string& ip, const std::string& mac, EntryType type);


};
