/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : statistics
 * @created     : Tuesday Jan 07, 2020 11:05:55 MST
 */

#ifndef STATISTICS_H

#define STATISTICS_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif
/*
 * One measurement
 * (example: linear_x_accel)
 */
typedef struct {
  float measurement;
  float mean;
  uint64_t count;
} reading;

/*
 * Covariance between two
 * measurements
 */
typedef struct {
  // Point to two readings that
  // this covariance is measuring
  // These can be the same 
  // (variance)
  reading* x;
  reading* y;

  // For faster computations, 
  // the formula uses this
  float xy_sum;

  // Final covariance
  float cov;
} covariance;

typedef struct {
  // Linear covariance
  covariance L_P[3][3];
  // Angular covariance
  covariance A_P[3][3];

  // Linear readings
  reading linear[3];
  // Angular readings
  reading angular[3];
} imu_reading_cov;

// Sets to zero, creates new imu_reading
imu_reading_cov new_reading_cov();

void reset_imu_reading(imu_reading_cov* cov);

// Updates given an imu reading
void update_reading(imu_reading_cov* imu, float lin_accel[3], float ang_veloc[3]);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard STATISTICS_H */

