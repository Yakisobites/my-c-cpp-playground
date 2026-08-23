#include "sleep.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
void sleep_ms(int milliseconds) { Sleep(milliseconds); }
#else
#include <unistd.h>
void sleep_ms(int milliseconds) {
  if (milliseconds < 0) {
    return; // Handle negative input gracefully
  }
  usleep((useconds_t)milliseconds *
         1000u); // Convert milliseconds to microseconds
}
#endif