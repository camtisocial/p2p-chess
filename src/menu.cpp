#include "menu.h"


int getTerminalWidth() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
}

std::string centerText(std::string text, int width) {
    int padding = ((width - text.length()) / 2);
    return std::string(padding, ' ') + text;
}


std::string centerChar(char character, int width) {
    int padding = ((width/2)-7);
    return std::string(padding, ' ') + character;
}

void setRawMode(bool enable) {
    static struct termios oldt, newt;
    if (enable) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }
}

KeyPress getKeyPressNonBlocking(std::string& input) {
    if (input.empty()) {
        return UNKNOWN;
    }

    if (input[0] == '\033' && input.size() >= 3) { 
        if (input[1] == '[') { // If the sequence starts with '['
            switch (input[2]) { // Check the third character
                case 'A': return UP; // Arrow up
                case 'B': return DOWN; // Arrow down
            }
        }
    } else if (input[0] == 'w' || input[0] == 'W') {
        return UP; // 'w' or 'W' key
    } else if (input[0] == 's' || input[0] == 'S') {
        return DOWN; // 's' or 'S' key
    } else if (input[0] == '\n') {
        return ENTER; // Enter key
    } else if (input[0] == ' ') {
        return ENTER; // Space key
    }

    return UNKNOWN; // Any other key
}

KeyPress getKeyPress() {
    char ch;
    read(STDIN_FILENO, &ch, 1);
    if (ch == '\033') {
        char seq[2];
        read(STDIN_FILENO, &seq[0], 1);
        read(STDIN_FILENO, &seq[1], 1);
        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return UP;
                case 'B': return DOWN;
                case 'C': return RIGHT;
                case 'D': return LEFT;
            }
        }
    } else if (ch == 'w' || ch == 'W') {
        return UP;
    } else if (ch == 's' || ch == 'S') {
        return DOWN;
    } else if (ch == '\n') {
        return ENTER;
    } else if (ch == ' ') {
        return ENTER;
    }
    return UNKNOWN;
}

std::string kbhit() {
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0); // Get the current file status flags
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK); // Set non-blocking mode

    char c;
    std::string input;
    while (read(STDIN_FILENO, &c, 1) > 0) {
        input += c;
    }

    fcntl(STDIN_FILENO, F_SETFL, oldf); // Restore original flags
    return input; // Return the input sequence

}

void clearLastLine() {
    std::cout << "\033[A\33[2K\r"; 
}

void displayMenu(const std::vector<std::string> options, int index) {
    int terminalWidth = getTerminalWidth();

    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << centerText("         _                   ", terminalWidth) << std::endl;
    std::cout << centerText("      ___| |__   ___  ___ ___  ", terminalWidth) << std::endl;
    std::cout << centerText("    / __| '_ \\ / _ \\/ __/ __|", terminalWidth) << std::endl;
    std::cout << centerText("   | (__| | | |  __/\\__ \\__ \\", terminalWidth) << std::endl;
    std::cout << centerText("    \\___|_| |_|\\___||___/___/", terminalWidth) << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    for (int i = 0; i < options.size(); ++i) {
        if (i == index) {
            std::cout << centerText("> " + options[i] + " <", terminalWidth) << std::endl;
        } else {
            std::cout << centerText("  " + options[i], terminalWidth) << std::endl;
        }
    }
}

