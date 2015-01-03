/* ...This is what Tank Drive is.  Its pathetic and awful
 * for most cases.  There are a few slightly ineligible 
 * methods for using low pass filters to change the
 * joystick's responsiveness based on the current speed
 * (gathered by encoders), but without extra sensory input
 * there's nothing more to it than what can be seen here.
 */

void TankDrive(float X, float Y){	 
	frontLeft->Set(X);
	frontRight->Set(Y);
	backLeft->Set(X);
	backRight->Set(Y);
}