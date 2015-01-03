#include <WPILib.h>
#include <Math.h>
class DefaultRobot : public SimpleRobot {
	Joystick *xBox;
	Talon *frontLeft;
	Talon *frontRight;
	Talon *backLeft;
	Talon *backRight;
	Gyro  *gyro;
	
	public:

	DefaultRobot(void){
		xBox       = new Joystick(1);
		frontLeft  = new Talon(1,1);
		backLeft   = new Talon(1,2);
		frontRight = new Talon(1,3);		
		backRight  = new Talon(1,4);
		gyro       = new Gyro(1);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		float lXaxis, lYaxis, triggers, rXaxis, rYaxis, tVal, ptVal = 0;
		float gyro_reading, error, v, m;
		float target_angle = 0;
		gyro->Reset();
		while (IsOperatorControl()){
			lYaxis   = xBox -> GetRawAxis(2);
			triggers = xBox -> GetRawAxis(3);
			rXaxis   = xBox -> GetRawAxis(4);	
			tVal = rXaxis;
		
			
			if (ptVal != tVal && tVal == 0){ gyro -> Reset(); }   //reset gyro 
			gyro_reading = gyro -> GetAngle();                    //get gyro angle
			error = target_angle - gyro_reading;                //calculate error
			error = error - 360 * floor(0.5 + error / 360.0);     //find shortest path to 0
			v = error/180*1.5;  //rotation velocity
			v = v > 0.5 ? 0.5 : v < -0.5 ? -0.5 : v;
			m = tVal == 0? v : tVal * 0.25;  //switch between driver rotation / gyro rotation
			MecanumCartesian(-triggers, lYaxis, m, 0);
			
			//MecanumCartesian(triggers, lYaxis, tVal, 0);
			
			//MecanumCartesian(triggers, lYaxis, rXaxis, 0);
			//right, forward, clockwise, theta
			
			//frontLeft->Set(0.25);
			//frontRight->Set(0.25); //negate
			//backLeft->Set(0.25);
		    //backRight->Set(0.25); //negate
			
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Gyro angle:  %f", gyro_reading);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "Error:       %f", error);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 2, "Turn speed: %f", v);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 3, "Y speed: %f", v);
			driverStationLCD->UpdateLCD();
			
			ptVal = tVal;
		}
	}
	
	void MecanumCartesian(float X, float Y, float Z, float theta){
		double forward, right, clockwise, cosA, sinA, temp, max;
		double front_left, front_right, rear_left, rear_right;
		forward = -Y; right = X; clockwise = Z;

		// if rotate gain is too hot, tweak it down:
		// clockwise /= 2;  //like this

		// add deadband so you don't get strafe when you don't want it:
		if ((right>-0.1)&&(right<0.1)) right = 0; 

		//incorporate gyro
		/*cosA = cos(theta * (3.14159 / 180.0));
		sinA = sin(theta * (3.14159 / 180.0));
		X = X * cosA - Y * sinA;
		Y = X * sinA + Y * cosA;*/
		
		/*  The four lines of code above allow for
		 * field-centric holonomic driving.  Make
		 * sure that, when chosing to do this, that
		 * you comment out all the gyro resetting
		 * and turning control loop in lines 42-49
		 * and replace 0 in the method calling to
		 * the gyro angle
		 */

		// now apply the inverse kinematic tranformation:
		front_left = forward + clockwise + right;
		front_right = forward - clockwise - right;
		rear_left = forward + clockwise - right;
		rear_right = forward - clockwise + right;

		// finally, normalize so that no wheel speed command
		// exceeds magnitude of 1:
		max=fabs(front_right);
		if (max < fabs(front_left)) max = fabs(front_left);
		if (max < fabs(rear_left)) max = fabs(rear_left);
		if (max < fabs(rear_right)) max = fabs(rear_right);
		
		if (max > 1){ 
			front_right/=max; 
			front_left/=max; 
			rear_left/=max; 
			rear_right/=max; 
		}
		
		//command motors
		frontLeft->Set(front_left);
		frontRight->Set(-front_right);
		backLeft->Set(rear_left);
		backRight->Set(-rear_right);
	}
};

START_ROBOT_CLASS(DefaultRobot);
