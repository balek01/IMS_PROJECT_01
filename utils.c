#include "utils.h"
#include <stdarg.h>

void debug(int level, const char *format, ...)
{
    if (DEBUG_LEVEL >= level)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}