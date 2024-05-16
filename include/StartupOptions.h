#include <bitset>

class StartupOptions
{
public:
    /// @brief Set client mode flag
    static constexpr void ClientMode() { options.set(0); }

    /// @brief Set server mode flag
    static constexpr void ServerMode() { options.set(1); }
private:

    /// @brief Bitset to store startup options
    static std::bitset<2> options;
};