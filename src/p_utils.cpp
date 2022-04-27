#include "p_util.h"

void thread_sleep(double seconds){

    #ifdef _WIN32
        Sleep(seconds*1E3);
    #else
        usleep(seconds*1E6);
    #endif
};
