#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "profiles.h"

/**
 * @brief 
 * 
 * @param target 
 * @param temperature 
 */
void control_temperature(int target, float temperature);


/**
 * @brief Get the bt target object
 * 
 * @param seconds 
 * @return float 
 */
float get_bt_target(int seconds, ProfilePoint * profile_array, 
    int profile_size);

/**
 * @brief Get the current stage object
 * 
 * @param seconds 
 * @return int 
 */
int get_current_stage(int seconds, ProfilePoint * profile_array, 
    int profile_size);

/**
 * @brief 
 * 
 */
void emergency_shutdown();


#endif // !__CONTROL_H__
