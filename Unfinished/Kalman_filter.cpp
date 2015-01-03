/* The Kalman filter is one of the most common and elaborate
 * data filters in industry, and is right up there with PID
 * loops with regard to it's comprehensibility.  Kalman
 * filters are , like PID loops, a calculus based system used 
 * to perform a specific task in an extremely specific way 
 * while actively reacting to the input.  The generalized 
 * definition is that it is an algorithm that uses a series of 
 * measurements observed over time, containing noise and other 
 * inaccuracies, and produces estimates of unknown variables 
 * that tend to be more precise than those based on a single 
 * measurement alone. This is the simplest snippet that can 
 * be considered a "kalman filter", being just a step above
 * an unnamed low pass filter.
 */

typedef struct {
  double q; //process noise covariance
  double r; //measurement noise covariance
  double x; //value
  double p; //estimation error covariance
  double k; //kalman gain
} kalman_state;

//Next it needs a function to initialize the kalman filter:

void kalman_init(double q, double r, double p, double intial_value) {
  kalman_state result;
  result.q = q;
  result.r = r;
  result.p = p;
  result.x = intial_value;
  return result;
}

/* And a function to update the kalman state, by calculating a 
 * prediction and verifying that against the real measurement:
 */

void kalman_update(kalman_state* state, double measurement) {
  //prediction update; omit x = x
  state->p = state->p + state->q;

  //measurement update
  state->k = state->p / (state->p + state->r);
  state->x = state->x + state->k * (measurement - state->x); //output variable
  state->p = (1 - state->k) * state->p;
}

/* First of all, the Kalman filter can be much easier to implement 
 * if the underlying model is simple. There weren't several dimensions, 
 * nor different sensor data to combine. The results are pretty good, 
 * but it is a challenge to find the right values for the process and 
 * sensor noise. Instead of just coming up with some guesses for the 
 * values, they can of course be estimated, but then you have to live 
 * with the result of the estimation.
 *
 * In the end it is a good, robust, simple filter (in one dimension).
 * In lack of several sensors it throws out the biggest advantage of 
 * the filter to combine several sensor data to a combined result. 
 * Consult the next Kalman filter example for combing a gyro with 
 * an accelerometer to reduce noise.