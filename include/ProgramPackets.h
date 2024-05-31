#pragma once

#include <string>

namespace ProgramPackets
{
    /// @brief ARP entry type
    enum class EntryType : uint16_t {
        new_entry = 0,
        modified_entry = 1,
        deleted_entry = 2,
    };

    /// @brief Info packet structure
    struct InfoPacketStruct
    {
        EntryType type;
        std::string ip_address;
        std::string mac_address;
    };
}

