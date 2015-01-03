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
		loaded     = new DigitalInput(2); //limit switch
		Retract    = new DigitalInput(3); //reed  switch
		compressor = new Compressor(1, 1); //1 is the Digital I/O, and 2 is the Relay port
		compressor->Start();
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 		
		float time, wait1 = 0.0f;
		float button1, button2;
		bool canFire, isFiring;
		bool limitHigh, limitLow;
		Latch   -> Set(DoubleSolenoid::kForward);
		if      (!limitHigh) ExtendReset( true);
		else if ( limitHigh) ExtendReset(false);	
		
		while (IsOperatorControl()){			
			time = Timer::GetPPCTimestamp();
			button1   = xBox   -> GetRawButton(1); //Button A
			button2   = xBox   -> GetRawButton(2); //Button B
			limitHigh = loaded  -> Get();
			limitLow  = Retract -> Get();			
			canFire = limitHigh && limitLow && !isFiring ? true : false;
			if (button1 && canFire) wait1 = time + 0.25f;
			if (wait1 > 0.0f){
				isFiring = true;
				SetLatch (true);
				if (time > wait1){
					isFiring = false;
					SetLatch (false);
					wait1 = 0.0f;
				}
			}
			
			if (!isFiring && (!limitHigh ||  button2)) ExtendReset( true);
			if (!isFiring &&   limitHigh && !button2 ) ExtendReset(false);			
		}		
	}
	
	void ExtendReset (bool state) { 
		if (!state) Retract -> Set(DoubleSolenoid::kForward);
		else        Retract -> Set(DoubleSolenoid::kReverse);
	}

	void SetLatch (bool state) {
		if (state) Latch -> Set(DoubleSolenoid::kForward);
		else       Latch -> Set(DoubleSolenoid::kReverse);
	}
};

START_ROBOT_CLASS(DefaultRobot);
