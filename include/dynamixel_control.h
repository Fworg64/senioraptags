#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"

// values are ok if return value is non negative
int16_t init_file (void);       // must be called before API gets used
int16_t set_velocity (float velocity);  // sets the velocity, can be signed
int16_t get_velocity (float*velocity);  // gets the velocity, can be signed, fills ptr
int16_t get_position (float*position);  // gets the position, can be signed
int16_t set_position (float position);  // sets the position, can be signed, 0 at center of travel
int16_t set_servo_speed (float speed);  // sets the unsigned speed, should only be used for position control
int16_t close_file ();                  // closes the dev device, call before exit





#ifdef __cplusplus
}
#endif
