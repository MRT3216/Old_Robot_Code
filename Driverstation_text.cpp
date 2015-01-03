#include <WPILib.h>
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
		int    ii = 5;
		float  ff = 3.3f;
		double dd = 8.6753091018605;
		bool   bb = false;
		
		while (IsOperatorControl()){
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "%i", ii);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "%f", ff);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 2, "%d", dd);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 3, "%b", bb);
			driverStationLCD->UpdateLCD();
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);
