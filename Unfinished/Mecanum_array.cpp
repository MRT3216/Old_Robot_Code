//returns [front left, front right, rear left, rear right]
/*float[] MecanumCartsian (float X, float Y, float Z, float theta){  //this does't work the way I expected it to (like with Java)
	double forward, right, clockwise, cosA, sinA, temp, max;
	double front_left, front_right, rear_left, rear_right;
	forward = -Y; right = X; clockwise = Z;

	//scale values
	//clockwise *= 0.25;
	//forward   *= 0.75;
	//right     *= 1.00;
	
	//Deadzone to prevent unwanted movement
	//if ((clockwise > -0.1) && (clockwise <0.1)) clockwise = 0; 
	//if ((forward   > -0.1) && (forward   <0.1)) forward   = 0; 
	//if ((right     > -0.1) && (right     <0.1)) right     = 0; 
	
	
	//incorporate gyro if being used.  If not, this code still stays the same
	cosA = cos(theta * (3.14159 / 180.0));
	sinA = sin(theta * (3.14159 / 180.0));
	X = X * cosA - Y * sinA;
	Y = X * sinA + Y * cosA;
	
	//apply the inverse kinematic tranformation:
	front_left = forward + clockwise + right;
	front_right = forward - clockwise - right;
	rear_left = forward + clockwise - right;
	rear_right = forward - clockwise + right;
	
	//normalize so that no wheel speed command exceeds magnitude of 1:
	max = fabs(front_left);
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
	
	return [front_left, -front_right, rear_left, -rear_right];
}*/