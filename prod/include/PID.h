#include <Arduino.h>

/*
 * An abstract PID controller
 *
 * A "val" is an element (speed or angular for example)
 * PID returns a new "val" that will help the robot get to
 * that desired "val"
 *
 * currently not much config, and only uses proportional
 * (Ki and Kd are set to 0)
 */
class PIDController {
  
  public:
    PIDController();
    ~PIDController();

  // Setters
  public:
    inline void set_desired_val(float val_) { 
      desired_val = val_; 
    }

    // Ideally invoked every loop when you read a value
    inline void set_val(float val_) { 
      sensed_val = val_; 
    }

    // Returns the next val we want to travel
    inline float get_val() {
      return sensed_val + control_signal;
    }

    inline float get_desired_val() {
      return desired_val;
    }

   
    void PID_Control_Loop();

  private:
    uint64_t period;

    // Time period
    uint64_t last_time;
    uint64_t curr_time;

    // Values used for error collection
    float sensed_val; 
    float control_signal;
    float desired_val;

    float max_control;
    float min_control;

    // Error values
    float total_error;
    float last_error;

    // Gain Values
    float Kp;
    float Ki;
    float Kd;
};
