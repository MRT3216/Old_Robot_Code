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
		loaded     = new DigitalInput(2);       //limit switch
		retracted  = new DigitalInput(4);       //reed  switch
		compressor = new Compressor(1, 1);      //(Digital I/O port, Relay port)
		pickup     = new Victor(5);
		range      = new AnalogChannel(2);
		frontLeft  = new Talon(1);
		backLeft   = new Talon(2);
		frontRight = new Talon(3);		
		backRight  = new Talon(4);
		driverStationLCD =  DriverStationLCD::GetInstance();
		
		
		compressor->Start();
		Arm -> Set(DoubleSolenoid::kForward);
		SetLatch(false);
		
	}
	
	void Autonomous(void){
		
		float minDistance      =  36.0f; //closest allowable distance to the wall (in inches)
		float shootingDistance = 144.0f; //the distance at which to release the latch (also in inches)
		float speed            = 000.0f; //speed to drive at to reach the goal distance
		bool hasfired = false;           //prevents the 
		
		
		
		//Rangefinder related		
		float vcc = 5;               // supplied voltage to sonar sensor (LV-MaxSonar series)
		float Vi=vcc/512;            // voltage to inch ratio
		float a_range;               // the range from the sonar sensor
		
		
		
		//Moving average stuff
		int numreadings = 3;          //The number of samples to smooth.  Helps to avoid incorrect readings due to noise or voltage spikes  
		float readings[numreadings];  //Stores the values from the rangefinder
		int index   = 0;              //The location within the array to store the value
		float total   = 750;          //The running total of the samples in the array
		float average = 0;            //The total / the number of readings
		for (int i = 0; i < numreadings; i++) readings[i] = 250;  //This may still work if changed to 0 because of the timer which guarantees that 
																  //the array has been cycled through (meaning that the initial values don't matter)
		
		
		
		//Control variables for managing the sequence of events
		double time = 0;                         //The current time
		double itime = Timer::GetPPCTimestamp(); //The initial time used for determining elapsed time
		double wait = .5;                        //The minimum period of time before shooting. It ensures that the smoothed range 
		                                         //is accurate by allowing the array to be cycled through at least once (this can be shortened)
		bool haswaited = false;                  //Prevents the latch from opening and closing once it has fired
		
		//Initial physical setup
		SetLatch(false);                         //close the latch (it should already be closed)
		ExtendReset(false);                      //make sure that the resetting cylinders are retracted
		Arm  -> Set(DoubleSolenoid::kReverse);   //lower the pickup arm
		
		while(IsAutonomous()){
		
			a_range = range -> GetAverageVoltage()/Vi;  //Convert analogue voltage from rangefinder to inches
			time = Timer::GetPPCTimestamp();            //get elapsed time since program start
			
			
			
			
			
			//moving average range smoothing
			total -= readings[index];         //subtract the last reading
			readings[index] = a_range;        //place value from sensor
			total += readings[index];         //add the reading to the total
			index++;                          //advance to the next position in the array
			if (index>=numreadings) index=0;  //if its at the end of the array, wrap around to the beginning
			average = total / numreadings;    //calculate the average
			
			
		
		    
		    ////////////////////////////////////////////////////////////////////////////////////////////
			
			

			
			speed = map(a_range, 0, 200, 0, 1)*0.5+0.1;   // Convert non-averaged range to a speed at which to drive forward
			if (time > itime+wait) haswaited = true;      // Check that a sufficient amount of time has elapsed to ensure that the moving average is populated with legit values
			if (average > minDistance) MecanumCartesian(0.0f, -speed, speed*0.025, 0.0f);       		     //Approach minimum desirable distance from the wall while decelerating 
			if (average < minDistance) MecanumCartesian(0.0f, 0.0f, 0, 0.0f);                                //Stop driving when close enough to the wall (consolidate with above?)
			if (!hasfired && (average < shootingDistance) && haswaited){ hasfired = true; SetLatch(true); }  //Fire iff time has elapsed, its close enough to the wall, and it hasn't already fired
			//printf("Range: %f  Average Range: %f  Over %fs:  %i\n", a_range, average, wait, haswaited);    //Constantly output information to NetConsole for diagnostic purposes.
		}				
	}
	
	void OperatorControl(void){
		
		driverStationLCD -> Clear();		
		//pickup -> Set(0);
		float lYaxis, triggers, rXaxis;
		bool left, right;           //gamepad bumpers used for controlling intake wheels          
		float button2;              //B button - hold to auto-align with the wall (distance only)
		float time;                 //Elapsed time in seconds
		float speed = 0.0f;         //Mapped speed based on rangefinder readings for aim assist
		float wait1 = 0.0f;         //
		float wait2 = 0.0f;         //
		float wait3 = 0.0f;         //
		bool button1;               //A button - initiates firing sequence
		bool p_button1   = true;    //Checked against the current state of the A button to determine a change in states
		bool waiting     = false;   //Variable used to delay resetting the arm after firing
		bool canFire   /*= false*/; //True if all conditions relating to firing allow for it
		bool isFiring    = false;   //True when firing
		bool dontRetract = false;   //Denies the arm's ability to retract until a period of time after the limit switch is tripped
		bool limitHigh /*= true*/;  //The state of the limit switch beneath the latch (true if pushed)
		bool limitLow  /*= true*/;  //The state of the reed switch around the resetting pneumatic cylinder (true if retracted)
		bool p_limitHigh = true;    //Checked against the current state of the limit switch to determine state change
		SetLatch(false);
		
		
		
		//Rangefinder / moving average related info		
		float vcc = 5;               // supplied voltage to sonar sensor (LV-MaxSonar series)
		float Vi=vcc/512;            // voltage to inch ratio
		float a_range;               // the range from the sonar sensor
		
		
		
		
		float shootingdistance = 144.0f;        //desired distance for auto-alignment
		Arm  -> Set(DoubleSolenoid::kReverse);  //Automatically set pickup arm do down position
		
		
		
		
		//Moving average stuff
		int numreadings = 3;                  //The number of samples to smooth.  Helps to avoid incorrect readings due to noise or voltage spikes  
		float readings[numreadings];          //Stores the values from the rangefinder
		int index   = 0;                      //The location within the array to store the value
		float total   = 3 * shootingdistance; //The running total of the samples in the array
		float average = 0;                    //The total / the number of readings
		for (int i = 0; i < numreadings; i++) readings[i] = shootingdistance;  //This may still work if changed to 0 because of the timer which guarantees that 
																               //the array has been cycled through (meaning that the initial values don't matter)
		
		
		
		
		
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
			canFire     = limitHigh && limitLow && !isFiring ? true : false;
			a_range = range -> GetAverageVoltage()/Vi;
			
			manageArm(right, left);
			
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "L_H: %i, L_L: %i", limitHigh, limitLow); //Print the state of limit and reed switches
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "C_F: %i, I_F: %i", canFire, isFiring);   //Print whether or not can fire and if firing
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 2, "D_R: %i", dontRetract);                  //Print the state of dontRetract
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 3, "Range:  %f", a_range);                   //Show analogue range from sonar sensor
			
			
			
			
			
			//moving average range smoothing
			total -= readings[index];         //subtract the last reading
			readings[index] = a_range;        //place value from sensor
			total += readings[index];         //add the reading to the total
			index++;                          //advance to the next position in the array
			if (index>=numreadings) index=0;  //if its at the end of the array, wrap around to the beginning
			average = total / numreadings;    //calculate the average
			
			
			
			
			
			speed = -map(a_range /*average-shootingdistance*/, 0, 250, -1, 1);                   //somehow incorporate shootingdistance and average?  
			if (!button2) MecanumCartesian(-triggers, lYaxis, rXaxis /*+ lYaxis*0.025*/, 0.0f);  //Typical driving when not pressing the B button
			else if (button2) MecanumCartesian(0.0f, speed, 0 /*speed*0.025*/, 0.0f);            //Auto distance adjusting when holding the B button
			//  ^ the above inline comments are as such so as to prevent messing up something what works
			//  multiply speed by >1 to improve responsiveness when nearing the firing zone
		    
		    
					    
		    
		    
		    ////////////////////////////////////////////////////////////////////////////////////////////
			
			
			
			
			
			//Open/close latch when firing
			if (button1 && !p_button1 && canFire && !dontRetract) wait1 = time + 5.0f;
			if (wait1 > 0.0f) {
				isFiring = true;
				SetLatch (true);
				if (time > wait1) {
					isFiring = false;
					SetLatch (false);
					wait1 = 0.0f;
					wait3 = time + 5.0f;
				}
			}
			
			//Delay before resetting arm
			if (wait3 > 0) {
				waiting = true;
				if (time > wait3) {
					waiting = false;
					wait3   = 0.0f;
				}
			}		
			
			//I have no freaking idea what I was thinking when I wrote the next three lines
			if (!isFiring && !limitHigh && !dontRetract && !waiting)             ExtendReset( true);
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
			
			
			
			
			
			//Store previous states for comparison / checking state change 
			p_button1 = button1; 
			p_limitHigh = limitHigh;
			driverStationLCD->UpdateLCD();
			
		}		
	}
	
	//extend resetting pneumatic cylinders
	void ExtendReset (bool state) { 
		if (state) Retract -> Set(DoubleSolenoid::kForward);
		else       Retract -> Set(DoubleSolenoid::kReverse);
	}

	//open / close latch that released the arm
	void SetLatch (bool state) {
		if (state) Latch -> Set(DoubleSolenoid::kForward);
		else       Latch -> Set(DoubleSolenoid::kReverse);
	}
	
	//manage spinning intake wheels
	void manageArm(bool forward, bool reverse){
		if      ( forward && !reverse) pickup -> Set(1);
		else if (!forward &&  reverse) pickup -> Set(-1);
		else pickup -> Set(0);
	}
	
	//Re-maps a number from one range to another
	float map(float value, float istart, float istop, float ostart, float ostop){
		return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
	}
	
	void MecanumCartesian(float X, float Y, float Z, float theta){
		double forward, right, clockwise, cosA, sinA, temp, max;
		double front_left, front_right, rear_left, rear_right;
		forward = -Y; right = X; clockwise = Z;

		//scale values
		//clockwise *= 0.25;
		//forward   *= 0.75;
		//right     *= 1.00;
		
		//Deadzone to prevent unwanted movement
		//if ((clockwise > -0.1) && (clockwise <0.1)) clockwise = 0; 
		//if ((forward   > -0.1) && (forward   <0.1)) forward   = 0; 
		//if ((right     > -0.1) && (right     <0.1)) right     = 0; 
		
		
		//incorporate gyro if being used.  If not, this code still stays the same
		cosA = cos(theta * (3.14159 / 180.0));
		sinA = sin(theta * (3.14159 / 180.0));
		X = X * cosA - Y * sinA;
		Y = X * sinA + Y * cosA;
		
		//apply the inverse kinematic tranformation:
		front_left = forward + clockwise + right;
		front_right = forward - clockwise - right;
		rear_left = forward + clockwise - right;
		rear_right = forward - clockwise + right;
		
		//normalize so that no wheel speed command exceeds magnitude of 1:
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
		
		//command motors
		frontLeft->Set(front_left);
		frontRight->Set(-front_right);
		backLeft->Set(rear_left);
		backRight->Set(-rear_right);
	}
};

