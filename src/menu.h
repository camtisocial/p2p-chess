#ifndef MENU_H
#define MENU_H

#include <cstdlib>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <boost/asio.hpp>
#include "globals.h"
#include "board.h"
#include "keyPress.h"

using boost::asio::ip::udp;


//@@@@@@@@@@@@@@@@@@@@@@  UTILITIES @@@@@@@@@@@@@@@@@@@@@@@@@@
  int getTerminalWidth();
  std::string centerChar(char character, int width);
  std::string centerText(std::string text, int width);
  KeyPress getKeyPress();
  void clearLastLine();
//@@@@@@@@@@@@@@@@@@@@@@  MENU  @@@@@@@@@@@@@@@@@@@@@@@@@@
  std::string setPeerIP();
  int setPeerPort();
  int setLocalPort();
  void setRawMode(bool enable);
  void displayMenu(std::vector<std::string> items, int index);
  bool setLocalColor(udp::socket& socket, udp::endpoint& peer_endpoint);
//@@@@@@@@@@@@@@@@@@@@@@  GAME  @@@@@@@@@@@@@@@@@@@@@@@@@@
  void announceGameResult(char result);




#endif //MENU_H