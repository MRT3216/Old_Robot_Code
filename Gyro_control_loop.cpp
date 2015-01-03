#include <WPILib.h>
#include <Math.h>
class DefaultRobot : public SimpleRobot {
	Talon    *motor;
	Gyro     *gyro;
	
	public:

	DefaultRobot(void){
		motor      = new Talon(1);
		gyro       = new Gyro(1);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		float gyro_reading, error, v, m;
		float sensitivity  = 1.5;  //error when this at 1 can't influence the robot's movement given small gyro readings
		float arTop        = 0.5;  //Maximum automated rotation speed
		float target_angle = 0;    //the desired angle
		gyro->Reset();
		while (IsOperatorControl()){
		
			gyro_reading = gyro -> GetAngle();                  //get gyro angle
			error = target_angle - gyro_reading;                //calculate error
			error = error - 360 * floor(0.5 + error / 360.0);   //find shortest path to 0
			v = error / 180 * sensitivity;                      //rotation velocity
			v = v > arTop ? arTop : v < -arTop ? -arTop : v;    //set max and minimum rotation values
			motor -> Set(m);                                    //command motor
			
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Gyro angle:  %f", gyro_reading);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "Error:       %f", error);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 2, "Motor speed: %f", v);
			driverStationLCD->UpdateLCD();
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);
