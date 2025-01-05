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

bool setLocalColor() {
    vector<std::string> options = {"White", "Black"};
    int selected{0};
    setRawMode(true);
    while (true) {
        system("clear");
        displayMenu(options, selected);
        KeyPress key = getKeyPress();
               if (key == UP) {
            selected = (selected - 1 + options.size()) % options.size();
        } else if (key == DOWN) {
            selected = (selected + 1) % options.size();
        } else if (key == ENTER) {
            if (options[selected] == "White") {
                std::cout << std::endl;
                std::cout << centerText("You play white", getTerminalWidth()) << std::endl;
                setRawMode(false);
                return 0;
            } else if (options[selected] == "Black") {
                std::cout << std::endl;
                std::cout << centerText("You play black", getTerminalWidth()) << std::endl;
                setRawMode(false);
                system("clear");
                return 1;
            }
        } 
    }
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
                // std::cout << centerText("Port number: ", getTerminalWidth()) << u_input << std::endl;
                // std::cout << centerText("Port number string: ", getTerminalWidth()) << tmpInt << std::endl;
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
                // std::cout << centerText("Port number: ", getTerminalWidth()) << u_input << std::endl;
                // std::cout << centerText("Port number string: ", getTerminalWidth()) << tmpInt << std::endl;
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
