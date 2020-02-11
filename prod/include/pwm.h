/**
 * @author      : theo (theo.j.lincke@gmail.com)
 * @file        : common
 * @created     : Friday Jan 03, 2020 16:54:25 MST
 */

#ifndef PWM_H

#define PWM_H

// HZ
#define PWM_FREQUENCY             250

// Resolution (12 bits)
#define PWM_RESOLUTION            12
#define MAX_BIT                   ((1 << PWM_RES) - 1)

// Period of a pwm signal (ms)
#define PWM_PERIOD                4.0
// High time of a zero signal (ms)
#define PWM_ZERO_TIME             1.5

// Duty cycle of a zero signal
#define PWM_DUTY_ZERO             ((PWM_ZERO_TIME) / (PWM_PERIOD))
// Zero input for analogWrite
#define PWM_ZERO                  (PWM_DUTY_ZERO * MAX_BIT)

// Seconds
#define MIN_PWM                   0.6
#define MAX_PWM                   2.4
#define MIN_PWM_INPUT             ((MIN_PWM / PWM_PERIOD) * MAX_BIT)
#define MAX_PWM_INPUT             ((MAX_PWM / PWM_PERIOD) * MAX_BIT)


// The value of the maximum drive input,
// i.e., if you set this to a value, all pwm writes will
// be equal to the fraction of input to max_drive_value
static int max_drive_value = 100;
int set_max_drive_value(int maximum){
  max_drive_value = maximum;
}

/*
 * PWM Setup stuff
 */
int pwm_init();

int pwm_new(int pin);
int pwm_drive(int pin, int val);


#endif /* end of include guard PWM_H */

