/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : messages
 * @created     : Sunday Nov 03, 2019 16:12:00 MST
 */

#ifndef MESSAGES_H

#define MESSAGES_H

#include <cinttypes>

#define BYTE uint8_t
#define MESSAGE_SIZE 4

typedef struct rmt_message_s {
  int16_t lin;
  int16_t ang;
  BYTE buffer[MESSAGE_SIZE];
} rmt_message;

int 
rmt_from_wire(rmt_message* msg, const char* message);

int
rmt_to_wire(rmt_message* msg);

#endif /* end of include guard MESSAGES_H */

