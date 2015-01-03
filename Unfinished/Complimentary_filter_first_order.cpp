#include <WPILib.h>
class DefaultRobot : public SimpleRobot {
	ADXL345_I2C *accl; 
	Gyro *gyro;
	
	public:

	DefaultRobot(void){
		accl = new ADXL345_I2C(1, ADXL345_I2C::kRange_2G);
		gyro = new Gyro(1);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		gyro->SetSensitivity(0.007); //default sensitivity.  You can adjust to account for drift (to a degree)
		gyro->Reset(); 
		
		float tau      = 0.075;
		float a        = 0.000;
		float x_angleC = 0;
		float angle;
		
		float y, z, accAngle, gyrRate;
		while (IsOperatorControl()){
			y = acc.GetAcceleration(ADXL345_I2C::kAxis_Y);
			z = acc.GetAcceleration(ADXL345_I2C::kAxis_Z);		
			accAngle = atan2(accY,accZ)/**180/PI*/; //last part to convert from radians to degrees if not working properly
			gyrRate = gyro->GetRate(); 
			
			angle = Complimentary(accAngle, gyrRate, 20);
			
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Angle: %f degrees", angle);
			driverStationLCD->UpdateLCD();
		}
	}
	
	float Complementary(float newAngle, float newRate,int looptime) {
		float dtC = float(looptime)/1000.0;                                    
		a=tau/(tau+dtC) ;
		x_angleC = a * (x_angleC + newRate * dtC) + (1-a) * (newAngle);
		return x_angleC;
	}
};

START_ROBOT_CLASS(DefaultRobot);

/* Complimentary filters manage both high-pass and low-pass filters simultaneously. The low pass filter 
 * filters high frequency signals (such as the accelerometer in the case of vibration) and low pass filter 
 * filters low frequency signals (such as the drift of the gyroscope). By combining these filters, you get 
 * a good signal, without the complications of the Kalman filter.  They have different orders, going up
 * in complication while going up in accuracy, although sometimes negligible after the first order (this 
 * one).  This program is currently untested, so be wary of it.  This goes for the other two filters
 * available here as well (Kalman and Second Order Complimentary)
 */
