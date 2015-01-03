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
		float lYaxis, triggers, rXaxis, gyro_reading;
		gyro->Reset();
		while (IsOperatorControl()){
			lYaxis   = xBox -> GetRawAxis(2);
			triggers = xBox -> GetRawAxis(3);
			rXaxis   = xBox -> GetRawAxis(4);	
			
			gyro_reading = gyro->GetAngle();						
			MecanumCartesian(triggers, lYaxis, rXaxis, gyro_reading);
		}
	}
	
	void MecanumCartesian(float X, float Y, float Z, float theta){
		double forward, right, clockwise, cosA, sinA, temp, max;
		double front_left, front_right, rear_left, rear_right;
		forward = -Y; right = X; clockwise = Z;

		cosA = cos(theta * (3.14159 / 180.0));
		sinA = sin(theta * (3.14159 / 180.0));
		X = X * cosA - Y * sinA;
		Y = X * sinA + Y * cosA;
		
		front_left = forward + clockwise + right;
		front_right = forward - clockwise - right;
		rear_left = forward + clockwise - right;
		rear_right = forward - clockwise + right;
		
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
		
		frontLeft->Set(front_left);
		frontRight->Set(-front_right);
		backLeft->Set(rear_left);
		backRight->Set(-rear_right);
	}
};

START_ROBOT_CLASS(DefaultRobot);
