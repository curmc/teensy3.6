/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : teensy_msg
 * @created     : Thursday Jan 09, 2020 12:30:07 MST
 */

#include "teensy_msg.h"

void parse_cmd_vel_message(cmd_vel* msg, const uint8_t buffer[CMD_VEL_BUFFER_SIZE]) {
  float* handle = (float*)&buffer[0];
  msg->linear_v = *handle++;
  msg->angular_v = *handle;
}

void update_teensy_imu(teensy_msg* msg) {
  /*
   * Serialize the id of the message
   */
  uint64_t* id = (uint64_t*)&msg->buffer[0];
  msg->msg.id++;
  *id++ = msg->msg.id;

  /*
   * Serialize the time change of the message
   */
  double* dt = (double*)id;
  *dt++ = msg->msg.dt;

  /*
   * Serialize the actual 
   * data in the message 
   * (all floats now)
   */
  float* handle = (float*)dt;

  // ACTUAL IMU READINGS
  for(size_t i = 0; i < 3; ++i)
    *handle++ = msg->msg.linear_acceleration[i];
  for(size_t i = 0; i < 3; ++i)
    *handle++ = msg->msg.angular_velocity[i];
}
