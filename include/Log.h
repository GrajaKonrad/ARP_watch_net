#pragma once

#include <iostream>
#include <string_view>

/// @brief Klasa do logowania wiadomości do konsoli
class Log{

public:
    /// @brief Green text color
    static constexpr char ColorGreen[] = "\033[1;32m";

    /// @brief Blue text color
    static constexpr char ColorBlue[] = "\033[1;34m";

    /// @brief Red text color
    static constexpr char ColorRed[] = "\033[1;31m";

    /// @brief Yellow text color
    static constexpr char ColorYellow[] = "\033[1;33m";

    /// @brief Clear text color
    static constexpr char ResetColor[] = "\033[0m";


    /// @brief Prints message in console selected color
    /// @param message message to print on stdout
    static const void MessageNormal(const std::string_view message) {
        std::cout << ResetColor << message << std::endl;
    }

    /// @brief Prints message in green color
    /// @param message message to print on stdout
    static const void MessageGreen(const std::string_view message) {
        std::cout << ColorGreen << message << ResetColor << std::endl;
    }

    /// @brief Prints message in blue color
    /// @param message message to print on stdout
    static const void MessageBlue(const std::string_view message) {
        std::cout << ColorBlue << message << ResetColor << std::endl;
    }

    /// @brief Prints message in red color
    /// @param message message to print on stdout
    static const void MessageRed(const std::string_view message) {
        std::cout << ColorRed << message << ResetColor << std::endl;
    }

    /// @brief Prints message in yellow color
    /// @param message message to print on stdout
    static const void MessageYellow(const std::string_view message) {
        std::cout << ColorYellow<< message << ResetColor << std::endl;
    }
};
