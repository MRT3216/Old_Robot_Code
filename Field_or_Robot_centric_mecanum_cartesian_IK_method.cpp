/* This is the logic for field-centric holonomic drive
 * for Mecanum wheels.  Robot centric drive is where
 * the coordinate system on which the Inverse Kinematic
 * translation of the wheels is relative to the robot
 * (always the same; the robot always in the same
 * place relative to itself).  This mode mimics the 
 * feel of an FPS game, and should be the easiest to get
 * accustomed to.  Field-centric drive's coordinate 
 * system is relative to the robot's starting
 * orientation, gathered by the gyro once at the
 * beginning of the match and referenced continuously
 * throughout the robot's running time. The vector is 
 * rotated using the trigonometry on line ____.
 * Relative to the starting position, telling the robot
 * to drive "forward" (Y) will cause the robot to move
 * forward RELATIVE to the starting position (by the
 * way, zero/reset the gyro at the beginning of 
 * OperatorControl()).  The same goes for strafing.
 * It doesn't matter which way the robot is facing;
 * forward is forward, left is left, and so on.  
 */


void MecanumCartesian(float X, float Y, float Z, float theta){
	double forward, right, clockwise, cosA, sinA, temp, max;
	double front_left, front_right, rear_left, rear_right;
	forward = -Y; right = X; clockwise = Z;
	
	// deadzones and speed tweaks go here

	//incorporate gyro
	cosA = cos(theta * (3.14159 / 180.0));
	sinA = sin(theta * (3.14159 / 180.0));
	X = X * cosA - Y * sinA;
	Y = X * sinA + Y * cosA;

	// now apply the inverse kinematic tranformation:
	front_left = forward + clockwise + right;
	front_right = forward - clockwise - right;
	rear_left = forward + clockwise - right;
	rear_right = forward - clockwise + right;
	
	/* THE BEST PART!  Putting in 0 for theta (instead of 
	 * the gyro angle) in the method will cause cosA to equal 
	 * 1 and sinA to equal 0, making X and Y equal to the 
	 * original input, making it robot-centric again (meaning
	 * that you have nothing to lose by utilizing this code as-is).
     */

	// finally, normalize so that no wheel speed command
	// exceeds magnitude of 1:
	max  = fabs(front_left);
	temp = fabs(front_right);
	if (temp>max) max = temp;
	temp = fabs(rear_left);
	if (temp>max) max = temp;
	temp = fabs(rear_right);
	if (temp>max) max = temp;

	if (max>1){
		front_left/=max; 
		front_right/=max; 
		rear_left/=max; 
		rear_right/=max;
	}
	
	//command motors
	frontLeft->Set(front_left);
	frontRight->Set(-front_right);
	backLeft->Set(rear_left);
	backRight->Set(-rear_right);
}