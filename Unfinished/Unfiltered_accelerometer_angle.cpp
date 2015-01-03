#include <WPILib.h>
class DefaultRobot : public SimpleRobot {
	ADXL345_I2C *accl; 
	
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
		float y, z, accAngle;
		while (IsOperatorControl()){
			y = acc.GetAcceleration(ADXL345_I2C::kAxis_Y);
			z = acc.GetAcceleration(ADXL345_I2C::kAxis_Z);		
			accAngle = atan2(accY,accZ)/**180/PI*/; //last part to convert from radians to degrees if not working properly
			
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Accelerometer angle: %f degrees", accAngle);
			driverStationLCD->UpdateLCD();
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);

/* Hypothetically, this code should output the angle relative to the acceleration
 * of gravity (the absolute angle).  Because it is using an accelerometer, it won't 
 * drift over time, but it also means that it's highly sensitive to motion, and will 
 * go crazy if shaken.  This is why it needs to be filtered and used in combination
 * with the gyro.
 *
 * This may be measuring the angle in the wrong plane, in which case, it needs to be
 * accounted for and the programs Complimentary_filter_first_order.cpp,
 * Complimentary_filter_second_order.cpp, and Kalman_filter.cpp will need to be changed.
 */
