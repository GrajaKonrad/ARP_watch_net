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

    /// @brief ARP packet structure
    struct arphdr {
        uint16_t ar_hrd;     /* format of hardware address */
        uint16_t ar_pro;     /* format of protocol address */
        uint8_t  ar_hln;     /* length of hardware address */
        uint8_t  ar_pln;     /* length of protocol address */
        uint16_t ar_op;      /* ARP opcode (command) */
        uint8_t  ar_sha[6];  /* sender hardware address */
        uint8_t  ar_sip[4];  /* sender IP address */
        uint8_t  ar_tha[6];  /* target hardware address */
        uint8_t  ar_tip[4];  /* target IP address */
    };
}

