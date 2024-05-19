#include "../include/Client.h"
#include "../include/StartupOptions.h"
#include "../include/Log.h"

#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>

Client::Client() {
    options = StartupOptions::GetInstance();
    arpMap = ParseArpFile();
    if(options->IsVerbose())
    {
        PrintArpMap(arpMap);
    }
}

Client::~Client() {
    arpMap.clear();
}

void Client::Run() {
    std::cout << std::endl;
    Log::MessageBlue("Client is running...");
    std::cout << std::endl;
    MonitorArpFile();
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

        // Check for changes
        bool changed = false;

        // Check for new entries or modified entries
        for (const auto& [ip, mac] : newArpMap) {
            if (arpMap.find(ip) == arpMap.end()) {
                PrintArpEntry(ip, mac, EntryType::new_entry);
                changed = true;
            } else if (arpMap[ip] != mac) {
                PrintArpEntry(ip, mac, EntryType::modified_entry);
                changed = true;
            }
        }

        // Check for removed entries
        for (const auto& [ip, mac] : arpMap) {
            if (newArpMap.find(ip) == newArpMap.end()) {
                PrintArpEntry(ip, mac, EntryType::deleted_entry);
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
        }
    }
}

void Client::PrintArpMap(const std::unordered_map<std::string, std::string> &arpMap){
    for (const auto& [ip, mac] : arpMap) {
        PrintArpEntry(ip, mac, EntryType::new_entry);
    }
}

void Client::PrintArpEntry(const std::string& ip, const std::string& mac, EntryType type){
    if (type == EntryType::new_entry)
    {
        Log::MessageGreen("New entry: IP = " + ip + ", MAC = " + mac);
    }
    else if (type == EntryType::modified_entry)
    {
        Log::MessageYellow("Modified entry: IP = " + ip + ", MAC changed from " + arpMap[ip] + " to " + mac);
    }
    else if (type == EntryType::deleted_entry)
    {
        Log::MessageRed("Removed entry: IP = " + ip + ", MAC = " + mac);
    }
    
}
