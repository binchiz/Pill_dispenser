#ifndef DEBUG_H
#define DEBUG_H

#define ENABLE_DPRINT 1

typedef enum {
    DEBUG_LEVEL_NONE = 0,
    DEBUG_LEVEL_ERROR = 1,
    DEBUG_LEVEL_WARN = 2,
    DEBUG_LEVEL_INFO = 3,
    DEBUG_LEVEL_DEBUG = 4,
} DebugLevel;

void set_debug_level(int debug_level);
int dprintf(int debug_level, const char *fmt, ...);

#endif // DEBUG_H
