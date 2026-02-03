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
float read_tempA();

/**
 * @brief 
 * 
 * @return float 
 */
float read_tempB();

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