void reviewOrReturn(std::vector<std::string> moveHistory, std::vector<std::vector<std::shared_ptr<ChessPiece>>>& board, std::string whitePieces, std::string blackPieces, std::string boardColor, std::string altTextColor, std::string lastMovedColor, int labelsOn) {
    std::vector<std::string> options = {"Review Game", "Return to Menu"};
    int selected{};
    int previousSelected = -1; 
    setRawMode(true);

    while (true) {
        if (selected != previousSelected) {
            system("clear");
            displayMenu(options, selected);
            previousSelected = selected; 
        }

        std::string input = kbhit();
        if (!input.empty()) {
            KeyPress key = getKeyPressNonBlocking(input);

            if (key == UP) {
                selected = (selected - 1 + options.size()) % options.size();
            } else if (key == DOWN) {
                selected = (selected + 1) % options.size();
            } else if (key == ENTER) {
                if (options[selected] == "Review Game") {
                    reviewGame(moveHistory, board, whitePieces, blackPieces, boardColor, altTextColor, lastMovedColor, labelsOn);
                    break;
                } else if (options[selected] == "Return to Menu") {
                    break;
                }
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

}

void setLocalColor(udp::socket& socket, udp::endpoint& peer_endpoint, bool& localColor) {
 std::vector<std::string> options = {"White", "Black"};
    int selected{0};
    int previousSelected = -1; // To track changes in selection

    setRawMode(true);

    while (!playerPickedColor) {
        // Only redraw the menu if the selection changes
        if (selected != previousSelected) {
            system("clear");
            displayMenu(options, selected);
            previousSelected = selected; 
        }

        if (playerPickedColor) {
            break;
        }

        std::string input = kbhit();
        if (!input.empty()) {
            KeyPress key = getKeyPressNonBlocking(input);

            if (key == UP) {
                selected = (selected - 1 + options.size()) % options.size();
            } else if (key == DOWN) {
                selected = (selected + 1) % options.size();
            } else if (key == ENTER) {
                if (options[selected] == "White") {
                    localColor = 0;
                    socket.send_to(boost::asio::buffer("W"), peer_endpoint);
                    playerPickedColor = true;
                    localPickedColor = true;
                    break;
                } else if (options[selected] == "Black") {
                    localColor = 1;
                    socket.send_to(boost::asio::buffer("B"), peer_endpoint);
                    playerPickedColor = true;
                    localPickedColor = true;
                    break;
                }
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    setRawMode(false);
}

std::string setPeerIP() {
    std::string ip;
    setRawMode(false);
    while (true) {
        std::cout << centerText("Enter peer IP: ", (getTerminalWidth()-10));
        std::cin >> ip;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        setRawMode(true);
        return ip;
        }
}

int setPeerPort() {
    int port{12345};
    std::string u_input;
    setRawMode(false);

    while (true) {
        std::cout << centerText("Enter peer port: ", (getTerminalWidth()-7));
        std::getline(std::cin, u_input);

        if (u_input.empty()) {
            std::cout << std::endl;
            std::cout << centerText("using default port: 12345", getTerminalWidth()) << std::endl;
            sleep(2);
            break;
        } else {
            try {
                int tmpInt= std::stoi(u_input);
                if ( tmpInt <= 0 || tmpInt > 65535) {
                    std::cout << std::endl;
                    std::cout << centerText("Invalid port number, using default", getTerminalWidth()) << std::endl;
                    sleep(2);
                    break;
                } else {
                    port = tmpInt;
                    break;
                }
            } catch (const std::out_of_range& e) {
                std::cout << centerText("Port number out of range, using default", getTerminalWidth()) << std::endl;
                sleep(2);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << centerText("Invalid input, using default", getTerminalWidth()) << std::endl;
                sleep(2);
                break;
            }
        }

    }
    setRawMode(true);
    return port;
}

int setLocalPort() {
    int port{12345};
    std::string u_input;
    setRawMode(false);

    while (true) {
        std::cout << std::endl;
        std::cout << centerText("Enter port or leave empty to use default (12345): ", getTerminalWidth());
        std::getline(std::cin, u_input);

        if (u_input.empty()) {
            std::cout << std::endl;
            std::cout << centerText("using default port: 12345", getTerminalWidth()) << std::endl;
            sleep(2);
            break;
        } else {
            try {
                int tmpInt= std::stoi(u_input);
                if ( tmpInt <= 0 || tmpInt > 65535) {
                    std::cout << std::endl;
                    std::cout << centerText("Invalid port number, using default", getTerminalWidth()) << std::endl;
                    sleep(2);
                    break;
                } else {
                    port = tmpInt;
                    break;
                }
            } catch (const std::out_of_range& e) {
                std::cout << centerText("Port number out of range, using default", getTerminalWidth()) << std::endl;
                sleep(2);
                break;
            } catch (const std::invalid_argument& e) {
                std::cout << centerText("Invalid input, using default", getTerminalWidth()) << std::endl;
                sleep(2);
                break;
            }
        }

    }
    setRawMode(true);
    return port;
}



void announceGameResult(char result) {
    if (result == 'B') {
        std::cout << std::endl;
        std::cout << centerText("    Black wins by checkmate", getTerminalWidth()) << std::endl;
        std::cout << centerText("    Press enter to continue", getTerminalWidth()) << std::endl;
    } else if (result == 'W') {
        std::cout << std::endl;
        std::cout << centerText("    White wins by checkmate", getTerminalWidth()) << std::endl;
        std::cout << centerText("    Press enter to continue", getTerminalWidth()) << std::endl;
    } else if (result == 'D') {
        std::cout << std::endl;
        std::cout << centerText("  Draw", getTerminalWidth()) << std::endl;
        std::cout << centerText("   Press enter to continue", getTerminalWidth()) << std::endl;
    } else if (result == 'b') {
        std::cout << std::endl;
        std::cout << centerText("   White resigned, Black wins", getTerminalWidth()) << std::endl;
        std::cout << centerText("    Press enter to continue", getTerminalWidth()) << std::endl;
    } else if (result == 'w') {
        std::cout << std::endl;
        std::cout << centerText("   Black resigned, White wins", getTerminalWidth()) << std::endl;
        std::cout << centerText("    Press enter to continue", getTerminalWidth()) << std::endl;
    } else if (result == 'q') {
        std::cout << std::endl;
        std::cout << centerText("   Player disconnected", getTerminalWidth()) << std::endl;
        std::cout << centerText("   Press enter to continue", getTerminalWidth()) << std::endl;
    }
}

