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
		gyro->SetSensitivity(0.007);
		gyro->Reset(); 
		
		float Q_angle  =  0.01;
		float Q_gyro   =  0.0003;
		float R_angle  =  0.01;
		float x_bias = 0;
		float P_00 = 0, P_01 = 0, P_10 = 0, P_11 = 0;	
		float  y, S;
		float K_0, K_1;
		float x_angle = 0;
		
		float y, z, accAngle, gyrRate;
		while (IsOperatorControl()){
			y = acc.GetAcceleration(ADXL345_I2C::kAxis_Y);
			z = acc.GetAcceleration(ADXL345_I2C::kAxis_Z);		
			accAngle = atan2(accY,accZ)/**180/PI*/; //last part to convert from radians to degrees if not working properly
			gyrRate = gyro->GetRate(); 
			
			angle = KalmanCalculate(accAngle, gyrRate, 20);
			
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Angle: %f degrees", angle);
			driverStationLCD->UpdateLCD();
		}
	}
	
	float KalmanCalculate(float newAngle, float newRate,int looptime){
		float dt = float(looptime)/1000;
		x_angle += dt * (newRate - x_bias);
		P_00    +=  - dt * (P_10 + P_01) + Q_angle * dt;
		P_01    +=  - dt * P_11;
		P_10    +=  - dt * P_11;
		P_11    +=  + Q_gyro * dt;

		y   = newAngle - x_angle;
		S   = P_00 + R_angle;
		K_0 = P_00 / S;
		K_1 = P_10 / S;

		x_angle +=  K_0 * y;
		x_bias  +=  K_1 * y;
		P_00    -= K_0 * P_00;
		P_01    -= K_0 * P_01;
		P_10    -= K_1 * P_00;
		P_11    -= K_1 * P_01;

		return x_angle;
	}
};

START_ROBOT_CLASS(DefaultRobot);

/* Kalman filters are similar in design to PID loops in that they are
 * a calculus based and actively and dynamically react to its inputs.
 * Because of this, their degree of complexity goes up exponentially
 * when combining data from more than one sensor.   
 *
 * This is the best filter you can use, even from a theoretical point 
 * of view, since it is one that minimizes the errors from the true 
 * signal value. However it is very difficult to understand. In fact, 
 * the filter needs to be able to calculate the coefficients of the 
 * matrices, the process-based error, measurement error, etc. that are 
 * not trivial.  See here: http://en.wikipedia.org/wiki/Kalman_filter
 */
