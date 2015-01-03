/* This is a basic cartesian holonomic drive method
 * for use with Mecanum wheels.  Consult the other
 * files for a look at implementation.  The difference
 * between this and the previous one is that this has
 * been encapsulated in a method (in case you didn't
 * know how to use it).  This is how all subsystems
 * should be divided to increase readability and
 * modularity.
 */

void MecanumCartesian(float X, float Y, float Z){

	double forward, right, clockwise, temp, max;
	double front_left, front_right, rear_left, rear_right;
	forward = -Y; right = X; clockwise = Z;

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