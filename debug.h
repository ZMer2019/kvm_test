#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>

#define TIME_FORMAT                     "%Y-%m-%d %H:%M:%S"

#define _LOG(color, tag, format, ...)                                       \
do{                                                                         \
    time_t now = time(NULL);                                                \
    char timestr[20] = {0};                                                 \
    strftime(timestr, 20, TIME_FORMAT, localtime(&now));                    \
    fprintf(stderr, color "[%s %s %d " tag "]: \e[0m" format ,             \
                timestr, __FUNCTION__, __LINE__, ##__VA_ARGS__);            \
    fflush(stderr);                                                         \
}while(0)
#define LOGD(format, ...) _LOG("\x1b[32m", "DEBUG", format, ##__VA_ARGS__)
#define LOGE(format, ...) _LOG("\x1b[31m", "ERROR", format, ##__VA_ARGS__)
#endif
