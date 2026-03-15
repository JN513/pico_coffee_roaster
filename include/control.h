#ifndef __CONTROL_H__
#define __CONTROL_H__


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
};

static ProfilePoint profile2[] = { // torra média curta
    {0,   30},
    {180, 150}, // primeira fase: secagem - 3 min
    {420, 190}, // segunda fase: maillard - 4 min
    {600, 205}, // terceira fase: desenvolvimento - 3 min
    {720, 210}, // quarta fase: finalização - 2 min
};

static ProfilePoint profile3[] = { // torra media longa
    {0,   30},
    {180, 150}, // Secagem (3 min)
    {420, 195}, // Maillard (mais energia para preparar o crack)
    {600, 215}, // Desenvolvimento (pós Primeiro Crack)
    {780, 222}, // Finalização: Alvo de torra média
};

#define PROFILE_SIZE (sizeof(profile)/sizeof(ProfilePoint))
#define PROFILE_SIZE2 (sizeof(profile2)/sizeof(ProfilePoint))
#define PROFILE_SIZE3 (sizeof(profile3)/sizeof(ProfilePoint))

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
float get_bt_target(int seconds);

/**
 * @brief Get the current stage object
 * 
 * @param seconds 
 * @return int 
 */
int get_current_stage(int seconds);

/**
 * @brief 
 * 
 */
void emergency_shutdown();

/**
 * @brief 
 * 
 * @param stage 
 */
void print_stage(int stage);

/**
 * @brief Get the finish time object
 * 
 * @return int 
 */
int get_finish_time();

#endif // !__CONTROL_H__
