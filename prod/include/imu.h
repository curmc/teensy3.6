/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : imu
 * @created     : Sunday Dec 29, 2019 11:46:26 MST
 */

#ifndef IMU_H

#define IMU_H

#include <SparkFunLSM9DS1.h>
#include <inttypes.h>


static struct imu_sensitivity_s {
  float ax = 1000; // TODO measure these
  float gz = 1000;
} def_sens;


class SpeedDiagnostics {

  public:

    /**
     * @brief Default Constructor
     *
     * @param imu_ Pass an imu type handle for the robot to syncronize with
     * @param sample_period The period in milliseconds to collect a rolling average
     * @param standard_deviation The standard deviation of the imu, allows for filtering
     *
     * @note sets dist to 0 for both lin and ang
     */
    SpeedDiagnostics(LSM9DS1& imu_, uint16_t sample_period = 10000, imu_sensitivity_s& stdev_ = def_sens);
    ~SpeedDiagnostics(){}

    // returns true if new values
    // have not been read yet, else false
    bool reset(float, float);
    bool setup();
    bool loop();

    inline void ang_set_heading(float val) { ang_speed_status.pos = val; }
    inline void lin_set_pos(float val) { lin_speed_status.pos = val; }

    // Value Getters
    inline float lin_get_accel() { return lin_speed_status.accel; }
    inline float lin_get_velocity() { return lin_speed_status.velocity; }
    inline float lin_get_dist() { return lin_speed_status.pos; }

    inline float ang_get_accel(){ return ang_speed_status.accel; }
    inline float ang_get_velocity(){ return ang_speed_status.velocity; }
    inline float ang_get_degree(){ return ang_speed_status.pos; }

    // Confidence values
    inline float lin_get_accel_conf() { return lin_speed_status.accel_conf; }
    inline float lin_get_velocity_conf() { return lin_speed_status.velocity_conf; }
    inline float lin_get_dist_conf() { return lin_speed_status.pos_conf; }

    inline float ang_get_accel_conf(){ return ang_speed_status.accel_conf; }
    inline float ang_get_velocity_conf(){ return ang_speed_status.velocity_conf; }
    inline float ang_get_degree_conf(){ return ang_speed_status.pos_conf; }



  private:

    /*
     * A struct used for holding processed data
     */
    struct {
      float pos, pos_conf; // (Degrees), (Meters)
      float velocity, velocity_conf; // (deg / s), (m / s)
      float accel, accel_conf; // (deg / s2), (m / s2)
      uint64_t count;
    } lin_speed_status,  // Processed data
      ang_speed_status,  // Processed data
      inst_lin_speed_status,  //Processing data
      inst_ang_speed_status;  //Processing data

    /*
     * A struct used for data collection
     * "Raw" data
     */
    struct {
      // Direct measurments
      float ax; // Acceleration readings
      float gz; // Rotational (gyr) readings
      
      // Indirect measurments
      float lvx, ldx;
      float gaz, gdz;
    } imu_reading, imu_previous_reading;


    // false until user calls available, then true,
    // turns false everytime the polling period is hit
    uint32_t sample_rate;
    LSM9DS1* imu;
    imu_sensitivity_s stdev;

    uint32_t period_val;
    uint64_t prev_time_val;
};


#endif /* end of include guard IMU_H */

