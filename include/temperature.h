#ifndef __TEMPERATURE_H__
#define __TEMPERATURE_H__

/**
 * @brief 
 * 
 */
void init_thermistors();

/**
 * @brief 
 * 
 * @return float 
 */
float read_tempA(); // BT (massa)

/**
 * @brief 
 * 
 * @return float 
 */
float read_tempB(); // ET (ar)

/**
 * @brief 
 * 
 * @param new_value 
 * @param state 
 * @return float 
 */
float filter(float new_value, float *state);

/**
 * @brief 
 * 
 */
void init_resistance_control();

/**
 * @brief Set the power object
 * 
 * @param power 
 */
void set_resistance_power(int power);


#endif // !__TEMPERATURE_H__