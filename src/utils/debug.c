#include "debug.h"
#include <stdarg.h>
#include <stdio.h>

static int current_debug_level = DEBUG_LEVEL_NONE;

void set_debug_level(int debug_level)
{
    current_debug_level = debug_level;
}
int dprintf(int debug_level, const char *fmt, ...)
{
    if (debug_level > current_debug_level)
    {
        return 0;
    }
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[DBG%d] ", debug_level);
    int ret = vfprintf(stderr, fmt, args);
    va_end(args);
    return ret;
}
