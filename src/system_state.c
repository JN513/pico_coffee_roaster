#include "system_state.h"

system_state_t g_state = {
  .bt = 0.0f,
  .et = 0.0f,
  .target = 0,
  .profile_id = 0,
  .roast_stage = 0,
  .profile_name = 0,
  .mode = SYS_START,
  .profile_type = ROAST_MEDIUM,
  .profile_duration = 0,
  .stop_flag = 0
};