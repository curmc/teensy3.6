#include <Arduino.h>


#define FROM_HOST_MSG_SIZE (sizeof(float) * 2 + sizeof(uint16_t) + 3)
typedef enum {MINING, DRIVING, DUMPING} robot_state;
typedef struct {
        uint16_t checksum;
        float lin, ang;
        robot_state state;
} from_host_msg;

#define TO_HOST_MSG_SIZE (sizeof(uint8_t) + sizeof(uint16_t))
typedef struct {
        uint8_t status_code;
        uint16_t checksum;
} to_host_msg;

to_host_msg to_host;
from_host_msg from_host;

void to_host_serialize(to_host_msg* msg, char buffer[TO_HOST_MSG_SIZE]);
void from_host_deserialize(from_host_msg* msg, char buffer[FROM_HOST_MSG_SIZE]);

char buffer[256];

void setup() {
    Serial.begin(9600);
}

void loop() {

        // if(Serial.available()) {
        //         int i = 0;
        //         while(Serial.available()) {
        //                 buffer[i] = Serial.read();
        //         }
        //         Serial.write("Hello", 5);
        //
        //         // for(uint32_t i = 0; i < FROM_HOST_MSG_SIZE; ++i) {
        //         //         buffer[i] = Serial.read();
        //         // }
        //         //
        //         // from_host_deserialize(&from_host, buffer);
        //         // to_host.status_code = (int)from_host.lin;
        //         // to_host.checksum = (int)from_host.ang;
        //         // to_host_serialize(&to_host, buffer);
        //         // Serial.write(buffer, TO_HOST_MSG_SIZE);
        // }
        Serial.write("Hello", 5);
        delay(1000);

}

void to_host_serialize(to_host_msg* msg, char buffer[TO_HOST_MSG_SIZE]) {
        uint16_t* handle = (uint16_t*)buffer;
        *handle++ = msg->checksum;

        uint8_t* status = (uint8_t*)handle;
        *status++ = msg->status_code;

}
void from_host_deserialize(from_host_msg* msg, char buffer[FROM_HOST_MSG_SIZE]){
        uint16_t* checksum = (uint16_t*)buffer;
        msg->checksum = *checksum++;

        uint8_t* state = (uint8_t*)checksum;
        switch(*state) {
                case 1:
                        msg->state = DRIVING;
                        break;
                case 2:
                        msg->state = DUMPING;
                        break;
                case 3:
                        msg->state = MINING;
                        break;

                default:
                        msg->state = DRIVING;
                        break;
        }
        state++;
        float* vals = (float*)state;
        msg->lin = *vals++;
        msg->ang = *vals++;
}
