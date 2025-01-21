#ifndef MENU_H
#define MENU_H

#include <cstdlib>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include <queue>
#include "globals.h"
#include "board.h"
#include "keyPress.h"

using boost::asio::ip::udp;


//@@@@@@@@@@@@@@@@@@@@@@  UTILITIES @@@@@@@@@@@@@@@@@@@@@@@@@@
  int getTerminalWidth();
  std::string centerChar(char character, int width);
  std::string centerText(std::string text, int width);
  KeyPress getKeyPressNonBlocking(std::string& u_input);
  KeyPress getKeyPress();
  void clearLastLine();
  void setRawMode(bool enable);
  std::string kbhit();
//@@@@@@@@@@@@@@@@@@@@@@  MENU  @@@@@@@@@@@@@@@@@@@@@@@@@@
  std::string setPeerIP();
  int setPeerPort();
  int setLocalPort();
  void displayMenu(std::vector<std::string> items, int index);
  void setLocalColor(udp::socket& socket, udp::endpoint& peer_endpoint, bool& localColor);
//@@@@@@@@@@@@@@@@@@@@@@  GAME  @@@@@@@@@@@@@@@@@@@@@@@@@@
  void announceGameResult(char result);




#endif //MENU_H