/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : robot
 * @created     : Thursday Dec 19, 2019 21:11:40 MST
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

  memset(&wire.imu_wire, 0, sizeof(teensy_msg));
  memset(&wire.vel_wire, 0, sizeof(cmd_vel));
  wire.samples = 5;

  linear = 0.0;
  angular = 0.0;

  // Assuming the imu is already calibrated
  if(imu_)
    imu = imu_;

  time_milli = 0L;
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
  time_milli = millis();
  
  return 1;
}

static float read_sensor(int n, LSM9DS1* imu) {
  switch(n) {
    case(0):
      imu->readGyro();
      return imu->calcGyro(imu->gx);
    case(1):
      imu->readGyro();
      return imu->calcGyro(imu->gy);
    case(2):
      imu->readGyro();
      return imu->calcGyro(imu->gz);
    case(3):
      imu->readAccel();
      return 9.8 * imu->calcAccel(imu->ax);
    case(4):
      imu->readAccel();
      return 9.8 * imu->calcAccel(imu->ay);
    case(5):
      imu->readAccel();
      return 9.8 * imu->calcAccel(imu->az);
    default:
      return -1;
  }
  return -1;
}

void Robot::calibrate_sensors() {
  imu->calibrate(false);
  // Copying from the library
  for(size_t i = 0; i < 3; ++i) {
    wire.imu_wire.msg.linear_acceleration_bias[i] = imu->aBias[i];
    wire.imu_wire.msg.angular_velocity_bias[i] = imu->gBias[i];
  }
}

void Robot::update_sensors() {

  float l_readings[wire.samples];
  float a_readings[wire.samples];

  for(int i =0; i < 3; ++i) {
    
    for(uint32_t x = 0; x < wire.samples; ++x){
      imu->readGyro();
      imu->readAccel();
      l_readings[x] = read_sensor(i + 3, imu);
      a_readings[x] = read_sensor(i, imu);
    }

    // Max and min indexes
    size_t l_min = 0;
    size_t l_max = 0;
    size_t a_min = 0;
    size_t a_max = 0;

    for(uint32_t x = 1; x < wire.samples; ++x){
      if(l_readings[x] > l_readings[l_max])
        l_max = x;
      if(l_readings[x] < l_readings[l_min])
        l_min = x;
      if(a_readings[x] > a_readings[a_max])
        a_max = x;
      if(a_readings[x] < a_readings[a_min])
        a_min = x;
    }
    l_readings[l_max] = 0.0;
    l_readings[l_min] = 0.0;
    a_readings[a_max] = 0.0;
    a_readings[a_min] = 0.0;

    float l = 0.0;
    float a = 0.0;
    for(uint32_t x = 0; x < wire.samples; ++x) {
      l += l_readings[x];
      a += a_readings[x];
    }

    wire.imu_wire.msg.linear_acceleration[i] = (l / (wire.samples - 2)) - wire.imu_wire.msg.linear_acceleration_bias[i];
    wire.imu_wire.msg.angular_velocity[i] = (a / (wire.samples - 2)) - wire.imu_wire.msg.angular_velocity_bias[i];
  }
  // float ax = wire.imu_wire.msg.linear_acceleration[0];
  // float ay = wire.imu_wire.msg.linear_acceleration[1];
  // float az = wire.imu_wire.msg.linear_acceleration[2];

  // float roll = atan2(-az, -ax);
  // float pitch = atan2(-ay, -(sqrt(ax * ax + az * az)));
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

    digitalWrite(33, HIGH);
    // Read cmd_vel input from the wire
    Serial.readBytes((char*)wire.vel_wire.buffer, CMD_VEL_BUFFER_SIZE);
    // Parse cmd_vel 
    parse_cmd_vel_message(&wire.vel_wire, wire.vel_wire.buffer);

    // Update local lin and ang
    linear = wire.vel_wire.linear_v;
    angular = wire.vel_wire.angular_v;

    // Update the teensy message
    update_teensy_imu(&wire.imu_wire);

    // Write to the wire
    Serial.write(wire.imu_wire.buffer, TEENSY_BUFFER_SIZE);
    /*
     * Since everything has gone to plan
     * we can now call this a complete period, so
     * we reset the imu reading and
     * get the time period passed in seconds
     */
    uint64_t prev_time = time_milli;
    wire.imu_wire.msg.dt = double(millis() - prev_time) / 1000.0;
    delay(100);
    digitalWrite(33, LOW);
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
  analogWrite(motors.rf, throttle.rf);
  analogWrite(motors.rb, throttle.rb);
  analogWrite(motors.lf, throttle.lf);
  analogWrite(motors.lb, throttle.lb);

  // update time now because motors ideally have been set
  time_milli = millis();
  return 1;
}
