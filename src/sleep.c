#include "sleep.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
void sleep_ms(int milliseconds) { Sleep(milliseconds); }
#else
#include <unistd.h>
void sleep_ms(int milliseconds) { usleep(milliseconds * 1000); }
#endif