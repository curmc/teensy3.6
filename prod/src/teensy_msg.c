/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : teensy_msg
 * @created     : Thursday Jan 09, 2020 12:30:07 MST
 */

#include "teensy_msg.h"

void parse_cmd_vel_message(cmd_vel* msg, const uint8_t buffer[CMD_VEL_BUFFER_SIZE]) {
        uint16_t* checksum = (uint16_t*)buffer;
        msg->checksum = *checksum++;

        uint8_t* state = (uint8_t *)checksum;

        msg->driving = *state++;
        msg->mining = *state++;
        msg->dumping = *state++;
        
        float* handle = (float *)state;
        msg->linear_v = *handle++;
        msg->angular_v = *handle;
}

void update_teensy_msg(teensy_msg* msg){ 
        uint16_t* handle = (uint16_t *)msg->buffer;
        *handle++ = msg->checksum;
        *handle++ = msg->status;
}
