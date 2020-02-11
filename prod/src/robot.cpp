/**
 * @author                        : theo (theo.j.lincke@gmail.com)
 * @file                                : robot
 * @created                 : Thursday Dec 19, 2019 21:11:40 MST
 */

#include "robot.h"

static teensy_msg outgoing;


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

        memset(&vel_wire, 0, sizeof(imu_message));
}

Robot::~Robot(){}

int Robot::robot_setup() {
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

        // start the timer
        Serial.begin(9600);
        return 1;
}


int Robot::robot_loop() {

        // Then read an update and publish
        // current imu state to the wire
        int ret = serial_listen();

        // Set throttles
        callback_velocity();

        // Write to the motors 
        // Note that this also updates time_milli
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
                size_t bytes = Serial.readBytes((char*)vel_wire.buffer, CMD_VEL_BUFFER_SIZE);
                if(bytes == CMD_VEL_BUFFER_SIZE) {

                        // Parse cmd_vel 
                        parse_cmd_vel_message(&vel_wire, vel_wire.buffer);

                        // Update local lin and ang
                        linear = vel_wire.linear_v;
                        angular = vel_wire.angular_v;

                        // Update the teensy message
                        outgoing.msg = imu_wire;
                        update_teensy_imu(&outgoing);

                        // Write to the wire
                        bytes = Serial.write(outgoing.buffer, TEENSY_BUFFER_SIZE);
                        if(bytes == TEENSY_BUFFER_SIZE){
                                /*
                                 * Since everything has gone to plan
                                 * we can now call this a complete period, so
                                 * we reset the imu reading and
                                 * get the time period passed in seconds
                                 */
                                uint64_t prev_time = time_milli;
                                imu_wire.dt = double(millis() - prev_time) / 1000.0;
                                // We don't update time_milli until we write to the motors through

                                // Sets values to zero
                                reset_imu_reading(&imu_covariance.imu_cov);
                        }
                }
        } 
        return ret;
}


void Robot::read_imu() {
        uint64_t start = millis();
        while(millis() - start < imu_covariance.timing) {

                // Update imu
                if(imu->gyroAvailable())
                        imu->readGyro();
                if(imu->accelAvailable())
                        imu->readAccel();

                // Linear readings
                float lin[3] = {imu->calcAccel(imu->ax), imu->calcAccel(imu->ay), imu->calcAccel(imu->az)};

                // Angular readings
                float ang[3] = {imu->calcGyro(imu->gx), imu->calcGyro(imu->gy), imu->calcGyro(imu->gz)};

                // Covariance update
                update_reading(&imu_covariance.imu_cov, lin, ang);
        }

        // Copy the data into imu_wire
        for(size_t i = 0; i < 3; ++i) {
                imu_wire.linear_acceleration[i] = imu_covariance.imu_cov.linear[i].mean;
                imu_wire.angular_velocity[i] = imu_covariance.imu_cov.angular[i].mean;
        }
        for(size_t i = 0; i < 9; ++i) {
                for(size_t j = 0; j < 3; ++j) {
                        imu_wire.linear_acceleration_covariance[i * 3 + j] = imu_covariance.imu_cov.L_P[i][j].cov;
                        imu_wire.angular_velocity[i * 3 + j] = imu_covariance.imu_cov.A_P[i][j].cov;
                }
        }
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
        analogWrite(motors.rf, throttle.rf);
        analogWrite(motors.rb, throttle.rb);
        analogWrite(motors.lf, throttle.lf);
        analogWrite(motors.lb, throttle.lb);

        // update time now because motors ideally have been set
        time_milli = millis();
        return 1;
}
