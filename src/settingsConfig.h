#ifndef SETTINGS_CONFIG_H
#define SETTINGS_CONFIG_H


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <limits>
#include "menu.h"

struct Config {
    int local_port = 12345;
    int peer_port = 12345;
    std::string white_pieces = "\x1B[1;37m";
    std::string black_pieces = "\x1B[1;91m";
    std::string board_color = "\x1B[1;90m";
    std::string alt_text_color = "\x1B[90m";
};

Config parseConfig(const std::string& filepath);
void seeColorOptions();

#endif