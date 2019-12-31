/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : pid
 * @created     : Thursday Dec 19, 2019 19:45:41 MST
 */



/*
 * This is a simple pid model 
 * using a really simple outline
 */

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>


int T; // period
struct timeval last_time, curr_time;

double sensed_output, control_signal;
double setpoint;

double Kp;
double Ki;
double Kd;

double total_error, last_error;

float max_control;
float min_control;


void PID_Control();
void UpdateSensor();

int main() {
  FILE* fp = fopen("./data.dat", "w");

  srand(time(NULL));

  T = 5000; // microseconds
  gettimeofday(&curr_time, NULL);
  gettimeofday(&last_time, NULL);
  
  // Desired is 100
  setpoint = 10;
  // Current read is 0
  sensed_output = 0;

  control_signal = 0;

  max_control = 10;
  min_control = -10;

  total_error = 0;
  last_error = 0;
  Kp = 100;
  Ki = 0;
  Kd = 50;

  printf("Starting Sensed Output: %f Desired Position: %f\n", sensed_output, setpoint);

  for(int i = 0; i < 1000; ++i) {
    if(i == 400)
      setpoint = 100;
    // Execute a PID Control Cycle
    PID_Control();
    UpdateSensor();
    printf("%f\n", sensed_output);

    // Sleep 6 milliseconds
    usleep(5000);
    fprintf(fp, "%d %f\n", i, sensed_output);
  }

  fclose(fp);
  return 0;
}



double randf(double low, double high){
    double a = (rand() / (double)(RAND_MAX)) * fabs(low - high) + low;
    return a;
}


void UpdateSensor() {
  sensed_output += control_signal;
}

void PID_Control() {

  // Get the current time sample
  gettimeofday(&curr_time, NULL);

  // Change in time from last measured value
  uint64_t del_time = (curr_time.tv_usec - last_time.tv_usec);

  // Execute on a set period
  if(del_time >= T) {

    // Current discrete error 
    double error = setpoint - sensed_output;

    // Integral Term
    total_error += error;

    // Constrain
    if(total_error >= max_control)
      total_error = max_control;
    else if(total_error <= min_control)
      total_error = min_control;

    // Derivative Term
    double delta_error = error - last_error;

    printf("Vals: %f %f %f\n", error, total_error, delta_error);

    // PID term
    control_signal = Kp * error + (Ki * T) * total_error + (Kd / T) * delta_error;

    if(control_signal >= max_control)
      control_signal = max_control;
    else if(control_signal <= min_control)
      control_signal = min_control;

    // Update the time constant at the measured interval
    last_error = error;
    last_time = curr_time;
  }
}
