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