#ifndef __UTILS_UNIVERSAL_SLEEP_H_
#define __UTILS_UNIVERSAL_SLEEP_H_

#ifdef _WIN32

#include <windows.h>

#define UNIVERSAL_UTILS__SLEEP(ms) Sleep(ms)

#elif (defined __linux__) || (defined __APPLE__)

#include <unistd.h>

#define UNIVERSAL_UTILS__SLEEP(ms) usleep(ms * 1000)

#endif /* _WIN32 | __linux__ | __APPLE__ */

#endif /* __UTILS_UNIVERSAL_SLEEP_H_ */
