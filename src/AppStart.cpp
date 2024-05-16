#include "../include/Log.h"
#include "../include/StartupOptions.h"

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

    for (int i = 0; i < argc; i++) {
        const std::string_view arg = argv[i];

        //Should be always last checked
        if (arg == "-c") {
            Log::MessageBlue("Client mode selected !");
            StartupOptions::ClientMode();
        } 
        if (arg == "-s") {
            Log::MessageBlue("Server mode selected !");
            StartupOptions::ServerMode();
        }
    }
    return 0;
}
