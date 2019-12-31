/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : robot
 * @created     : Thursday Dec 19, 2019 21:11:40 MST
 */

#include "robot.h"

Robot::Robot()
{
   motors.lf = LF_PIN;
   motors.lb = LB_PIN;
   motors.rf = RF_PIN;
   motors.rb = RB_PIN;

   throttle = (throttle_s){0};
   linear = 0;
   angular = 0;  
   recieved = false;
}


Robot::~Robot(){}

int Robot::robot_setup() {

  // Write pwm init frequency and zero to
  // each motor
  analogWriteFrequency(motors.lf, PWM_INIT);
  analogWrite(motors.lf, PWM_ZERO);

  analogWriteFrequency(motors.lb, PWM_INIT);
  analogWrite(motors.lb, PWM_ZERO);

  analogWriteFrequency(motors.rf, PWM_INIT);
  analogWrite(motors.rf, PWM_ZERO);

  analogWriteFrequency(motors.rb, PWM_INIT);
  analogWrite(motors.rb, PWM_ZERO);

  analogWriteResolution(PWM_RES);

  // Begin Serial communication with master
  // serial device
  Serial.begin(9600);
  // Used for imu data
  Wire.begin();

  // Set speed to zero
  speed = (speed_estimator){0};

  // Get the current time
  speed.current_time = millis();
  speed.previous_time = millis();

  
  if(imu.begin() == false) {
    while(1);
  }

  angular_pid_c.set_desired_val(0);
  angular_pid_c.set_val(0);

  imu.calibrate();
  return 1;
}


int Robot::robot_loop() {
  
  // Update the robot sensed
  // values from the sensor
  sensor_update();

  angular_pid_c.PID_Control_Loop();

  // Status on message
  int ret = serial_listen();

  callback_velocity();
  motor_write();

  return ret;
}

int Robot::convert(int8_t throttle_) {
  if(throttle_ > MAX_THROTTLE)
    throttle_ = MAX_THROTTLE;
  else if(throttle_ < -MAX_THROTTLE)
    throttle_ = -MAX_THROTTLE;

  return PWM_ZERO + (throttle_ * PWM_SCALAR);
}

int Robot::sensor_update() {

  speed.current_time = millis();

  // Read from gyroscope
  if(imu.gyroAvailable()){
    imu.readGyro();
    // Update sensed angular velocity
    speed.curr_sensed_angular = imu.calcGyro(imu.gz);
    angular_pid_c.set_val(speed.curr_sensed_accel);
  }

  // Read from accelerometer
  if(imu.accelAvailable()){
    imu.readAccel();
    
    speed.curr_sensed_accel = imu.calcAccel(imu.ax);

    //
    // // Previous speed
    // float speed_temp = speed.previous_sensed_speed;
    // speed.previous_sensed_speed = speed.curr_sensed_speed;
    //
    /*
     * V(t) = Delta(t) * acceleration + V(t - 1)
     */
    //
    // // Returns in g's mult by 9.8
    // float instant_accel = imu.calcAccel(imu.ax) * 9.8; // m/s^2
    // speed.curr_sensed_accel = instant_accel;
    //
    // float period = (speed.current_time - speed_temp) / 1000.0; // s
    //
    // if(abs(imu.calcAccel(imu.ax)) < 0.01)
    //   speed.curr_sensed_speed = 0;
    // else
    //   speed.curr_sensed_speed = instant_accel * period + speed.previous_sensed_speed;
    //
    // speed.previous_time = speed.current_time;
  }
  return 1;
}

int Robot::serial_listen() {
  int ret;
  if((ret = Serial.available())) {
    recieved = true;
    Serial.readBytes((char*)buffer, 11);

    // Recieve a value and set our new desired speed
    int* temp_lin = reinterpret_cast<int*>(&buffer[1]);
    int* temp_ang = reinterpret_cast<int*>(&buffer[5]);

    linear = *temp_lin;
    angular = *temp_ang;

  } else {
    linear = 0;
    angular = 0;
    recieved = false;
  }

  angular_pid_c.set_desired_val(angular);

  float* lin_sensed = reinterpret_cast<float*>(&buffer[0]);
  float* ang_sensed = reinterpret_cast<float*>(&buffer[4]);

  *lin_sensed = (float)speed.curr_sensed_accel;
  *ang_sensed = (float)speed.curr_sensed_angular;

  buffer[8] = '\0';
  // Serial.write(buffer, 9);
  Serial.write('l');
  Serial.print(speed.curr_sensed_accel);
  Serial.write('k');
  Serial.write('a');
  Serial.print(speed.curr_sensed_angular);
  Serial.write('b');
  Serial.write('c');
  Serial.print(angular_pid_c.get_val());
  Serial.write('d');

  return ret;
}

int Robot::callback_velocity() {
  // angular = angular_pid_c.get_val();

  throttle.lf = -(linear + angular);
  throttle.lb = -(linear + angular);
  throttle.rf = (linear - angular);
  throttle.rb = (linear - angular);
  return 1;
}

int Robot::motor_write() {
  analogWrite(motors.rf, convert(throttle.rf));
  analogWrite(motors.rb, convert(throttle.rb));
  analogWrite(motors.lf, convert(throttle.lf));
  analogWrite(motors.lb, convert(throttle.lb));
  return 1;
}
