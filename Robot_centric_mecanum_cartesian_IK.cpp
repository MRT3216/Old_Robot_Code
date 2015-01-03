// 3-axis joystick interface to a mecanum drive

// define your driver interface,
// in this case a 3-axis joystick:
forward = -Y;
right = X;
clockwise = Z;

// put any drivability adjustments here for forward, right, and clockwise,
// for example gain, deadband, etc:

// if rotate gain is too hot, tweak it down:
clockwise /= 2;  

// add deadband so you don't get strafe when you don't want it:
if ((right>-0.1)&&(right<0.1)) right = 0; 


//inverse kinematic transformation:
front_left = forward + clockwise + right;
front_right = forward - clockwise - right;
rear_left = forward + clockwise - right;
rear_right = forward - clockwise + right;


// finally, normalize so that no wheel speed command
// exceeds magnitude of 1:
max = abs(front_left);
temp = abs(front_right);
if (temp>max) max = temp;
temp = abs(rear_left);
if (temp>max) max = temp;
temp = abs(rear_right);
if (temp>max) max = temp;

if (max>1) {
	front_left/=max; 
	front_right/=max; 
	rear_left/=max; 
	rear_right/=max;
}


/* You're done. send these four wheel commands to their respective wheels
 * Because of the orientation of most mecanum drivetrains, you will probably
 * have to negate the values for either the right or left side.  Do that as
 * you're commanding the motors (as in motor->Set(-speed);).
 */