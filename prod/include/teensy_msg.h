/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : teensy_msg
 * @created     : Thursday Jan 09, 2020 12:23:07 MST
 */

#ifndef TEENSY_MSG_H
#define TEENSY_MSG_H

#include <inttypes.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TEENSY_BUFFER_SIZE 4
#define CMD_VEL_BUFFER_SIZE 13

typedef struct {
        uint16_t checksum;
        uint16_t status;
        uint8_t buffer[TEENSY_BUFFER_SIZE];
} teensy_msg;

typedef struct {
        uint16_t checksum;

        uint8_t driving;
        uint8_t mining;
        uint8_t dumping;

        float linear_v;
        float angular_v;
        uint8_t buffer[CMD_VEL_BUFFER_SIZE];
} cmd_vel;


void parse_cmd_vel_message(cmd_vel* msg, const uint8_t buffer[CMD_VEL_BUFFER_SIZE]);
void update_teensy_msg(teensy_msg* msg);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard TEENSY_MSG_H */
