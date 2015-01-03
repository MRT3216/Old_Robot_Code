#include <WPILib.h>
class DefaultRobot : public SimpleRobot {  
        
    DigitalInput *Limit;
    
	public:

	DefaultRobot(void){
		Limit  = new DigitalInput(1);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		driverStationLCD -> Clear();
		bool limit;		
		while (IsOperatorControl()){
			limitLow  = Limit -> Get();		
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Button Pushed:   %b", limitLow);			
		}		
	}
};

START_ROBOT_CLASS(DefaultRobot);
