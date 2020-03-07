

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include "serial.h"


typedef struct {
        pthread_mutex_t serial_lock;
        int fd;
} serial_dev_attr;



#define TO_TEENSY_MSG_SIZE (sizeof(float) * 2 + sizeof(uint16_t) + 3)
typedef enum { MINING, DRIVING, DUMPING } robot_state;
typedef struct {
        uint16_t checksum;
        float lin, ang;
        robot_state state;
} to_teensy_msg;

#define FROM_TEENSY_MSG_SIZE (sizeof(uint8_t) + sizeof(uint16_t))
typedef struct {
        uint8_t status_code;
        uint16_t checksum;
} from_teensy_msg;

typedef struct  {
        pthread_mutex_t r_buffer_lock;
        pthread_mutex_t w_buffer_lock;
        uint8_t r_buffer[256];
        uint8_t w_buffer[256];
} teensy_device;


void teensy_init();
void teensy_cleanup();

int new_serial_device(serial_dev_attr* attr, const char* serialport);
int serial_device_cleanup(serial_dev_attr* attr);

typedef enum {TIMEOUT, NOREAD, FAILURE, SUCCESS} serial_status;

serial_status t_send(to_teensy_msg* msg, serial_dev_attr* dev);
serial_status t_recieve(from_teensy_msg* msg, serial_dev_attr* dev);

#ifdef __cplusplus
}
#endif
