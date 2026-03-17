#ifndef __FC_DETECTOR_H__
#define __FC_DETECTOR_H__

/**
 * @brief 
 * 
 */
void fc_fft_init();

/**
 * @brief 
 * 
 */
void sample_audio();

/**
 * @brief 
 * 
 */
void run_fft();

/**
 * @brief 
 * 
 * @return float 
 */
float crack_energy();

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool detect_fc(float);

#endif // !__FC_DETECTOR_H__