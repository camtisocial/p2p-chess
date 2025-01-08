#ifndef GLOBALS_H
#define GLOBALS_H

#include <atomic>

extern std::atomic<bool> keepBroadcasting;
extern std::atomic<bool> running;
extern std::atomic<bool> reprint;

#endif //GLOBALS_H
