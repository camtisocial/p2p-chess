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
                            config.white_pieces = "\x1B[1;37m";
                        } else if (value == "black") {
                            config.white_pieces = "\x1B[1;30m";
                        } else if (value == "red") {
                            config.white_pieces = "\x1B[1;31m";
                        } else if (value == "green") {
                            config.white_pieces = "\x1B[1;32m";
                        } else if (value == "yellow") {
                            config.white_pieces = "\x1B[1;33m";
                        } else if (value == "blue") {
                            config.white_pieces = "\x1B[1;34m";
                        } else if (value == "magenta") {
                            config.white_pieces = "\x1B[1;35m";
                        } else if (value == "cyan") {
                            config.white_pieces = "\x1B[1;36m";
                        } else if (value == "bright_white") {
                            config.white_pieces = "\x1B[1;97m";
                        } else if (value == "bright_black") {
                            config.white_pieces = "\x1B[1;90m";
                        } else if (value == "bright_red") {
                            config.white_pieces = "\x1B[1;91m";
                        } else if (value == "bright_green") {
                            config.white_pieces = "\x1B[1;92m";
                        } else if (value == "bright_yellow") {
                            config.white_pieces = "\x1B[1;93m";
                        } else if (value == "bright_blue") {
                            config.white_pieces = "\x1B[1;94m";
                        } else if (value == "bright_magenta") {
                            config.white_pieces = "\x1B[1;95m";
                        } else if (value == "bright_cyan") {
                            config.white_pieces = "\x1B[1;96m";
                        }
                    } else if (key == "black_pieces") {
                        if (value == "white") {
                            config.black_pieces = "\x1B[1;37m";
                        } else if (value == "black") {
                            config.black_pieces = "\x1B[1;30m";
                        } else if (value == "red") {
                            config.black_pieces = "\x1B[1;31m";
                        } else if (value == "green") {
                            config.black_pieces = "\x1B[1;32m";
                        } else if (value == "yellow") {
                            config.black_pieces = "\x1B[1;33m";
                        } else if (value == "blue") {
                            config.black_pieces = "\x1B[1;34m";
                        } else if (value == "magenta") {
                            config.black_pieces = "\x1B[1;35m";
                        } else if (value == "cyan") {
                            config.black_pieces = "\x1B[1;36m";
                        } else if (value == "bright_white") {
                            config.black_pieces = "\x1B[1;97m";
                        } else if (value == "bright_black") {
                            config.black_pieces = "\x1B[1;90m";
                        } else if (value == "bright_red") {
                            config.black_pieces = "\x1B[1;91m";
                        } else if (value == "bright_green") {
                            config.black_pieces = "\x1B[1;92m";
                        } else if (value == "bright_yellow") {
                            config.black_pieces = "\x1B[1;93m";
                        } else if (value == "bright_blue") {
                            config.black_pieces = "\x1B[1;94m";
                        } else if (value == "bright_magenta") {
                            config.black_pieces = "\x1B[1;95m";
                        } else if (value == "bright_cyan") {
                            config.black_pieces = "\x1B[1;96m";
                        }
                    }
                    else if (key == "board_color") {
                        if (value == "white") {
                            config.board_color = "\x1B[1;37m";
                        } else if (value == "black") {
                            config.board_color = "\x1B[1;30m";
                        } else if (value == "red") {
                            config.board_color = "\x1B[1;31m";
                        } else if (value == "green") {
                            config.board_color = "\x1B[1;32m";
                        } else if (value == "yellow") {
                            config.board_color = "\x1B[1;33m";
                        } else if (value == "blue") {
                            config.board_color = "\x1B[1;34m";
                        } else if (value == "magenta") {
                            config.board_color = "\x1B[1;35m";
                        } else if (value == "cyan") {
                            config.board_color = "\x1B[1;36m";
                        } else if (value == "bright_white") {
                            config.board_color = "\x1B[1;97m";
                        } else if (value == "bright_black") {
                            config.board_color = "\x1B[1;90m";
                        } else if (value == "bright_red") {
                            config.board_color = "\x1B[1;91m";
                        } else if (value == "bright_green") {
                            config.board_color = "\x1B[1;92m";
                        } else if (value == "bright_yellow") {
                            config.board_color = "\x1B[1;93m";
                        } else if (value == "bright_blue") {
                            config.board_color = "\x1B[1;94m";
                        } else if (value == "bright_magenta") {
                            config.board_color = "\x1B[1;95m";
                        } else if (value == "bright_cyan") {
                            config.board_color = "\x1B[1;96m";
                        }
                    } else if (key == "alt_text_color") {
                        if (value == "white") {
                            config.alt_text_color = "\x1B[1;37m";
                        } else if (value == "black") {
                            config.alt_text_color = "\x1B[1;30m";
                        } else if (value == "red") {
                            config.alt_text_color = "\x1B[1;31m";
                        } else if (value == "green") {
                            config.alt_text_color = "\x1B[1;32m";
                        } else if (value == "yellow") {
                            config.alt_text_color = "\x1B[1;33m";
                        } else if (value == "blue") {
                            config.alt_text_color = "\x1B[1;34m";
                        } else if (value == "magenta") {
                            config.alt_text_color = "\x1B[1;35m";
                        } else if (value == "cyan") {
                            config.alt_text_color = "\x1B[1;36m";
                        } else if (value == "bright_white") {
                            config.alt_text_color = "\x1B[1;97m";
                        } else if (value == "bright_black") {
                            config.alt_text_color = "\x1B[1;90m";
                        } else if (value == "bright_red") {
                            config.alt_text_color = "\x1B[1;91m";
                        } else if (value == "bright_green") {
                            config.alt_text_color = "\x1B[1;92m";
                        } else if (value == "bright_yellow") {
                            config.alt_text_color = "\x1B[1;93m";
                        } else if (value == "bright_blue") {
                            config.alt_text_color = "\x1B[1;94m";
                        } else if (value == "bright_magenta") {
                            config.alt_text_color = "\x1B[1;95m";
                        } else if (value == "bright_cyan") {
                            config.alt_text_color = "\x1B[1;96m";
                        }
                    } else if (key == "last_moved_piece") {
                        if (value == "white") {
                            config.last_moved_color = "\x1B[1;37m";
                        } else if (value == "black") {
                            config.last_moved_color = "\x1B[1;30m";
                        } else if (value == "red") {
                            config.last_moved_color = "\x1B[1;31m";
                        } else if (value == "green") {
                            config.last_moved_color = "\x1B[1;32m";
                        } else if (value == "yellow") {
                            config.last_moved_color = "\x1B[1;33m";
                        } else if (value == "blue") {
                            config.last_moved_color = "\x1B[1;34m";
                        } else if (value == "magenta") {
                            config.last_moved_color = "\x1B[1;35m";
                        } else if (value == "cyan") {
                            config.last_moved_color = "\x1B[1;36m";
                        } else if (value == "bright_white") {
                            config.last_moved_color = "\x1B[1;97m";
                        } else if (value == "bright_black") {
                            config.last_moved_color = "\x1B[1;90m";
                        } else if (value == "bright_red") {
                            config.last_moved_color = "\x1B[1;91m";
                        } else if (value == "bright_green") {
                            config.last_moved_color = "\x1B[1;92m";
                        } else if (value == "bright_yellow") {
                            config.last_moved_color = "\x1B[1;93m";
                        } else if (value == "bright_blue") {
                            config.last_moved_color = "\x1B[1;94m";
                        } else if (value == "bright_magenta") {
                            config.last_moved_color = "\x1B[1;95m";
                        } else if (value == "bright_cyan") {
                            config.last_moved_color = "\x1B[1;96m";
                        }

                    } else if (key == "last_moved") {
                        bool tmp = std::stoi(value);
                        config.last_moved = tmp;
                    } else if (key == "labels_on") {
                        config.labels_on = std::stoi(value);
                    }
                } else if (section == "Stockfish") {
                    if (key == "stockfish_path") {
                     config.stockfish_path = value;
                    } else if (key == "eval_depth") {
                        config.eval_depth = std::stoi(value);
                    } 
                }
            }
        }
    }
    return config;
}

