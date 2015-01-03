#include <WPILib.h>
class DefaultRobot : public SimpleRobot {
	Gyro *gyro;
	
	public:

	DefaultRobot(void){
		gyro = new Gyro(1);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		gyro->SetSensitivity(0.007); //default sensitivity.  You can adjust to account for drift (to a degree)
		gyro->Reset(); // zeros gyro so that the current orientation is the one that every future reading is relative to
		while (IsOperatorControl()){
			gyro_reading = gyro->GetAngle(); 			
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Gyro angle:  %f", gyro_reading);
			driverStationLCD->UpdateLCD();
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);
