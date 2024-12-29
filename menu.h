#ifndef MENU_H
#define MENU_H

#include <cstdlib>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include "board.h"
#include "keyPress.h"

  int getTerminalWidth();
  void setRawMode(bool enable);
  void displayMenu(std::vector<std::string> items, int index);
  std::string centerText(std::string text, int width);
  KeyPress getKeyPress();



#endif //MENU_H