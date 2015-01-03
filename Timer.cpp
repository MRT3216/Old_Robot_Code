#include <WPILib.h>
#define GetMsecTime() (GetFPGATime()/1000)
class DefaultRobot : public SimpleRobot {
    
	public:

	DefaultRobot(void){
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		driverStationLCD -> Clear();		
		float actualTime = GetMsecTime(), ttime;
		
		while (IsOperatorControl()){
			ttime = GetMsecTime()-actualTime;
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "%f", ttime);
			driverStationLCD->UpdateLCD();
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);
