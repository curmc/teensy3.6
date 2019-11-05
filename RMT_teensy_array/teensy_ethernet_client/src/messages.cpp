/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : messages
 * @created     : Sunday Nov 03, 2019 16:16:59 MST
 */

extern "C" {
#include "messages.h"

int
rmt_from_wire(rmt_message* msg, const char* message) {
  if(message && msg) {
    int16_t* lin = (int16_t*)message;
    int16_t* ang = (int16_t*)(message + 2);
    msg->lin = *lin;
    msg->ang = *ang;
    return 1;
  }
  return 0;
}

int
rmt_to_wire(rmt_message* msg) {
  if(msg) {
    int16_t* lin = (int16_t*)msg->buffer;
    int16_t* ang = (int16_t*)(msg->buffer + 2);
    *lin = msg->lin;
    *ang = msg->ang;
    return 1;
  }
  return 0;
}
}
