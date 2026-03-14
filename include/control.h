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
    int bt;      // alvo massa
} ProfilePoint;

static ProfilePoint profile[] = {
    {0,   30},
    {120, 150}, // primeira fase: secagem - 2 min
    {300, 190}, // segunda fase: maillard - 3 min
    {420, 205}, // terceira fase: desenvolvimento - 2 min
    {540, 210},  // quarta fase: finalização - 2 min
    {720, 0}  // quinta fase: refrigeração - 3 min
};

static ProfilePoint profile2[] = { // torra média curta
    {0,   30},
    {180, 150}, // primeira fase: secagem - 3 min
    {420, 190}, // segunda fase: maillard - 4 min
    {600, 205}, // terceira fase: desenvolvimento - 3 min
    {720, 210},  // quarta fase: finalização - 2 min
    {900, 0}  // quinta fase: refrigeração - 3 min
};

static ProfilePoint profile3[] = { // torra media longa
    {0,   30},
    {180, 150}, // Secagem (3 min)
    {420, 195}, // Maillard (mais energia para preparar o crack)
    {600, 215}, // Desenvolvimento (pós Primeiro Crack)
    {780, 222}, // Finalização: Alvo de torra média
    {960, 0}    // Resfriamento
};

#define PROFILE_SIZE (sizeof(profile)/sizeof(ProfilePoint))
#define PROFILE_SIZE2 (sizeof(profile2)/sizeof(ProfilePoint))
#define PROFILE_SIZE3 (sizeof(profile3)/sizeof(ProfilePoint))

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

int get_stage(int seconds);


/**
 * @brief 
 * 
 * @param seconds 
 * @return int 
 */
int fan_base_by_phase(int seconds);

#endif // !__CONTROL_H__
