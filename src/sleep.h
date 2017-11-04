#ifndef __sleep_h__
#define __sleep_h__

#include <chrono>
#include <thread>

void microSleep(unsigned long microseconds) {
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

#endif
