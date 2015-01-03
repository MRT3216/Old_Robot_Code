#include <WPILib.h>
class DefaultRobot : public SimpleRobot {
	ADXL345_I2C *accl; 
	/* ADXL345 is the part number of the standard KOP accelerometer 
	 * (usually paired with the gyro, although completely independent.
	 * I2C is the control method (probably the easier of the two, the
	 * alternative being SPI).
	 */
	
	public:

	DefaultRobot(void){
		accl = new ADXL345_I2C(1, ADXL345_I2C::kRange_2G);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		float x, y, z;
		while (IsOperatorControl()){
			x = acc.GetAcceleration(ADXL345_I2C::kAxis_X);
			y = acc.GetAcceleration(ADXL345_I2C::kAxis_Y);
			z = acc.GetAcceleration(ADXL345_I2C::kAxis_Z);		
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "X=%f  Y=%f  Z=%f", x, y, z);
			driverStationLCD->UpdateLCD();
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);
