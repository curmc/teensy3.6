/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : covariance
 * @created     : Tuesday Jan 07, 2020 11:33:02 MST
 */

#include "covariance.h"
#include <string.h>


imu_reading_cov new_reading_cov() {
  imu_reading_cov ret;
  memset(&ret, 0, sizeof(imu_reading_cov));
  
  // Fill in rows 
  for(size_t i = 0; i < 3; ++i) {
    ret.L_P[0][i].x = &ret.linear[0]; 
    ret.L_P[1][i].x = &ret.linear[1]; 
    ret.L_P[2][i].x = &ret.linear[2]; 

    ret.A_P[0][i].x = &ret.angular[0];
    ret.A_P[1][i].x = &ret.angular[1];
    ret.A_P[2][i].x = &ret.angular[2];
  }

  // Fill in cols
  for(size_t i = 0; i < 3; ++i) {
    ret.L_P[i][0].y = &ret.linear[0]; 
    ret.L_P[i][1].y = &ret.linear[1]; 
    ret.L_P[i][2].y = &ret.linear[2]; 

    ret.A_P[i][0].y = &ret.angular[0]; 
    ret.A_P[i][1].y = &ret.angular[1]; 
    ret.A_P[i][2].y = &ret.angular[2]; 
  }
  return ret;
}

void reset_imu_reading(imu_reading_cov* cov) {
  for(size_t i = 0; i < 3; ++i) {
    cov->linear[i].measurement = 0;
    cov->linear[i].mean = 0;
    cov->linear[i].count = 0;

    cov->angular[i].measurement = 0;
    cov->angular[i].mean = 0;
    cov->angular[i].count = 0;
  }
}

// Updates a single reading 
static void update_single_reading(float measured_value, reading* r) {
  r->measurement = measured_value;
  // Rolling mean
  r->mean = ((r->mean * r->count) + measured_value) / (r->count + 1);
  r->count ++;
}

static void update_covariance(covariance* cov) {
  cov->xy_sum += cov->x->measurement * cov->y->measurement;

  // The count NOW (not before)
  uint64_t nx = cov->x->count;

  // Rolling covariance formula
  cov->cov = (cov->xy_sum - nx * cov->x->mean * cov->y->mean) / (nx - 1);
}


void update_reading(imu_reading_cov* imu, float lin_accel[3], float ang_veloc[3]) {
  for(size_t i = 0; i < 3; ++i) {
    update_single_reading(lin_accel[i], &imu->linear[i]);
    update_single_reading(ang_veloc[i], &imu->angular[i]);
  }

  for(size_t i = 0; i < 3; ++i) {
    for(size_t j = 0; j < 3; ++j) {
      update_covariance(&imu->L_P[i][j]);
      update_covariance(&imu->A_P[i][j]);
    }
  }
}

