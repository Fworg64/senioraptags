#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"


int16_t init_file (void);
int16_t set_velocity (float velocity);
int16_t get_velocity (float*velocity);
int16_t get_position (float*position);
int16_t set_position (float position);
int16_t set_servo_speed (float speed);
int16_t close_file ();





#ifdef __cplusplus
}
#endif
