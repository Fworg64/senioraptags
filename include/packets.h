#ifdef __cplusplus
extern "C"{
#endif

#ifndef __PACKETS_H__
#define __PACKETS_H__


#include "stdint.h"

#define UART_TIMEOUT_ERR	-3
#define MALFORMED_COMMAND       -2
#define IMPROPER_RANGE          -1
#define NO_COMMAND              0
#define SET_VELOCITY            1
#define SET_POSITION           2
#define GET_VELOCITY            3
#define GET_POSITION            4
#define SET_SPEED		5

#define MAGIC_WORD              0xB00BEEE5

struct cmd_packet {
    int16_t cmd;
    int16_t param;
    uint32_t magic;
};

//uint16_t create_cmd (struct cmd_packet *cmd_str, uint8_t cmd, uint8_t cmd_dir)


//int8_t  get_command (char * buff, int16_t *param, uint16_t len);

//uint16_t create_packet (char * buffer, int16_t param, char cmd_type, char cmd_dir);



#endif
#ifdef __cplusplus
}
#endif
