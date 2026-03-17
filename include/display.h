#ifndef __DISPLAY_H__

#include "profiles.h"

/**
 * @brief 
 * 
 */
void init_display();

/**
 * @brief 
 * 
 * @param tempA 
 * @param tempB 
 * @param stage 
 * @param profile_name 
 */
void update_display_info(float tempA, float tempB, int stage, char * profile_name);

/**
 * @brief 
 * 
 * @param line1 
 * @param line2 
 */
void print_info(char *line1, char *line2);

/**
 * @brief 
 * 
 * @param profile_id 
 * @param profile_name 
 * @param profile_duration 
 * @param profile_type 
 */
void print_menu(int profile_id, char * profile_name, int profile_duration, RoastType profile_type);

#endif // !__DISPLAY_H__