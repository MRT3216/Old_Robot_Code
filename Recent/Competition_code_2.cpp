//This is the most recent successful tested code

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
	Victor *pickup;
	Gyro *gyro;
	AnalogChannel *range;
	Talon *frontLeft;
	Talon *frontRight;
	Talon *backLeft;
	Talon *backRight;
	DriverStationLCD *driverStationLCD;
    
	public:

	DefaultRobot(void){
		xBox       = new Joystick(1);
		Latch      = new DoubleSolenoid(1, 2);  //(forward, reverse)
		Retract    = new DoubleSolenoid(3, 4);  //(forward, reverse)
		Arm        = new DoubleSolenoid(5, 6);  //(forward, reverse)
		loaded     = new DigitalInput(2); //limit switch
		retracted  = new DigitalInput(4); //reed  switch
		compressor = new Compressor(1, 1); //(Digital I/O port, Relay port)
		pickup     = new Victor(5);
		gyro       = new Gyro(1);
		range      = new AnalogChannel(2);
		frontLeft  = new Talon(1);
		backLeft   = new Talon(2);
		frontRight = new Talon(3);		
		backRight  = new Talon(4);
		driverStationLCD =  DriverStationLCD::GetInstance();
		
		
		compressor->Start();
		Arm -> Set(DoubleSolenoid::kForward);
		//SetLatch(false);
		
	}
	
	void Autonomous(void){
		
		float minDistance      =  36.0f; //closest allowable distance to the wall
		float shootingDistance = 144.0f;
		float speed            = 000.0f;
		bool hasfired = false;
		
		
		
		//Rangefinder related		
		float vcc = 5;               // supplied voltage to sonar sensor (LV-MaxSonar series)
		float Vi=vcc/512;            // voltage to inch ratio
		float a_range;               // the range from the gyro
		
		
		
		//Gyro driving feedback loop related info	
		float error, v, angle;
		float target_angle = 0;		
		float sensitivity = 1.0f;
		float arTop = 0.5f;
		gyro -> Reset();
		
		
		
		//Moving average stuff
		int numreadings = 3;
		float readings[numreadings];
		int index   = 0;
		float total   = 750;
		float average = 0;
		for (int i = 0; i < numreadings; i++) readings[i] = 250;
		
		double time = 0, itime = Timer::GetPPCTimestamp();
		double wait = .5;
		bool haswaited = false;
		
		//Initial physical setup
		pickup -> Set(0);
		SetLatch(false);
		ExtendReset(false);
		manageArm(false, false);
		Arm  -> Set(DoubleSolenoid::kReverse);
		
		while(IsAutonomous()){
			angle = gyro -> GetAngle();
			a_range = range -> GetAverageVoltage()/Vi;
			time = Timer::GetPPCTimestamp();
			
			total -= readings[index];
			readings[index] = a_range;
			total += readings[index];
			index++;
			if (index>=numreadings) index=0;
			average = total / numreadings;
			
			
		
		    
		    ////////////////////////////////////////////////////////////////////////////////////////////
			
			

			
			speed = map(a_range, 0, 200, 0, 1)*0.5+0.1;
			if (time > itime+wait) haswaited = true;
			if (average > minDistance) MecanumCartesian(0.0f, -speed, speed*0.025, 0.0f);       		    //Drive straight (?)
			if (average < minDistance) MecanumCartesian(0.0f, 0.0f, 0, 0.0f);
			if (!hasfired && (average < shootingDistance) && haswaited){ hasfired = true; SetLatch(true); }
			printf("Range: %f  Average Range: %f  Time: %f  Over %fs:  %i\n", a_range, average, time, wait, haswaited);
		}				
	}
	
	void OperatorControl(void){
		
		driverStationLCD -> Clear();		
		pickup -> Set(0);
		bool left, right;
		float angle, button2, p_button2 = true;
		float time, wait1 = 0.0f, wait2 = 0.0f, wait3 = 0.0f;
		bool button1, p_button1 = true, waiting = false;
		bool canFire, isFiring = false, dontRetract = false;
		bool limitHigh, limitLow, p_limitHigh = true;
		SetLatch(false);
		
		
		
		//Rangefinder / moving average related info		
		float vcc = 5;               // supplied voltage to sonar sensor (LV-MaxSonar series)
		float Vi=vcc/512;            // voltage to inch ratio
		float a_range;               // the range from the gyro
		
		
		
		//Gyro driving feedback loop related info
		float lYaxis, triggers, rXaxis;
		float tVal, p_tVal = 0;		
		float error, v, m;
		float target_angle = 0;		
		float sensitivity = 1.5f;
		float rScale = 0.5f, arTop = 0.5f;
		gyro -> Reset();
		
		
		
		
		float shootingdistance = 144.0f;
		Arm  -> Set(DoubleSolenoid::kReverse);
		
		
		
		
		//Moving average stuff
		int numreadings = 3;
		float readings[numreadings];
		int index   = 0;
		float total   = 750;
		float average = 0;
		for (int i = 0; i < numreadings; i++) readings[i] = 250;
		
		
		
		
		
		while (IsOperatorControl()){	
			time = Timer::GetPPCTimestamp();
			button1     = xBox -> GetRawButton(1); //Button A
			button2     = xBox -> GetRawButton(2); //Button B
			left        = xBox -> GetRawButton(5);
			right       = xBox -> GetRawButton(6);
			lYaxis      = xBox -> GetRawAxis(2);
			triggers    = xBox -> GetRawAxis(3);
			rXaxis      = xBox -> GetRawAxis(4);	
			limitHigh   = !loaded    -> Get();
			limitLow    = !retracted -> Get();
			angle       = gyro -> GetAngle();
			tVal        = rXaxis;
			canFire     = limitHigh && limitLow && !isFiring ? true : false;
			a_range = range -> GetAverageVoltage()/Vi;  //filter this?
			
			manageArm(right, left);
			
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "L_H: %i, L_L: %i", limitHigh, limitLow);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "C_F: %i, I_F: %i", canFire, isFiring);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 2, "D_R: %i", dontRetract);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 3, "Range:  %f", a_range);
			
			
			
			
			if (!button2) MecanumCartesian(-triggers, lYaxis, /*m*/ tVal, 0.0f);  //initiate IK/control
			else if (button2) MecanumCartesian(0.0f, -map(a_range, 144, 250, -1, 1), 0.0f, 0.0f);
			
			
			
			
			total -= readings[index];
			readings[index] = a_range;
			total += readings[index];
			index++;
			if (index>=numreadings) index=0;
			average = total / numreadings;
		    
		    
					    
		    
		    
		    ////////////////////////////////////////////////////////////////////////////////////////////
			
			
			
			
			
			if (button1 && !p_button1 && canFire && !dontRetract) wait1 = time + 5.0f;
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
			if (!isFiring && !limitHigh && !dontRetract && !waiting) ExtendReset(true);
			if (!isFiring &&  limitHigh && !dontRetract)                         ExtendReset(false);	
			if (limitHigh && !limitLow && !canFire && !isFiring && !dontRetract) ExtendReset(false);
			
			//time delay after limit switch is first tripped
			if (limitHigh && !p_limitHigh && !dontRetract && !isFiring && !waiting) wait2 = time + 5.0f;
			if (wait2 > 0){
				dontRetract = true;
				ExtendReset(true);
				if (time > wait2){
					wait2 = 0;
					dontRetract = false;
				}
			}
			
			
			
			
			
			////////////////////////////////////////////////////////////////////////////////////////////
			
			
			
			
			
			p_button1 = button1; 
			p_limitHigh = limitHigh;
			driverStationLCD->UpdateLCD();
			
		}		
	}
	
	void ExtendReset (bool state) { 
		if (state) Retract -> Set(DoubleSolenoid::kForward);
		else       Retract -> Set(DoubleSolenoid::kReverse);
	}

	void SetLatch (bool state) {
		if (state) Latch -> Set(DoubleSolenoid::kForward);
		else       Latch -> Set(DoubleSolenoid::kReverse);
	}
	
	void manageArm(bool forward, bool reverse){
		if      ( forward && !reverse) pickup -> Set(1);
		else if (!forward &&  reverse) pickup -> Set(-1);
		else pickup -> Set(0);
	}
	
	float map(float value, float istart, float istop, float ostart, float ostop){
		return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
	}
	
	void MecanumCartesian(float X, float Y, float Z, float theta){
		double forward, right, clockwise, cosA, sinA, temp, max;
		double front_left, front_right, rear_left, rear_right;
		forward = -Y; right = X; clockwise = Z;

		//clockwise *= 0.25;
		//forward *= 0.75;
		
		
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
