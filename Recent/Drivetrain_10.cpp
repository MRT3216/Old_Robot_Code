#include <WPILib.h>
#include <Math.h>
class DefaultRobot : public SimpleRobot {
	Joystick *xBox;	
	DoubleSolenoid *Latch;
	DoubleSolenoid *Retract;
	DoubleSolenoid *Arm;
	DigitalInput *loaded;          
    DigitalInput *retracted;
	Compressor *compressor;
	Talon *frontLeft;
	Talon *frontRight;
	Talon *backLeft;
	Talon *backRight;
	Gyro *gyro;
	AnalogChannel *sonar;
	Relay *intakeRollers;
    
	public:

	DefaultRobot(void){
		xBox          = new Joystick(1);
		Latch         = new DoubleSolenoid(1, 2);  //(forward, reverse)
		Retract       = new DoubleSolenoid(3, 4);  //(forward, reverse)
		Arm           = new DoubleSolenoid(5, 6);  //(forward, reverse)
		loaded        = new DigitalInput(2); //limit switch
		retracted     = new DigitalInput(3); //reed  switch
		compressor    = new Compressor(1, 1); //(Digital I/O port, Relay port)
		frontLeft     = new Talon(1);
		backLeft      = new Talon(2);
		frontRight    = new Talon(3);		
		backRight     = new Talon(4);
		gyro          = new Gyro(1);
		sonar         = new AnalogChannel(2);
		intakeRollers = new Relay(2);
		
		compressor->Start();
		Arm -> Set(DoubleSolenoid::kReverse);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		float lYaxis, triggers, rXaxis;
		float time, wait1 = 0.0f, wait2 = 0.0f, wait3 = 0.0f;
		bool button1, p_button1 = false, button2, waiting = false;
		bool canFire, isFiring = false, dontRetract = false;
		bool limitHigh, limitLow, p_limitHigh = true;
		bool leftBumper, rightBumper;
		//Arm -> Set(DoubleSolenoid::kReverse);
		Arm -> Set(DoubleSolenoid::kForward);
		intakeRollers -> Set(Relay::kOff);
		SetLatch(false);	
		
		
		//Gyro driving feedback loop related info
		float tVal, p_tVal = 0;		
		float gyro_reading, error, v, m;
		float target_angle = 0;		
		float sensitivity = 1.5;
		float rScale = 0.25, arTop = 0.5;
		gyro->Reset();
		
		
		//Rangefinder / moving average related info
		const int numReadings = 10;  // the size of the array of ranges
		int index = 0;               // the index of the current reading
		float readings[numReadings]; // the readings from the analogue input
		float total = 0;             // the running total
		float average = 0;           // the average		
		float vcc = 5;               // supplied voltage to sonar sensor (LV-MaxSonar series)
		float range = 0;             // measured approximate range from sensor (no filter)
		float Vi=vcc/512;            // voltage to inch ratio
		
		while (IsOperatorControl()){	
			DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
			time = Timer::GetPPCTimestamp();
			lYaxis      = xBox -> GetRawAxis(2);
			triggers    = xBox -> GetRawAxis(3);
			rXaxis      = xBox -> GetRawAxis(4);	
			leftBumper  = xBox -> GetRawButton(5);
			rightBumper = xBox -> GetRawButton(6);
			button1     = xBox -> GetRawButton(1); //Button A
			button2     = xBox -> GetRawButton(2); //Button B - not yet used
			limitHigh   = !loaded    -> Get();
			limitLow    = !retracted -> Get();
			ManageIntake(leftBumper, rightBumper);
			tVal = rXaxis;
			canFire = limitHigh && limitLow && !isFiring ? true : false;
			/*driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Limit switch: %i", limitHigh   );
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "Reed  switch: %i", limitLow    );
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 2, "Can fire:     %i", canFire     );
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 3, "Is firing:    %i", isFiring    );
		  //driverStationLCD->PrintfLine((DriverStationLCD::Line) 4, "Dont retract: %i", dontRetract );
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 4, "Gyro Reading: %i", gyro_reading);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 5, "Range:        %f", average     );*/
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 5, "LY:        %f", lYaxis);
			
			
			
			
			
			////////////////////////////////////////////////////////////////////////////////////////////
			
			
			
			
			
			range = sonar->GetAverageVoltage()/Vi;  // gather range from sensor
			total = total - readings[index];        // subtract the last reading
			readings[index] = range;  				// read from the sensor
			total = total + readings[index];   		// add the reading to the total
			index = index + 1;      				// advance to the next position in the array
			if (index >= numReadings) index = 0;    // if at the end of the array, wrap around to the beginning                
			average = total / numReadings;          // calculate the average	
			
			
			
			
			
			////////////////////////////////////////////////////////////////////////////////////////////
			
			
			
			
			
			if (p_tVal != tVal && tVal == 0) gyro -> Reset();   //reset gyro 
			gyro_reading = gyro -> GetAngle();                  //get gyro angle
			error = target_angle - gyro_reading;                //calculate error
			error = error - 360 * floor(0.5 + error / 360.0);   //find shortest path to 0
			v = error / 180 * sensitivity;                      //rotation velocity
			v = v > arTop ? arTop : v < -arTop ? -arTop : v;    //set max and minimum values
			m = tVal == 0? v : tVal * rScale;                   //switch between driver rotation / gyro rotation
			//MecanumCartesian(-triggers, lYaxis, m, 0.0f);     //initiate IK/controll
			MecanumCartesian(-triggers, lYaxis, tVal, 0.0f);    //initiate IK/controll
			p_tVal = tVal;                                      //set the previous value for comprison
			
			
			
			
			
			/*///////////////////////////////////////////////////////////////////////////////////////////
			
			
			
			
			
			if (button1 && !p_button1 && canFire && !dontRetract) wait1 = time + 2.00f;
			if (wait1 > 0.0f){
				isFiring = true;
				SetLatch (true);
				if (time > wait1){
					isFiring = false;
					SetLatch (false);
					wait1 = 0.0f;
					wait3 = time + 5.0f;
				}
			}
			
			if (wait3 > 0){
				waiting = true;
				if (time > wait3){
					waiting = false;
					wait3   = 0.0f;
				}
			}		
			
			//manage extending / retracting in the event that neither firing or delay is occurring
			if (!isFiring && !limitHigh && !dontRetract && !waiting) ExtendReset( true);  //change so that it can't break the frame
			if (!isFiring &&  limitHigh && !dontRetract) ExtendReset(false);	
			
			//time delay after limit switch is first tripped
			if (limitHigh && !p_limitHigh && !dontRetract && !isFiring) wait2 = time + 5.0f;
			if (wait2 > 0){
				dontRetract = true;
				ExtendReset(true);
				if (time > wait2){
					wait2 = 0;
					dontRetract = false;
				}
			}
			
			
			
			
			
			///////////////////////////////////////////////////////////////////////////////////////////*/
			
			
			
			
			
			p_button1 = button1; 
			p_limitHigh = limitHigh;
			driverStationLCD->UpdateLCD();
			
		}		
	}
	
	void ManageIntake(bool forward, bool reverse){
		if      ( forward &&  reverse) intakeRollers -> Set(Relay::kOff);	
		else if ( forward && !reverse) intakeRollers -> Set(Relay::kForward);
		else if (!forward &&  reverse) intakeRollers -> Set(Relay::kReverse);
		else intakeRollers -> Set(Relay::kOff);
	}
	
	void ExtendReset (bool state) { 
		if (state) Retract -> Set(DoubleSolenoid::kForward);
		else       Retract -> Set(DoubleSolenoid::kReverse);
	}

	void SetLatch (bool state) {
		if (state) Latch -> Set(DoubleSolenoid::kForward);
		else       Latch -> Set(DoubleSolenoid::kReverse);
	}
	
	void MecanumCartesian(float X, float Y, float Z, float theta){
		double forward, right, clockwise, cosA, sinA, temp, max;
		double front_left, front_right, rear_left, rear_right;
		forward = -Y; right = X; clockwise = Z;

		clockwise *= 0.25;
		forward *= 0.5;
		
		
		cosA = cos(theta * (3.14159 / 180.0));
		sinA = sin(theta * (3.14159 / 180.0));
		X = X * cosA - Y * sinA;
		Y = X * sinA + Y * cosA;
		
		front_left = forward + clockwise + right;
		front_right = forward - clockwise - right;
		rear_left = forward + clockwise - right;
		rear_right = forward - clockwise + right;
		
		max = fabs(front_left);
		temp = fabs(front_right);
		if (temp>max) max = temp;
		temp = fabs(rear_left);
		if (temp>max) max = temp;
		temp = fabs(rear_right);
		if (temp>max) max = temp;

		if (max>1){
			front_left/=max; 
			front_right/=max; 
			rear_left/=max; 
			rear_right/=max;
		}
		
		frontLeft->Set(front_left);
		frontRight->Set(-front_right);
		backLeft->Set(rear_left);
		backRight->Set(-rear_right);
	}
};

START_ROBOT_CLASS(DefaultRobot);
