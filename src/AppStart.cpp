#include "../include/Log.h"
#include "../include/StartupOptions.h"
#include "../include/Client.h"

#include <iostream>
#include <string.h>
#include <string_view>

int main(int argc, char* argv[]) {
    Log::MessageGreen("Welcome to ARP Watch Network !");
    std::cout<<std::endl;

    if (argc == 1) {
        Log::MessageBlue("Please select client or server mode [ -c, -s] !");
        return 0;
    }

    StartupOptions* startupOptions = StartupOptions::GetInstance();

    for (int i = 0; i < argc; i++) {
        const std::string_view arg = argv[i];

        if (arg == "-c") {
            Log::MessageBlue("Client mode selected !");
            startupOptions->ClientMode();
        } 
        if (arg == "-s") {
            Log::MessageBlue("Server mode selected !");
            startupOptions->ServerMode();
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
    }

    if(startupOptions->IsClient())
    {
        Client* client = new Client();
        client->Run();
        delete client;
    }
    
    delete startupOptions;
    return 0;
}
