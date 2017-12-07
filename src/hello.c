#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <unistd.h>
//#include <fcntl.h>
#include <dynamixel_control.h>

int main(int argc, char **argv)
{
	int ret;
	//unsigned int ctr = 0;
	//float vel = 2.0f;
	ret = init_file ();
	if (ret >0 ){
		printf ("opened file\n");
		//set_servo_speed (100);
/*		for (ctr = 0; ctr < 30; ctr++){
			printf ("made it to for loop");
			set_velocity (vel);
			printf ("set velocity");
			usleep (1000000);
			vel = -1.0f*vel;
		}
*/ set_position (0.0f);		//printf ("position was %f\n", pos);
		close_file ();

	} else {
		printf ("couldn't open\n");
	}
		return 0;
}


#ifdef __cplusplus
}
#endif
