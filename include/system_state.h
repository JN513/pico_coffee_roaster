#ifndef SYSTEM_STATE_H
#define SYSTEM_STATE_H

#include <stdbool.h>
#include "profiles.h"

typedef enum {
    SYS_START,
    SYS_IDLE,
    SYS_MENU,
    SYS_PREHEAT,
    SYS_ROAST,
    SYS_COOLDOWN,
    SYS_EMERGENCY
} system_mode_t;

typedef struct {

    float bt; // Bean temperature (Grão)
    float et; // Environment temperature (Ar - ambiente)

    int target; // Temperatura alvo do PID

    int profile_id; // Profile atual -1 vazio

    int seconds; // Tempo na torra

    int stop_flag;

    int roast_stage;

    int profile_duration;

    char * profile_name;

    system_mode_t mode;

    RoastType profile_type;

} system_state_t;

extern system_state_t g_state;

#endif