START_ROBOT_CLASS(DefaultRobot);


/*  Why is this code the way it is?  Its to compensate for the numerous exceptions that happen due to
 *  inconsistencies with the operator (MATT!!!), firing mechanism, and sensory.  Here's what's supposed to happen:
 *
 *  1) If the throwing arm is not set, extend the resetting cylinders until the arm trips the upper limit switch.
 *  2) Once the limit switch has been set, retract the cylinders until the lower reed switch is tripped.  
 *  3) If told to fire, open then close then latch.
 *  4) Goto 1      ;)
 *
 *  Why doesn't this work?  There's a lot of things that can and will go wrong.  Here's a few of the ones it has to 
 *  try to account for:
 *
 *  1) The operator trying to fire before the resetting cylinder is retracted
 *  2) The limit switch telling the cylinders to retract the instant it is tripped before the latch shuts
 *  3) Violent shaking of the arm after throwing causing the resetting cylinder to miss the arm and damage the robot
 *  4) The latch not staying open for long enough to release the arm when at low pressure
 *  5) The resetting cylinders extending after a failed arm release and pushing on the frame, wasting time and causing damage
 *  6) The solenoids failing when the voltage drops too low (this can only be practically remedied by keeping the voltage high)
 *  7) The arm taking an excessively long time to push through the latch
 * 
 *  Some of these problems can be remedied with improved hardware, some with sensor placement, and most in software.  Putting the
 *  limit switch in the ideal location (triggering exactly when the latch clicks shut and staying triggered until it is released),
 *  repositioning the arm and pneumatics (to prevent bending out the frame or causing problems in the event of a misfire), etc. 
 *  would have helped to fix a few of the issues, but they can be fixed with the programming as well.  Here are a few suggestions
 *  for solutions to the above list of problems, some of which have been addressed in the above code and some not:
 *
 *  1) Have a boolean that permits / denies the operator the ability to fire until the arm is reset
 *  2) As soon as the limit switch is tripped start a timer delaying cylinder from retracting
 *  3) Start another timer after firing to delay resetting until the arm has stopped shaking
 *  4) Open the latch for a period of time using a timer
 *  5) Check the limit switch, and only reset the arm is it has been fired
 *  6) Attempt to toggle cylinders back based on sensors
 *  7) Open the latch while resetting the arm, but not during step #2
 */
