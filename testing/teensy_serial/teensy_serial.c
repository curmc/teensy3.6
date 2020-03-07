#include <stdlib.h>
#include "teensy_serial.h"

static uint16_t generate_checksum() { return rand() % 100000; }

static teensy_device device;
static int to_teensy_serialize(to_teensy_msg* msg, teensy_device* dev);
static int from_teensy_deserialize(from_teensy_msg* msg, teensy_device* dev);


void teensy_init() {
        pthread_mutex_init(&device.r_buffer_lock, NULL);
        pthread_mutex_init(&device.r_buffer_lock, NULL);
}

void teensy_cleanup() {
        pthread_mutex_destroy(&device.r_buffer_lock);
        pthread_mutex_destroy(&device.r_buffer_lock);
}

int new_serial_device(serial_dev_attr* attr, const char* serialport) {
        pthread_mutex_init(&attr->serial_lock, NULL);
        attr->fd = serialport_init(serialport, 9600);
        return attr->fd;
}

int serial_device_cleanup(serial_dev_attr* attr) {
        return serialport_close(attr->fd); 
}

serial_status t_send(to_teensy_msg* msg, serial_dev_attr* dev) {

        // Serialize the message
        if(!to_teensy_serialize(msg, &device)) {
                return FAILURE;
        }
        
        // Write data
        pthread_mutex_lock(&dev->serial_lock);
        int ret = serialport_write(dev->fd, device.w_buffer, TO_TEENSY_MSG_SIZE);
        pthread_mutex_unlock(&dev->serial_lock);

        // serialport write returns -1 on failure else 0
        if(ret < 0)
                return FAILURE;

        return SUCCESS;
}

serial_status t_recieve(from_teensy_msg* msg, serial_dev_attr* dev) {

        // Read from the device
        pthread_mutex_lock(&dev->serial_lock);
        int ret = serialport_read(dev->fd, device.r_buffer, TO_TEENSY_MSG_SIZE, 100);
        pthread_mutex_unlock(&dev->serial_lock);


        // Serialport read returns 0 on success, else -1 or -2
        if(ret == -2)
                return TIMEOUT;

        if(ret == -1)
                return NOREAD;

        if(ret == 0){
                if(!from_teensy_deserialize(msg, &device)) {
                        return FAILURE;
                }
                return SUCCESS;
        }

        return FAILURE;
}

static int to_teensy_serialize(to_teensy_msg* msg, teensy_device* dev) {
        pthread_mutex_lock(&dev->w_buffer_lock);

        uint16_t* checksum = (uint16_t*)dev->w_buffer;
        *checksum++ = msg->checksum;

        uint8_t* state = (uint8_t*)checksum;
        switch(msg->state) {
                case DRIVING:
                        *state++ = 1;
                        break;
                case DUMPING:
                        *state++ = 2;
                        break;
                case MINING:
                        *state++ = 3;
                        break;
                default:
                        *state++ = 0;
                        break;
        }

        float* vals = (float*)state;
        *vals++ = msg->lin;
        *vals++ = msg->ang;

        pthread_mutex_unlock(&dev->w_buffer_lock);
        return 1;
}


static int from_teensy_deserialize(from_teensy_msg* msg, teensy_device* dev) {
        
        pthread_mutex_lock(&dev->r_buffer_lock);

        uint16_t* handle = (uint16_t*)dev->r_buffer;
        msg->checksum = *handle++;

        uint8_t* status = (uint8_t*)handle;
        msg->status_code = *status++;
        return 1; 
}




































