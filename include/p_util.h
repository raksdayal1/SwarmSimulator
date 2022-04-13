/*
 * Created by Rakshit Allamraju
 * Date: Feb. 25 2022
 *
 * Modified by Rakshit Allamraju
 * Date: April 5 2022
 */

#ifndef P_UTIL_H
#define P_UTIL_H

#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

void thread_sleep(double seconds){

    #ifdef _WIN32
        Sleep(seconds*1E3);
    #else
        usleep(seconds*1E6);
    #endif


};

#endif // P_UTIL_H
