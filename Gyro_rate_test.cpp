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
		float gyrRate;
		while (IsOperatorControl()){
			gyrRate = gyro->GetRate(); 			
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Gyro rate:  %f", gyrRate);
			driverStationLCD->UpdateLCD();
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);

/* The gyro's rate is important to know when filtering information.  It is
 * much easier to manipulate the output if you use the rate prior to it being
 * converted to degrees and run through the accumulator.  You could even utilize
 * it to make your own accumulator, but it won't be nearly as fast as the built-in
 * FPGA accumulator (which is only on analogue input ports 1 and 2, so you can
 * never have more than two gyros on-board the robot without a coprocessor.
 */