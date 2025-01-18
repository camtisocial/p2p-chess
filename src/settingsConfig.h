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
    std::string white_pieces = "white";
    std::string black_pieces = "black";
    std::string board_color = "bright_black";
};

Config parseConfig(const std::string& filepath);
void seeColorOptions();

#endif