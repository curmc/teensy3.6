#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "teensy_msg.h"

/*
 * PWM Constants
 * From the data sheet
 */
// // Desired resolution
// #define PWM_RESOLUTION          12
// #define MAX_PWM                 ((1 << PWM_RES) - 1)
//
// // Period of a pwm cycle (ms)
// #define PWM_PERIOD              4.0
//
// // Period of pwm high time for zero (ms)
// #define PWM_ZERO_TIME           1.5
//
// // Duty cycle for zero input
// #define PWM_DUTY_ZERO           ((PWM_ZERO_TIME) / (PWM_PERIOD))
// // Input to frequency for zero
// #define PWM_ZERO                (PWM_DUTY_ZERO * MAX_PWM)
//


// Scalar value to multiply given speed
#define PWM_SCALAR              5.12
// Zero value for pwm
#define PWM_ZERO                1535

// Resolution bits
#define PWM_RES                 12

// The frequency to drive the pwm frequency
#define PWM_INIT                250

// Min throttle is just -MaxThrottle
#define MAX_THROTTLE            100


#define LF_PIN                  16
#define LB_PIN                  5
#define RF_PIN                  4
#define RB_PIN                  7

#define RMT_MESSAGE_SIZE        11

#define MSG_STATUS_PIN          33


/*
 * A robot handles motor writing and serial
 * listening
 */
class Robot {
  public:

    // Default Constructor
    Robot(LSM9DS1* imu_, uint32_t timing);

    // Default Destructor
    ~Robot();

    // Run once in setup()
    int robot_setup();


    void calibrate_sensors();

    /*
     * Utility 
     */
    // Converts a "pseudo throttle" to a motor value
    static int convert(float throttle_);

    // Listen to the serial channel and update linear and angular values
    int serial_listen();

    // Update motor throttles
    int callback_velocity();

    // Write to motors (this is where analogWrite is called)
    int motor_write();

    void update_sensors();

  private:


    // Motor Pins
    struct {
      int32_t lf;
      int32_t lb;
      int32_t rf;
      int32_t rb;
    } motors;

    // Motor throttles
    struct { 
      int32_t lf;
      int32_t lb;
      int32_t rf;
      int32_t rb;
    } throttle;

    // In and out wire messages and number 
    // of imu samples to obtain on a read
    struct {
      teensy_msg imu_wire;
      cmd_vel vel_wire;
      uint32_t samples = 5;
    } wire;

    // Net throttle (copied from cmd_vel)
    float linear;
    float angular;

    // An actual imu
    LSM9DS1* imu;

    // Time in milliseconds for the wire
    uint64_t time_milli;
};
