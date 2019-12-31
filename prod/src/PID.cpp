/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : PIDController
 * @created     : Thursday Dec 19, 2019 21:28:21 MST
 */

#include "PID.h"

PIDController::PIDController(){
  period = 1; // milliseconds 

  last_time = millis();
  curr_time = millis();

  sensed_val = 0;
  control_signal = 0;
  desired_val = 0;

  max_control = 5;
  min_control = -5;
  
  total_error = 0;
  last_error = 0;

  Kp = 100;
  Ki = 0;
  Kd = 0;
}

PIDController::~PIDController(){}

void PIDController::PID_Control_Loop() {
  curr_time = millis();

  uint64_t del_time = curr_time - last_time;


  if(del_time >= period) {
    float error = desired_val - sensed_val;

    total_error += error;

    if(total_error >= max_control)
      total_error = max_control;
    else if(total_error <= min_control)
      total_error = min_control;

    float delta_error = error - last_error;

    control_signal = Kp * error + (Ki * period) * total_error + (Kd / period) * delta_error;

    if(control_signal >= max_control)
      control_signal = max_control;
    else if(control_signal <= min_control)
      control_signal = min_control;

    last_error = error;
    last_time = curr_time;
  }
}
