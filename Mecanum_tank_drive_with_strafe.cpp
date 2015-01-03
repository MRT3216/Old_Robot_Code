/* This is a basic cartesian holonomic drive method
 * for use with Mecanum wheels.  It combines certain
 * desirable traits of tank drive with the ability
 * to strafe.  If anyone wants that...
 */

void MecanumCartesian(float X, float Y, float Z){

	double forward, right, clockwise, temp, max;
	double front_left, front_right, rear_left, rear_right;
	
	/* "tank drive" interface plus strafe.
     * push both joysticks forward to go forward
     * push joystick2 to the right to rotate right
     * push joystick1 forward and pull joystick2 backward to rotate clockwise
	 */
	 
	forward = -(Y1+Y2)/2; right = X2; clockwise = -(Y1-Y2)/2;

	// deadzones and speed tweaks go here
	
	front_left = forward + clockwise + right;
	front_right = forward - clockwise - right;
	rear_left = forward + clockwise - right;
	rear_right = forward - clockwise + right;
	
	max  = fabs(front_left);
	temp = fabs(front_right);
	if (temp>max) max = temp;
	temp = fabs(rear_left);
	if (temp>max) max = temp;
	temp = fabs(rear_right);
	if (temp>max) max = temp;

	if (max>1) {
		front_left/=max; 
		front_right/=max; 
		rear_left/=max; 
		rear_right/=max;
	}
	
	/* Assuming you have four motor controllers that
	 * correspond to front_left, front_right, rear_left
	 * and rear_right, you would send them the
	 * corresponding values.  Motor controllers defined
	 * in the DefaultRobot portion of the program are
	 * global by default, meaning that you can control
	 * them here, from within the method.  This is 
	 * the point in your code where you would negate
	 * the values if necessary.
	 */
	 
	frontLeft->Set(front_left);
	frontRight->Set(-front_right);
	backLeft->Set(rear_left);
	backRight->Set(-rear_right);
}