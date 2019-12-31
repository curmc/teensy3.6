/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : imu
 * @created     : Sunday Dec 29, 2019 12:10:32 MST
 */

#include "imu.h"

SpeedDiagnostics::SpeedDiagnostics(LSM9DS1& imu_, uint16_t sample_period, imu_sensitivity_s& stdev_) {
  stdev = stdev_; 
  sample_rate = sample_period;
  imu = &imu_;
  period_val = 0;
  prev_time_val = millis();

  lin_speed_status = {0,};
  ang_speed_status = {0,};

  inst_lin_speed_status = {0,};
  inst_ang_speed_status = {0,};

  imu_reading = {0,};
  imu_previous_reading = {0,};
}

bool SpeedDiagnostics::setup() {
  prev_time_val = millis();
  return true;
}


bool SpeedDiagnostics::reset(float lin_pos, float ang_pos) {
  lin_speed_status = {0,};
  ang_speed_status = {0,};

  imu_reading = {0,};
  imu_previous_reading = {0,};

  lin_speed_status.pos = lin_pos;
  ang_speed_status.pos = ang_pos;

  imu->calibrate(); 
  return this->setup();
}


bool SpeedDiagnostics::loop() {
  // 1. Get the difference in time
  float time_diff = (float)period_val;
  period_val = millis() - prev_time_val;
  time_diff = (float)period_val - time_diff;

  // If we have exceeded sample_rate, reset and start over
  if(period_val > sample_rate) {
    prev_time_val = millis();
    period_val = 0;

    // Copy into the readable values
    lin_speed_status = inst_lin_speed_status;
    ang_speed_status = inst_ang_speed_status;

    // Reset the instantaneous 
    // processed data
    inst_lin_speed_status = {0,};
    inst_ang_speed_status = {0,};
  }

  // TODO this is a lot of copying
  imu_previous_reading = imu_reading;

  if(imu->accelAvailable()) {
    imu->readAccel();
    imu_reading.ax = imu->calcAccel(imu->ax) * 9.8;
    if(abs(imu_reading.ax) < 1){
      imu_reading.ax = 0.0;
      imu->calibrate();
    }


    // Integrate to get velocity
    imu_reading.lvx = (imu_reading.ax * time_diff) + imu_previous_reading.lvx;

    // Integrate to get distance
    imu_reading.ldx = (imu_reading.lvx * time_diff) + imu_previous_reading.ldx;
  }

  // if(imu->gyroAvailable()) {
    imu->readGyro();
    imu_reading.gz = imu->calcGyro(imu->gz);

    // Differentiate to get acceleration
    imu_reading.gaz = (imu_reading.gz - imu_previous_reading.gz) / time_diff;

    // Integrate to get distance
    imu_reading.gdz = (imu_reading.gz * time_diff) + imu_previous_reading.gdz;
    Serial.println("hi");
    Serial.println(imu_reading.gaz);
    Serial.println(imu_reading.gz);
    Serial.println(imu_reading.gdz);
  // }

  uint64_t count_l = ++inst_lin_speed_status.count;
  uint64_t count_a = ++inst_ang_speed_status.count;

  if(abs(imu_reading.gz - imu_previous_reading.gz) < stdev.gz) {
    // A simple rolling average
    inst_ang_speed_status.accel = ((inst_ang_speed_status.accel * (count_a - 1)) + imu_reading.gaz) / (float)count_a;
    inst_ang_speed_status.velocity = ((inst_ang_speed_status.velocity * (count_a - 1)) + imu_reading.gz) / (float)count_a;
    inst_ang_speed_status.pos = ((inst_ang_speed_status.pos * (count_a - 1)) + imu_reading.gdz) / (float)count_a;
  }

  if(abs(imu_reading.ax - imu_previous_reading.ax) < stdev.ax) {
    inst_lin_speed_status.accel = ((inst_lin_speed_status.accel * (count_l - 1)) + imu_reading.ax) / (float)count_l;
    inst_lin_speed_status.velocity = ((inst_lin_speed_status.velocity * (count_l - 1)) + imu_reading.lvx) / (float)count_l;
    inst_lin_speed_status.pos = ((inst_lin_speed_status.pos * (count_l - 1)) + imu_reading.ldx) / (float)count_l;
  }

  return true;
}