void seeColorOptions() {
    std::cout << std::endl;
    int terminalWidth = (getTerminalWidth()/2)-13;
    std::cout << std::string(terminalWidth-5, ' ') + "Modify settings.ini to change colors)" << std::endl;
    std::cout << std::string(terminalWidth-5, ' ') + "(usr/share/terminalChess/settings.ini)" << std::endl;
    std::cout << std::endl;
    std::cout << "\x1B[1;31m" << std::string(terminalWidth+4, ' ') + "This is red" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;32m" << std::string(terminalWidth+4, ' ') + "This is green" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;33m" << std::string(terminalWidth+4, ' ') + "This is yellow" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;34m" << std::string(terminalWidth+4, ' ') + "This is blue" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;35m" << std::string(terminalWidth+4, ' ') + "This is magenta" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;36m" << std::string(terminalWidth+4, ' ') + "This is cyan" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;37m" << std::string(terminalWidth+4, ' ') + "This is white" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;30m" << std::string(terminalWidth+4, ' ') + "This is black" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;91m" << std::string(terminalWidth+4, ' ') + "This is bright_red" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;92m" << std::string(terminalWidth+4, ' ') + "This is bright_green" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;93m" << std::string(terminalWidth+4, ' ') + "This is bright_yellow" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;94m" << std::string(terminalWidth+4, ' ') + "This is bright_blue" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;95m" << std::string(terminalWidth+4, ' ') + "This is bright_magenta" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;96m" << std::string(terminalWidth+4, ' ') + "This is bright_cyan" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;97m" << std::string(terminalWidth+4, ' ') + "This is bright_white" << "\033[0m" << std::endl;
    std::cout << "\x1B[1;90m" << std::string(terminalWidth+4, ' ') + "This is bright_black" << "\033[0m" << std::endl;
    std::cout << std::endl;
    std::cout << centerText ("  Press enter to continue", getTerminalWidth()) << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}




















