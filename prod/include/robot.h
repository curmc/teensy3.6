#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "PID.h"

// Scalar value to multiply given speed
#define PWM_SCALAR      5.12
// Zero value for pwm
#define PWM_ZERO        1535
// Resolution bits
#define PWM_RES         12
// Initial frequency
#define PWM_INIT        250

// Min throttle is just -MaxThrottle
#define MAX_THROTTLE    100


#define LF_PIN          16
#define LB_PIN          5
#define RF_PIN          4
#define RB_PIN          7

#define MSG_SIZE        11

#define MSG_STATUS_PIN  33


/*
 * A robot handles motor writing and serial
 * listening
 */
class Robot {
  public:

    // Default Constructor
    Robot();

    // Default Destructor
    ~Robot();

    // Run once in setup()
    int robot_setup();

    // Run once in loop()
    int robot_loop();
    


    /*
     * Utility 
     */
  protected:
    // Converts a "pseudo throttle" to a motor value
    static int convert(int8_t throttle_);

    // Listen to the serial channel and update linear and angular values
    int serial_listen();

    // Update motor throttles
    int callback_velocity();

    // Write to motors (this is where analogWrite is called)
    int motor_write();

    // Update sensed position from sensors
    int sensor_update();


  private:
    // Motor Pins
    struct motors_s {
      int lf;
      int lb;
      int rf;
      int rb;
    } motors;

    // Motor pSeudo throttles
    struct throttle_s { 
      int8_t lf;
      int8_t lb;
      int8_t rf;
      int8_t rb;
    } throttle;

    // Buffer used to recieve and send serial messages
    uint8_t buffer[11];

    struct speed_estimator {
      float previous_sensed_angular;
      float previous_sensed_speed;

      float curr_sensed_speed;
      float curr_sensed_angular;

      float curr_sensed_accel;

      uint64_t previous_time;
      uint64_t current_time;
    } speed;

    // Motor speeds
    int8_t linear;
    int8_t angular;

    PIDController angular_pid_c;
    
    LSM9DS1 imu;
    bool recieved;
};
