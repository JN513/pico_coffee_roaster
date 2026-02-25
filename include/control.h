#ifndef __CONTROL_H__
#define __CONTROL_H__

typedef struct {
    float kp;
    float ki;
    float kd;

    float integral;
    float last_error;

    float out_min;
    float out_max;
} PID;

typedef struct {
    int time;      // segundos
    float bt;      // alvo massa
} ProfilePoint;

static ProfilePoint profile[] = {
    {0,   30},
    {120, 150},
    {300, 190},
    {420, 205},
    {540, 210}
};

#define PROFILE_SIZE (sizeof(profile)/sizeof(ProfilePoint))

/**
 * @brief 
 * 
 * @param pid 
 * @param setpoint 
 * @param measured 
 * @param dt 
 * @return float 
 */
float pid_compute(PID *pid, float setpoint, float measured, float dt);


/**
 * @brief Get the bt target object
 * 
 * @param seconds 
 * @return float 
 */
float get_bt_target(int seconds);


/**
 * @brief 
 * 
 * @param seconds 
 * @return int 
 */
int fan_base_by_phase(int seconds);

#endif // !__CONTROL_H__
