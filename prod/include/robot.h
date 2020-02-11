#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "teensy_msg.h"

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

    // Run once in loop()
    int robot_loop();
    
    /*
     * Utility 
     */
  protected:
    // Converts a "pseudo throttle" to a motor value
    static int convert(float throttle_);

    // Listen to the serial channel and update linear and angular values
    int serial_listen();

    // Update motor throttles
    int callback_velocity();

    // Write to motors (this is where analogWrite is called)
    int motor_write();

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


    float linear;
    float angular;

    cmd_vel vel_wire;

};
