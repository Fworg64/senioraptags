#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/poll.h>
#include <packets.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE	512

char readBuf[MAX_BUFFER_SIZE];
char writeBuf[MAX_BUFFER_SIZE];


#define DEVICE_NAME	"/mnt/bbb/rpmsg_pru30"

#define RPM_PER_COUNT	0.1111f
#define RAD_PER_RPM	(.10467f)
#define RPS_PER_COUNT	(RPM_PER_COUNT*RAD_PER_RPM)

#define DEGREE_PER_RAD   ((3.14159)/180.0f)
#define DEGREE_PER_COUNT	(300.0/1024.0)
#define RAD_PER_CNT	(DEGREE_PER_RAD*DEGREE_PER_COUNT)



struct pollfd pollfds[1];

int16_t init_file (){
	pollfds[0].fd = open (DEVICE_NAME, O_RDWR);
	return pollfds[0].fd;
}




int16_t set_velocity (float velocity){
	struct cmd_packet pack;
	pack.cmd = SET_VELOCITY;
	pack.param = (int16_t)-1.0f* (velocity/RPS_PER_COUNT);
	if (pack.param > 1023){
		pack.param = 1023;
	} else if (pack.param < -1023){
		pack.param = -1023;
	}
	pack.magic = MAGIC_WORD;
	memcpy (writeBuf, &pack, sizeof (struct cmd_packet));
	write (pollfds[0].fd, writeBuf, sizeof (struct cmd_packet));
	return 0;
}

int16_t get_velocity (float *velocity){
	//float velocity=0.0f ;
	struct cmd_packet pack;
	pack.cmd = GET_VELOCITY;
	pack.magic = MAGIC_WORD;
	memcpy (writeBuf, &pack, sizeof (struct cmd_packet));
	write (pollfds[0].fd, writeBuf, sizeof (struct cmd_packet));
	read  (pollfds[0].fd, readBuf, sizeof (struct cmd_packet));
	memcpy (&pack, readBuf, sizeof (struct cmd_packet));
	if (pack.magic == MAGIC_WORD){
		if (pack.cmd == SET_VELOCITY){
			*velocity = -1.0f*RPS_PER_COUNT * pack.param;
		} else {
			return pack.cmd;
		}
	}
	return 0;
}

int16_t set_position (float pos ){
	struct cmd_packet pack;
	pack.cmd = SET_POSITION;
	pack.param= (int16_t) pos/RAD_PER_CNT;
	pack.param += (1023>>1);
	if (pack.param > 1023){
		pack.param = 1023;
	} else if (pack.param < 0){
		pack.param = 0;
	}
	pack.magic = MAGIC_WORD;
	memcpy (writeBuf, &pack, sizeof (struct cmd_packet));
	write (pollfds[0].fd, writeBuf, sizeof (struct cmd_packet));
	return (0);
}



int16_t get_position (float *position){
	//float position = -1.0f;
	struct cmd_packet pack;
	pack.cmd = GET_POSITION;
	pack.magic = MAGIC_WORD;
	memcpy (writeBuf, &pack, sizeof (struct cmd_packet));
	write (pollfds[0].fd, writeBuf, sizeof (struct cmd_packet));
	read (pollfds[0].fd, readBuf, sizeof (struct cmd_packet));
	memcpy (&pack, readBuf, sizeof (struct cmd_packet));
	if (pack.cmd == SET_POSITION){
		if (pack.magic == MAGIC_WORD){
			*position = pack.param * RAD_PER_CNT;
                        *position -= (1023>>1);
		}else {
			return pack.cmd;
		}
	}
	return 0;
}

int16_t close_file (void){
	return (close (pollfds[0].fd));
}

int16_t set_servo_speed (float speed){
	struct cmd_packet pack;
	pack.cmd = SET_SPEED;
	pack.param = (int16_t) -1.0f*(speed/RPS_PER_COUNT);
	if (pack.param > 1023){
		pack.param = 1023;
	} else if (pack.param < 0){
		pack.param = 0;
	}
	pack.magic = MAGIC_WORD;
	memcpy (writeBuf, &pack, sizeof (struct cmd_packet));
	write (pollfds[0].fd, writeBuf, sizeof (struct cmd_packet));
	return 0;
}
