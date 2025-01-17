#include "settingsConfig.h"

Config parseConfig(const std::string& filepath) {
    Config config;
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Error: Cannot open config file: " << filepath << std::endl;
        return config;
    }

    std::string line, section;
    while (std::getline(file, line)) {
        // Remove comments
        size_t commentPos = line.find('#');
        if (commentPos != std::string::npos) line = line.substr(0, commentPos);

        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        // Handle sections
        if (line.front() == '[' && line.back() == ']') {
            section = line.substr(1, line.size() - 2);
        } else {
            // Parse key-value pairs
            size_t equalPos = line.find('=');
            if (equalPos != std::string::npos) {
                std::string key = line.substr(0, equalPos);
                std::string value = line.substr(equalPos + 1);

                // Trim whitespace
                key.erase(0, key.find_first_not_of(" \t\r\n"));
                key.erase(key.find_last_not_of(" \t\r\n") + 1);
                value.erase(0, value.find_first_not_of(" \t\r\n"));
                value.erase(value.find_last_not_of(" \t\r\n") + 1);

                // Assign values based on the section
                if (section == "Network") {
                    if (key == "local_port") config.local_port = std::stoi(value);
                    else if (key == "peer_port") config.peer_port = std::stoi(value);
                } else if (section == "Appearance") {
                    if (key == "white_pieces") config.white_pieces= value;
                    else if (key == "black_pieces") config.black_pieces= value;
                    else if (key == "board_color") config.board_color = value;
                }
            }
        }
    }
    return config;
}
