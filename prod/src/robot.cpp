/**
 * @author                        : theo (theo.j.lincke@gmail.com)
 * @file                                : robot
 * @created                 : Thursday Dec 19, 2019 21:11:40 MST
 */

#include "robot.h"


Robot::Robot(LSM9DS1* imu_, uint32_t timming)
{
        // Set motor pins
        motors.lf = LF_PIN;
        motors.lb = LB_PIN;
        motors.rf = RF_PIN;
        motors.rb = RB_PIN;

        // Set throttle to zero
        memset(&throttle, 0, sizeof(throttle));

        // Initialize the imu reading for covariance
        linear = 0.0;
        angular = 0.0;

        memset(&vel_wire, 0, sizeof(cmd_vel));
        memset(&outgoing, 0, sizeof(teensy_msg));
}

Robot::~Robot(){}

int Robot::robot_setup() {

#ifndef DEBUG
        // Write zero to all motors
        analogWriteFrequency(motors.lf, PWM_INIT);
        analogWrite(motors.lf, PWM_ZERO);

        analogWriteFrequency(motors.lb, PWM_INIT);
        analogWrite(motors.lb, PWM_ZERO);

        analogWriteFrequency(motors.rf, PWM_INIT);
        analogWrite(motors.rf, PWM_ZERO);

        analogWriteFrequency(motors.rb, PWM_INIT);
        analogWrite(motors.rb, PWM_ZERO);

        analogWriteResolution(PWM_RES);
#endif

        // start the timer
        Serial.begin(9600);
        return 1;
}


int Robot::robot_loop() {

        int ret = serial_listen();
        callback_velocity();
        motor_write();

        return ret;
}

int Robot::convert(float throttle_) {
        if(throttle_ > MAX_THROTTLE)
                throttle_ = (float)MAX_THROTTLE;
        else if(throttle_ < -MAX_THROTTLE)
                throttle_ = (float)-MAX_THROTTLE;

        // round the command 
        return PWM_ZERO + ((int)throttle_ * PWM_SCALAR);
}

int Robot::serial_listen() {
        // serial available return
        int ret;

        if((ret = Serial.available())) {

                // Read cmd_vel input from the wire
                size_t bytes = Serial.readBytes((char*)vel_wire.buffer, 
                                                 CMD_VEL_BUFFER_SIZE);


                if(bytes == CMD_VEL_BUFFER_SIZE) {

                        // Parse cmd_vel 
                        parse_cmd_vel_message(&vel_wire, vel_wire.buffer);

                        // Update local lin and ang
                        linear = vel_wire.linear_v;
                        angular = vel_wire.angular_v;

                        // Update the teensy message
                        outgoing.checksum = vel_wire.checksum;
                        outgoing.status = 1;
                        update_teensy_msg(&outgoing);

                        // Write to the wire
                        bytes = Serial.write(outgoing.buffer, TEENSY_BUFFER_SIZE);
                } else {
                        outgoing.checksum = 0;
                        outgoing.status = 0;
                        bytes = Serial.write(outgoing.buffer, TEENSY_BUFFER_SIZE);
                }
        } 
        return ret;
}


// update throttles accordingly, this
// is where a pd controller would go
int Robot::callback_velocity() {
        int l = convert(-(linear + angular));
        int r = convert(linear - angular); 
        throttle.lf = l;
        throttle.lb = l;
        throttle.rf = r;
        throttle.rb = r;
        return 1;
}

int Robot::motor_write() {
#ifndef DEBUG
        analogWrite(motors.rf, throttle.rf);
        analogWrite(motors.rb, throttle.rb);
        analogWrite(motors.lf, throttle.lf);
        analogWrite(motors.lb, throttle.lb);
#endif
        return 1;
}
