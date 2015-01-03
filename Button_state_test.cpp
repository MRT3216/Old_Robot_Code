#include <WPILib.h>

class DefaultRobot : public SimpleRobot {

	Joystick *xBox;    
	
	public:

	DefaultRobot(void){
		xBox           = new Joystick(1);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		driverStationLCD -> Clear();
		bool aButton = false, p_aButton = false;		
		int pushCount = 0, releaseCount = 0;
		
		while (IsOperatorControl()){
		
			aButton  = xBox -> GetRawButton(1);			
			if (aButton && !p_aButton){// && maxLimit && minLimit){
				driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Button Pushed:   %i", pushCount);
				pushCount++;
			}
			
			if (!aButton && p_aButton){// && maxLimit && minLimit){
				driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "Button Released: %i", releaseCount);
				releaseCount++;
			}
			
			driverStationLCD->UpdateLCD();
			p_aButton  = aButton;
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);
