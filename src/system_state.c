#include "system_state.h"

system_state_t g_state = {
  .bt = 0.0f,
  .et = 0.0f,
  .target = 0,
  .profile_id = -1,
  .roast_stage = 0,
  .profile_name = 0,
  .mode = SYS_START
};