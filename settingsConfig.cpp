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
                    if (key == "white_pieces"){
                       config.white_pieces= value;
                       if (value == "white") {
                            config.white_pieces = "\033[37m";
                       } else if (value == "black") {
                            config.white_pieces = "\033[30m";
                       } else if (value == "red") {
                            config.white_pieces = "\033[31m";
                       } else if (value == "green") {
                            config.white_pieces = "\033[32m";
                       } else if (value == "yellow") {
                            config.white_pieces = "\033[33m";
                       } else if (value == "blue") {
                            config.white_pieces = "\033[34m";
                       } else if (value == "magenta") {
                            config.white_pieces = "\033[35m";
                       } else if (value == "cyan") {
                            config.white_pieces = "\033[36m";
                       } else if (value == "bright_white") {
                            config.white_pieces = "\033[97m";
                       } else if (value == "bright_black") {
                            config.white_pieces = "\033[90m";
                       } else if (value == "bright_red") {
                            config.white_pieces = "\033[91m";
                       } else if (value == "bright_green") {
                            config.white_pieces = "\033[92m";
                       } else if (value == "bright_yellow") {
                            config.white_pieces = "\033[93m";
                       } else if (value == "bright_blue") {
                            config.white_pieces = "\033[94m";
                       } else if (value == "bright_magenta") {
                            config.white_pieces = "\033[95m";
                       } else if (value == "bright_cyan") {
                            config.white_pieces = "\033[96m";
                       }
                    } else if (key == "black_pieces") {
                        if (value == "white") {
                            config.black_pieces = "\033[37m";
                        } else if (value == "black") {
                            config.black_pieces = "\033[30m";
                        } else if (value == "red") {
                            config.black_pieces = "\033[31m";
                        } else if (value == "green") {
                            config.black_pieces = "\033[32m";
                        } else if (value == "yellow") {
                            config.black_pieces = "\033[33m";
                        } else if (value == "blue") {
                            config.black_pieces = "\033[34m";
                        } else if (value == "magenta") {
                            config.black_pieces = "\033[35m";
                        } else if (value == "cyan") {
                            config.black_pieces = "\033[36m";
                        } else if (value == "bright_white") {
                            config.black_pieces = "\033[97m";
                        } else if (value == "bright_black") {
                            config.black_pieces = "\033[90m";
                        } else if (value == "bright_red") {
                            config.black_pieces = "\033[91m";
                        } else if (value == "bright_green") {
                            config.black_pieces = "\033[92m";
                        } else if (value == "bright_yellow") {
                            config.black_pieces = "\033[93m";
                        } else if (value == "bright_blue") {
                            config.black_pieces = "\033[94m";
                        } else if (value == "bright_magenta") {
                            config.black_pieces = "\033[95m";
                        } else if (value == "bright_cyan") {
                            config.black_pieces = "\033[96m";
                        }
                    }
                    else if (key == "board_color") {
                        if (value == "white") {
                            config.board_color = "\033[37m";
                        } else if (value == "black") {
                            config.board_color = "\033[30m";
                        } else if (value == "red") {
                            config.board_color = "\033[31m";
                        } else if (value == "green") {
                            config.board_color = "\033[32m";
                        } else if (value == "yellow") {
                            config.board_color = "\033[33m";
                        } else if (value == "blue") {
                            config.board_color = "\033[34m";
                        } else if (value == "magenta") {
                            config.board_color = "\033[35m";
                        } else if (value == "cyan") {
                            config.board_color = "\033[36m";
                        } else if (value == "bright_white") {
                            config.board_color = "\033[97m";
                        } else if (value == "bright_black") {
                            config.board_color = "\033[90m";
                        } else if (value == "bright_red") {
                            config.board_color = "\033[91m";
                        } else if (value == "bright_green") {
                            config.board_color = "\033[92m";
                        } else if (value == "bright_yellow") {
                            config.board_color = "\033[93m";
                        } else if (value == "bright_blue") {
                            config.board_color = "\033[94m";
                        } else if (value == "bright_magenta") {
                            config.board_color = "\033[95m";
                        } else if (value == "bright_cyan") {
                            config.board_color = "\033[96m";
                        }
                    }
                }
            }
        }
    }
    return config;
}

void seeColorOptions() {
    std::cout << std::endl;
    int terminalWidth = ((getTerminalWidth())/2.4);
    std::cout << std::string(terminalWidth-(terminalWidth*0.15), ' ') + "Modify settings.ini to change colors" << std::endl;
    std::cout << std::endl;
    std::cout << "\033[31m" << std::string(terminalWidth, ' ') + "This is red" << "\033[0m" << std::endl;
    std::cout << "\033[91m" << std::string(terminalWidth, ' ') + "This is bright red" << "\033[0m" << std::endl;
    std::cout << "\033[32m" << std::string(terminalWidth, ' ') + "This is green" << "\033[0m" << std::endl;
    std::cout << "\033[92m" << std::string(terminalWidth, ' ') + "This is bright green" << "\033[0m" << std::endl;
    std::cout << "\033[33m" << std::string(terminalWidth, ' ') + "This is yellow" << "\033[0m" << std::endl;
    std::cout << "\033[93m" << std::string(terminalWidth, ' ') + "This is bright yellow" << "\033[0m" << std::endl;
    std::cout << "\033[34m" << std::string(terminalWidth, ' ') + "This is blue" << "\033[0m" << std::endl;
    std::cout << "\033[94m" << std::string(terminalWidth, ' ') + "This is bright blue" << "\033[0m" << std::endl;
    std::cout << "\033[35m" << std::string(terminalWidth, ' ') + "This is magenta" << "\033[0m" << std::endl;
    std::cout << "\033[95m" << std::string(terminalWidth, ' ') + "This is bright magenta" << "\033[0m" << std::endl;
    std::cout << "\033[36m" << std::string(terminalWidth, ' ') + "This is cyan" << "\033[0m" << std::endl;
    std::cout << "\033[96m" << std::string(terminalWidth, ' ') + "This is bright cyan" << "\033[0m" << std::endl;
    std::cout << "\033[37m" << std::string(terminalWidth, ' ') + "This is white" << "\033[0m" << std::endl;
    std::cout << "\033[97m" << std::string(terminalWidth, ' ') + "This is bright white" << "\033[0m" << std::endl;
    std::cout << "\033[30m" << std::string(terminalWidth, ' ') + "This is black" << "\033[0m" << std::endl;
    std::cout << "\033[90m" << std::string(terminalWidth, ' ') + "This is bright black" << "\033[0m" << std::endl;
    std::cout << std::endl;
    std::cout << std::string(terminalWidth-(terminalWidth*0.15), ' ') + "press enter to return to main menu" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}