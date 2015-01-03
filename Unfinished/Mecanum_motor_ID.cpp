/* This is a major issue with mecanum wheels.  Figuring
 * out which motor goes with which PWM cable and which
 * direction it should be turning.  This should help a
 * little.  Basically, the way you figure it out is by
 * designating (in your code) which motor will be front
 * left, front right, etc. and try spinning them each
 * individually, checking whether or not the front left
 * motor as defined in your code is the front left motor
 * on the robot.  This just cycles through four motors
 * labeled appropriately, and the operator can record
 * which values need to be negated and which PWMs need
 * to be switched.
 */
 
 #include <WPILib.h>
class DefaultRobot : public SimpleRobot {
	Joystick *xBox;	
	Talon *frontRight;
	Talon *frontLeft;	
	Talon *backRight;
	Talon *backLeft;
	
	public:

	DefaultRobot(void){
		xBox       = new Joystick(1);
		frontLeft  = new Talon(1,1);
		backLeft   = new Talon(1,2);
		frontRight = new Talon(1,3);		
		backRight  = new Talon(1,4);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		bool xButton, bButton, yButton, aButton;
		int s = 1;
		float a = 0.25;
		while (IsOperatorControl()){
		
			xButton = xBox -> GetRawButton(1);
			bButton = xBox -> GetRawButton(2);
			yButton = xBox -> GetRawButton(3);
			aButton = xBox -> GetRawButton(4);	
			
			/* I'm just guessing these values.  Figure it out.
			 * Run joy.cpl (Run->[type in] joy.cpl) to find the 
			 * actual values of everything
			 */
			
			if (xButton) s--;    if (bButton) s++;
			if (yButton) a=0.25; if (aButton) a=0;			
			s = s > 4? 1 : s < 1? 4 : s;
			
			switch(s){
				case 1:
					frontRight->Set(a);
					break;
				case 2:
					frontLeft->Set(a);
					break;
				case 3:
					backRight->Set(a);
					break;
				case 4:
					backRight->Set(a);
					break;
			}
				
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "#%f at %f", s, a);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "Frnt Rght: %f", frontRight->Get());
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 2, "Frnt Left: %f", frontLeft->Get());
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 3, "Back Rght: %f", backRight->Get());
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 4, "Back Left: %f", backLeft->Get());
			driverStationLCD->UpdateLCD();
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);
