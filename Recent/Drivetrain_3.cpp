#include <WPILib.h>
#include <Math.h>

static double waitTimeOut1;
class DefaultRobot : public SimpleRobot {
	Joystick *xBox;
	Talon *frontLeft;
	Talon *frontRight;
	Talon *backLeft;
	Talon *backRight;
	Gyro  *gyro;	
	Solenoid *openLatch;
	Solenoid *closeLatch;	
	Solenoid *extendArm;
	Solenoid *retrctArm;
	DigitalInput *limitSwitchMax;          
    DigitalInput *limitSwitchMin;
    Relay *intakeRollers; //use PWM splitter for both spikes
	Compressor *compressor;
    
	public:

	DefaultRobot(void){
		xBox           = new Joystick(1);
		frontLeft      = new Talon(1,1);
		backLeft       = new Talon(1,2);
		frontRight     = new Talon(1,3);		
		backRight      = new Talon(1,4);
		gyro           = new Gyro(1);		
		openLatch      = new Solenoid(1);
		closeLatch     = new Solenoid(2);		
		extendArm      = new Solenoid(3);
		retrctArm      = new Solenoid(4);
		limitSwitchMax = new DigitalInput(1);
		limitSwitchMin = new DigitalInput(2);
		intakeRollers  = new Relay(1);
		compressor     = new Compressor(1, 2); //1 is the Digital I/O, and 2 is the Relay port
		compressor->Start();
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		driverStationLCD -> Clear();
		bool xButton, yButton, aButton, bButton;
		bool p_xButton = false, p_yButton = false;
		bool p_aButton = false, p_bButton = false;
		
		bool leftBumper, rightBumper;
		bool firing = false;
		
		float rXaxis, lYaxis, triggers;
		float tVal, p_tVal = 0;
		
		float gyro_reading, error, v, m;
		bool maxLimit, minLimit;
		float target_angle = 0;		
		float sensitivity = 1.5;
		float rScale = 0.25, arTop = 0.5;
		
		int pushCount = 0, releaseCount = 0;
		
		gyro->Reset();
		
		while (IsOperatorControl()){
			lYaxis   = xBox -> GetRawAxis(2);
			triggers = xBox -> GetRawAxis(3);
			rXaxis   = xBox -> GetRawAxis(4);
			aButton  = xBox -> GetRawButton(1);
			bButton  = xBox -> GetRawButton(2);
			xButton  = xBox -> GetRawButton(3);
			yButton  = xBox -> GetRawButton(4);	
			
			leftBumper  = xBox -> GetRawButton(5);
			rightBumper = xBox -> GetRawButton(6);
			
			maxLimit = limitSwitchMax->Get();
			minLimit = limitSwitchMin->Get();
			
			tVal = rXaxis;			
			
			if (p_tVal != tVal && tVal == 0) gyro -> Reset();   //reset gyro 
			gyro_reading = gyro -> GetAngle();                  //get gyro angle
			error = target_angle - gyro_reading;                //calculate error
			error = error - 360 * floor(0.5 + error / 360.0);   //find shortest path to 0
			v = error / 180 * sensitivity;                      //rotation velocity
			v = v > arTop ? arTop : v < -arTop ? -arTop : v;    //set max and minimum values
			m = tVal == 0? v : tVal * rScale;                   //switch between driver rotation / gyro rotation
			MecanumCartesian(-triggers, lYaxis, m, 0.0f);       //initiate IK/controll
			//right, forward, clockwise, theta
			
			
			/*if (aButton && !p_aButton && !firing){// && maxLimit && minLimit){
				driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Init firing");
				waitTimeOut1 = Timer::GetPPCTimestamp() + 0.5;
				firing = true;
			}
			
			if (Timer::GetPPCTimestamp > 0){
				OpenLatch(true);
				if (Timer::GetPPCTimestamp() >= waitTimeOut1){
					OpenLatch(false);
					waitTimeOut1 = 0.0;
					firing = false;
				}
			}*/
			
			if (aButton && !p_aButton){// && maxLimit && minLimit){
				driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Button Pushed:   %i", pushCount);
				pushCount++;
			}
			if (!aButton && p_aButton){// && maxLimit && minLimit){
				driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "Button Released: %i", releaseCount);
				releaseCount++;
			}
			
			ManageIntake(rightBumper, leftBumper);
			
			/*driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Gyro angle:  %f", gyro_reading);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "Error:       %f", error);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 2, "Turn speed: %f", v);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 3, "Y speed: %f", v);*/
			driverStationLCD->UpdateLCD();
			
			p_tVal = tVal;
			p_aButton  = aButton;
			p_bButton  = bButton;
			p_xButton  = xButton;
			p_yButton  = yButton;	
		}
	}
	
	void ManageIntake(bool forward, bool reverse){
		if      ( forward &&  reverse) intakeRollers -> Set(Relay::kOff);	
		else if ( forward && !reverse) intakeRollers -> Set(Relay::kForward);
		else if (!forward &&  reverse) intakeRollers -> Set(Relay::kReverse);
		else intakeRollers -> Set(Relay::kOff);
	}
	
	void SetArm(bool state){
		extendArm->Set( state);
		retrctArm->Set(!state);
	}
	
	void OpenLatch(bool state){
		openLatch->Set(  state);
		closeLatch->Set(!state);
	}
	
	void MecanumCartesian(float X, float Y, float Z, float theta){
		double forward, right, clockwise, cosA, sinA, max;
		double front_left, front_right, rear_left, rear_right;
		forward = -Y; right = X; clockwise = Z;

		// add deadband so you don't get strafe when you don't want it:
		if ((right>-0.1)&&(right<0.1)) right = 0; 

		//incorporate gyro
		cosA = cos(theta * (3.14159 / 180.0));
		sinA = sin(theta * (3.14159 / 180.0));
		X = X * cosA - Y * sinA;
		Y = X * sinA + Y * cosA;

		// now apply the inverse kinematic tranformation:
		front_left = forward + clockwise + right;
		front_right = forward - clockwise - right;
		rear_left = forward + clockwise - right;
		rear_right = forward - clockwise + right;

		// finally, normalize so that no wheel speed command
		// exceeds magnitude of 1:
		max=fabs(front_right);
		if (max < fabs(front_left)) max = fabs(front_left);
		if (max < fabs(rear_left)) max = fabs(rear_left);
		if (max < fabs(rear_right)) max = fabs(rear_right);
		
		if (max > 1){ 
			front_right/=max; 
			front_left/=max; 
			rear_left/=max; 
			rear_right/=max; 
		}
		
		//command motors
		frontLeft->Set(front_left);
		frontRight->Set(-front_right);
		backLeft->Set(rear_left);
		backRight->Set(-rear_right);
	}
};

START_ROBOT_CLASS(DefaultRobot);
