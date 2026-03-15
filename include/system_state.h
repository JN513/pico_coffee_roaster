#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <stdbool.h>

typedef enum {
    SYS_IDLE,
    SYS_MENU,
    SYS_PREHEAT,
    SYS_ROAST,
    SYS_COOLDOWN,
    SYS_EMERGENCY
} system_mode_t;

typedef struct {

    float bt;
    float et;

    int target;

    int profile_id;

    int seconds;

    system_mode_t mode;

} system_state_t;

extern system_state_t g_state;

#endif