/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : teensy_msg
 * @created     : Thursday Jan 09, 2020 12:23:07 MST
 */

#ifndef TEENSY_MSG_H

#define TEENSY_MSG_H

#include <inttypes.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TEENSY_BUFFER_SIZE ((sizeof(float)) + (sizeof(float) * 6 * 2) + (sizeof(float) * 3 * 2) + sizeof(uint64_t))
#define CMD_VEL_BUFFER_SIZE (sizeof(float) * 2)

typedef struct {
  uint64_t id;
  double dt;

  float angular_velocity[3];
  float linear_acceleration[3];

  float angular_velocity_covariance[9];
  float linear_acceleration_covariance[9];
} imu_message;

typedef struct {
  imu_message msg;
  uint8_t buffer[TEENSY_BUFFER_SIZE];
} teensy_msg;

typedef struct {
  float linear_v;
  float angular_v;
  uint8_t buffer[CMD_VEL_BUFFER_SIZE];
} cmd_vel;

void parse_cmd_vel_message(cmd_vel* msg, const uint8_t buffer[CMD_VEL_BUFFER_SIZE]);

// Assumes values are already set
void update_teensy_imu(teensy_msg* msg);

#ifdef __cplusplus
}
#endif

#endif /* end of include guard TEENSY_MSG_H */

