/* The map function re-maps a number from one range to another.  For
 * instance, you could take a a value between 1 and 20 and scale it to
 * a number between 5 and 200.  This is especially helpful in autonomous
 * when mapping distance values from a sonar sensor to speeds at which
 * to drive the robot to reach a particular distance.  Example:
 *
 * 		DriveTrain -> Set(map(sonar_distance, 0, 250, -1, 1));
 *
 * The above code causes the robot to move forwards until the sonar sensor
 * reads 0 inches.  The further the rangefinder indicates the robot is from
 * 0 inches, the faster it will travel, and the closer it gets the slower it
 * will travel.  This can cause one of two problems - the robot will coast
 * past its target, or it will never reach its target due to its deceleration.
 * You can tweak the output maximum and minimum values to improve the speed
 * of approach and multiply the entire statement to improve responsiveness
 * or accuracy at low speeds.  [UNTESTED - You can also subtract a destination
 * distance from the measured distance ('value') to set that as the goal instead
 * of 0]
 */

float map(float value, float istart, float istop, float ostart, float ostop){
	return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
}

/* constrain() restricts a value to not exceed a maximum and minimum value.  This 
 * can be utilized to create a simple Integral feedback loop (just the 'I' from a 
 * PID loop).  In the following example, a sensor is used in an integral controller
 * to vary the speed of a motor in relation to the sensor value.  An integral
 * controller is characterized by eliminating the steady-state error for a constant 
 * or step input, but possibly making the transient response slower relative to a
 * proportional or derivative loop.
 *
 *
 *
 * cval += constrain(-(sensorval - goodval) / 2,-max,max);
 * DriveTrain -> Set(cval);
 *
 *
 *
 * cval      - the current value being sent to the motor
 * sensorval - measured speed of the motor
 * goodval   - the sensor output when cval is at the desired speed
 * "/2"      - the coefficient of the loop, in this case 1/2
 * max       - the max / min output to the motor controller
 * 
 */

float constrain(float value, float min, float max){
	return value > max ? max : value < min ? min : value;
}