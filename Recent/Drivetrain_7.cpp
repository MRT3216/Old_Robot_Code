#include <WPILib.h>
class DefaultRobot : public SimpleRobot {
	Joystick *xBox;	
	DoubleSolenoid *Latch;
	DoubleSolenoid *Retract;
	DigitalInput *loaded;          
    DigitalInput *retracted;
	Compressor *compressor;
    
	public:

	DefaultRobot(void){
		xBox       = new Joystick(1);
		Latch      = new DoubleSolenoid(1, 2);  //(forward, reverse) *
		Retract    = new DoubleSolenoid(3, 4);  //(forward, reverse)
		loaded     = new DigitalInput(2);       //limit switch
		retracted  = new DigitalInput(3); //reed  switch
		compressor = new Compressor(1, 1); //1 is the Digital I/O, and 2 is the Relay port
		compressor->Start();
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 		
		float time, wait1 = 0.0f, wait2 = 0.0f;
		bool button1, p_button1 = false, button2;
		bool canFire, isFiring = false, dontRetract = false;
		bool limitHigh, limitLow, p_limitHigh = false;
		Latch   -> Set(DoubleSolenoid::kReverse);	
		
		while (IsOperatorControl()){	
			DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
			time = Timer::GetPPCTimestamp();
			button1   = xBox      -> GetRawButton(1); //Button A
			button2   = xBox      -> GetRawButton(2); //Button B
			limitHigh = !loaded    -> Get();
			limitLow  = !retracted -> Get();
			canFire = limitHigh && limitLow && !isFiring ? true : false;
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Limit switch: %i", limitHigh);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1, "Reed  switch: %i", limitLow );
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 2, "Can fire:     %i", canFire  );
			
			if (button1 && !p_button1 && canFire && !dontRetract) wait1 = time + 0.25f;
			if (wait1 > 0.0f){
				isFiring = true;
				SetLatch (true);
				if (time > wait1){
					isFiring = false;
					SetLatch (false);
					wait1 = 0.0f;
				}
			}
			
			/* The next few commented out areas are a preventative measure to
			 * stop the arm from jittering up and down as it *almost* latches
			 * shut.  What the following does is start a state machine to force
			 * the arm to continue extending even after the limit switch has
			 * been pressed for (presently) 5 seconds.  It is commented out
			 * because Gary and Harmony refuse to let me try it out on the 
			 * robot while they're adjusting the arm.  Apparently I can 
			 * program at home...  :(
			 */
			
			if (!isFiring && (!limitHigh ||  button2) /*&& !dontRetract*/) ExtendReset( true);  //change so that it can't break the frame
			if (!isFiring &&   limitHigh && !button2  /*&& !dontRetract*/) ExtendReset(false);	
			
			/*if (limitHigh && !p_limitHigh) wait2 = time + 5.0f;
			if (wait2 > 0){
				dontRetract = true;
				ExtendReset(true);
				if (time > wait2){
					wait2 = 0;
					dontRetract = false;
				}
			}*/
			
			p_button1 = button1; 
			p_limitHigh = limitHigh;
			driverStationLCD->UpdateLCD();
			
		}		
	}
	
	void ExtendReset (bool state) { 
		if (state) Retract -> Set(DoubleSolenoid::kForward);
		else        Retract -> Set(DoubleSolenoid::kReverse);
	}

	void SetLatch (bool state) {
		if (state) Latch -> Set(DoubleSolenoid::kForward);
		else       Latch -> Set(DoubleSolenoid::kReverse);
	}
};

START_ROBOT_CLASS(DefaultRobot);
