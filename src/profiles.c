#include "profiles.h"
#include <stdio.h>

int get_profile_finish_time(int profile_id) {
    if(profile_id >= PROFILE_COUNT) return 0;

    return profiles[profile_id].total_time;
}

char * get_profile_name(int profile_id) {
    if(profile_id >= PROFILE_COUNT) return 0;

    return profiles[profile_id].name;
}

ProfilePoint * get_profile_pointer(int profile_id) {
    if(profile_id >= PROFILE_COUNT) return 0;

    return profiles[profile_id].points;
}

int get_profile_size(int profile_id) {
    if(profile_id >= PROFILE_COUNT) return 0;

    return profiles[profile_id].size;
}

RoastType get_profile_type(int profile_id) {
    if(profile_id >= PROFILE_COUNT) return 0;

    return profiles[profile_id].type;
}

void print_stage(int stage) {
    switch(stage) {
        case 0: printf("- Fase: Secagem\n"); break;
        case 1: printf("- Fase: Maillard\n"); break;
        case 2: printf("- Fase: Desenvolvimento\n"); break;
        case 3: printf("- Fase: Finalização\n"); break;
        case 4: printf("- Fase: Refrigeração\n"); break;
    }
}