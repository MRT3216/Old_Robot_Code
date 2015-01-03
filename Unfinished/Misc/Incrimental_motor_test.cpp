#include <WPILib.h>
#include <Math.h>
class DefaultRobot : public SimpleRobot {	
	Joystick *xBox;
	Talon *motor1;
	
	public:

	DefaultRobot(void){
		xBox       = new Joystick(1);
		motor1     = new Talon(1,1);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		float motorVal = 0;
		while (IsOperatorControl()){
			bool xButton   = xBox -> GetRawButton(3);
			bool lBumper   = xBox -> GetRawButton (5);
			bool rBumper   = xBox -> GetRawButton (6);
			
			
			
			if (xButton){
				motorVal = 0;
				Wait(0.2);
			}
			
			else if (lBumper){
				if (motorVal > -1.0f){
					motorVal -= 0.1f;
					Wait(0.2);
				}
			}
			
			motor1 -> Set(motorVal);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "%f", motorVal);
			driverStationLCD->UpdateLCD();
	}}
};

START_ROBOT_CLASS(DefaultRobot);
