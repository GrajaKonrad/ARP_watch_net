#include "../include/Log.h"
#include "../include/StartupOptions.h"
#include "../include/Client.h"
#include "../include/Server.h"

#include <iostream>
#include <string.h>
#include <string_view>

int main(int argc, char* argv[]) {
    Log::MessageGreen("Welcome to ARP Watch Network !");
    std::cout<<std::endl;

    if (argc == 1) {
        Log::MessageBlue("Too few arguments passed ! \nPlease select client or server mode [ -c, -s] and specify file or packet mode [ -f, -p] !");
        return 0;
    }

    StartupOptions* startupOptions = StartupOptions::GetInstance();

    for (int i = 0; i < argc; i++) {
        const std::string_view arg = argv[i];

        if (arg == "-c") {
            Log::MessageBlue("Client mode selected !");
            startupOptions->ClientMode();
            if (++i < argc) {
                startupOptions->SetServerIP(argv[i]);
            } else {
                Log::MessageRed("Please specify Server IP !");
                return 0;
            }

            if (++i < argc) {
                startupOptions->SetServerPort(argv[i]);
            } else {
                Log::MessageRed("Please specify Server Port !");
                return 0;
            }
        } 
        if (arg == "-s") {
            Log::MessageBlue("Server mode selected !");
            startupOptions->ServerMode();
            if (++i < argc) {
                startupOptions->SetServerPort(argv[i]);
            } else {
                Log::MessageRed("Please specify Server Port !");
                return 0;
            }
        }
        if (arg == "-v") {
            Log::MessageBlue("Verbose mode selected !");
            startupOptions->VerboseOutput();
        }
        if (arg == "-a"){
            Log::MessageBlue("Own ARP Path selected !");
            startupOptions->CustomARPPath();
            if (++i < argc) {
                startupOptions->SetARPPath(argv[i]);
            } else {
                Log::MessageRed("Please specify ARP Path !");
                return 0;
            }
        }
        if (arg == "-t"){
            Log::MessageBlue("Own ARP Interval selected !");
            startupOptions->CustomARPInterval();
            if (++i < argc) {
                startupOptions->SetARPInterval(std::stoi(argv[i]));
            } else {
                Log::MessageRed("Please specify ARP Interval !");
                return 0;
            }
        }
        if (arg == "-f")
        {
            Log::MessageBlue("File mode selected !");
            startupOptions->FileModeSelected();   
        }
        if (arg == "-p")
        {
            Log::MessageBlue("Packet mode selected !");
            startupOptions->PacketModeSelected();
            if (++i < argc) {
                startupOptions->SetInterface(argv[i]);
            } else {   
                Log::MessageRed("Please specify Interface !");
                return 0;
            }
        }

        if (arg == "-h")
        {
            Log::MessageBlue("Help selected !");
            Log::MessageGreen("Usage: arp_watch_network [OPTIONS]");
            Log::MessageGreen("Options:");
            Log::MessageGreen("-c [IP] [PORT] - Client mode");
            Log::MessageGreen("-s [PORT] - Server mode");
            Log::MessageGreen("-f - Use ARP file to track changes in ARP table");
            Log::MessageGreen("-p [INTERFACE] - Use Packet Capture to identify new ARP entries");
            Log::MessageGreen("-v - Verbose output");
            Log::MessageGreen("-a [ARP_PATH] - CLIENT Only: Specified own ARP Path");
            Log::MessageGreen("-t [ARP_INTERVAL] - CLIENT Only: Specified own ARP watch arp-file-read interval");
            return 0;
        }
    }

    if (startupOptions->IsClient() && startupOptions->IsServer())
    {
        Log::MessageRed("Please select only one mode !");
        return 0;
    }
    else if (!startupOptions->IsClient() && !startupOptions->IsServer())
    {
        Log::MessageRed("Please select mode !");
        return 0;
    }

    if (startupOptions->IsClient() && startupOptions->IsFileMode() && startupOptions->IsPacketMode())
    {
        Log::MessageRed("Please select only one mode !");
        return 0;
    }
    else if (startupOptions->IsClient() && !startupOptions->IsFileMode() && !startupOptions->IsPacketMode())
    {
        Log::MessageRed("Please select mode !");
        return 0;
    }

    if(startupOptions->IsClient())
    {
        Client* client = new Client();
        client->Run();
        delete client;
    }
    else if(startupOptions->IsServer())
    {
        Server* server = new Server();
        server->Run();
        delete server;
    }
    
    delete startupOptions;
    return 0;
}
