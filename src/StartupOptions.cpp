#include "../include/StartupOptions.h"

StartupOptions* StartupOptions::options_instance = NULL;

StartupOptions* StartupOptions::GetInstance() {
    if (options_instance == nullptr) {
        options_instance = new StartupOptions();
    }
    return options_instance;
}

StartupOptions::StartupOptions() {
    options.reset();

    arpPath = "/proc/net/arp";
    arpInterval = 10;
}