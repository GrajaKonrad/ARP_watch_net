#pragma once

#include "StartupOptions.h"
#include "ProgramPackets.h"

#include <netinet/in.h>

class Server
{
public:
    /// @brief Server constructor
    Server();

    /// @brief Run server
    void Run();

    /// @brief Server destructor
    ~Server();

private:
    /// @brief Application startup options
    StartupOptions* options;

    /// @brief Socket file descriptor
    int sfd;

    /// @brief Server address
    struct sockaddr_in saddr;

    /// @brief Setup server network settings (socket, bind)
    void SetupServerNet();

    /// @brief Receive message from clients
    void ReciveMessage();

    /// @brief Print ARP entry
    void PrintArpEntry(const std::string& ip, const std::string& mac, ProgramPackets::EntryType type);
};