#include <WPILib.h>
#include <Math.h>

class PIDcontroller{
	unsigned long lastTime;
	double Input, Output, OutputTemp, Setpoint;
	double ITerm, lastInput;
	double kp, ki, kd;
	int SampleTime; //1 sec //
	#define GetMsecTime()           (GetFPGATime()/1000)
	double outMin, outMax;
	bool inAuto; //

	#define MANUAL 0
	#define AUTOMATIC 1

	#define DIRECT 0
	#define REVERSE 1
	int controllerDirection; //	
	
public:
	
	void init() {
		controllerDirection = DIRECT; 
		inAuto=false;	
		SampleTime=1000;
	}
	
	void Compute() {
		if (!inAuto) return;
		unsigned long now = GetMsecTime();
		int timeChange = (now - lastTime);
		if (timeChange>=SampleTime) {
			
			/*Compute all the working error variables*/
			double error = Setpoint - Input;
			ITerm+= (ki * error);
			if (ITerm > outMax) ITerm= outMax;
			else if (ITerm < outMin) ITerm= outMin;
			double dInput = (Input - lastInput);
	  
			/*Compute PID Output*/
			Output = kp * error + ITerm- kd * dInput;
			if (Output > outMax) Output = outMax;
			else if (Output < outMin) Output = outMin;
			OutputTemp = Output;

			/*Remember some variables for next time*/
			lastInput = Input;
			lastTime = now;
		}
	}

	void SetTunings(double Kp, double Ki, double Kd) {
		if (Kp<0 || Ki<0|| Kd<0) return;

		double SampleTimeInSec = ((double)SampleTime)/1000;
		kp = Kp;
		ki = Ki * SampleTimeInSec;
		kd = Kd / SampleTimeInSec;

		if (controllerDirection ==REVERSE) {
			kp = (0 - kp);
			ki = (0 - ki);
			kd = (0 - kd);
		}
	}

	void SetSampleTime(int NewSampleTime) {
		if (NewSampleTime > 0) {
			double ratio  = (double)NewSampleTime
				/ (double)SampleTime;
			ki *= ratio;
			kd /= ratio;
			SampleTime = (unsigned long)NewSampleTime;
		}
	}

	void SetOutputLimits(double Min, double Max) {
		if (Min > Max) return;
		outMin = Min;
		outMax = Max;

		if (Output > outMax) Output = outMax;
		else if (Output < outMin) Output = outMin;

		if (ITerm > outMax) ITerm= outMax;
		else if (ITerm < outMin) ITerm= outMin;
	}

	void SetMode(int Mode) {
		bool newAuto = (Mode == AUTOMATIC);
		if (newAuto == !inAuto) {  // we just went from manual to auto
			Initialize();
		}
		inAuto = newAuto;
	}

	void Initialize() {
		lastInput = Input;
		ITerm = Output;
		if (ITerm > outMax) ITerm= outMax;
		else if (ITerm < outMin) ITerm= outMin;
	}

	void SetControllerDirection(int Direction) {
		controllerDirection = Direction;
	}
	
	void putInput(double in){
		Input = in; //input from -1 to 1
	}
	
	double getOutput(){
		return OutputTemp; //output from min to max
	}
	
};

class DefaultRobot : public SimpleRobot {
	Joystick *xBox;
	Talon *frontLeft;
	Talon *frontRight;
	Talon *backLeft;
	Talon *backRight;
	Gyro  *gyro;
	PIDcontroller *controller;
	
	public:

	DefaultRobot(void){
		xBox       = new Joystick(1);
		frontLeft  = new Talon(1,1);
		backLeft   = new Talon(1,2);
		frontRight = new Talon(1,3);		
		backRight  = new Talon(1,4);
		gyro       = new Gyro(1);
		controller = new PIDcontroller;
		
		
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		float lXaxis, lYaxis, triggers, rXaxis, rYaxis, tVal, ptVal = 0;
		float gyro_reading, error, v, m;
		float target_angle = 0;
		gyro->Reset();
		
		
		controller->init();
		controller->SetTunings(0,0,0);
		controller->SetSampleTime(10);
		controller->SetOutputLimits(-1,1);
		controller->SetMode(AUTOMATIC);
		controller->SetControllerDirection(DIRECT);
		controller->Initialize();
		controller->Compute();
		
		while (IsOperatorControl()){
			lYaxis   = xBox -> GetRawAxis(2);
			triggers = xBox -> GetRawAxis(3);
			rXaxis   = xBox -> GetRawAxis(4);	
			tVal = rXaxis;
			
			
			if (ptVal != tVal && tVal == 0) gyro -> Reset();   //reset gyro 
			gyro_reading = gyro -> GetAngle();                    //get gyro angle
			error = target_angle - gyro_reading;                //calculate error
			error = error - 360 * floor(0.5 + error / 360.0);     //find shortest path to 0
			v = error / 180 * 1.5;  //rotation velocity
			v = v > 0.5 ? 0.5 : v < -0.5 ? -0.5 : v;
			m = tVal == 0? v : tVal * 0.25;  //switch between driver rotation / gyro rotation
			MecanumCartesian(-triggers, lYaxis, m, 0);
			
			//MecanumCartesian(triggers, lYaxis, tVal, 0);
			
			//MecanumCartesian(triggers, lYaxis, rXaxis, 0);
			//right, forward, clockwise, theta
			
			//frontLeft->Set(0.25);
			//frontRight->Set(0.25); //negate
			//backLeft->Set(0.25);
		    //backRight->Set(0.25); //negate
			
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Gyro angle:  %f", gyro_reading);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "Error:       %f", error);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 2, "Turn speed: %f", v);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 3, "Y speed: %f", v);
			driverStationLCD->UpdateLCD();
			
			ptVal = tVal;
		}
	}
	
	void MecanumCartesian(float X, float Y, float Z, float theta){
		double forward, right, clockwise, cosA, sinA, temp, max;
		double front_left, front_right, rear_left, rear_right;
		forward = -Y; right = X; clockwise = Z;

		// if rotate gain is too hot, tweak it down:
		// clockwise /= 2;  //like this

		// add deadband so you don't get strafe when you don't want it:
		if ((right>-0.1)&&(right<0.1)) right = 0; 

		//incorporate gyro
		cosA = cos(theta * (3.14159 / 180.0));
		sinA = sin(theta * (3.14159 / 180.0));
		X = X * cosA - Y * sinA;
		Y = X * sinA + Y * cosA;
		
		/*  The four lines of code above allow for
		 * field-centric holonomic driving.  Make
		 * sure that, when chosing to do this, that
		 * you comment out all the gyro resetting
		 * and turning control loop in lines 42-49
		 * and replace 0 in the method calling to
		 * the gyro angle
		 */

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


