/* stripped down code to debug compressor. Relay works
 * but the sensor may not...
 */
#include <WPILib.h>
class DefaultRobot : public SimpleRobot {
	Joystick *xBox;	
	DoubleSolenoid *Latch;
	//Compressor *compressor;
    //Relay *relay;
	public:

	DefaultRobot(void){
		xBox       = new Joystick(1);
		Latch      = new DoubleSolenoid(1, 2);  //(forward, reverse)
		//compressor = new Compressor(1, 1); //1 is the Digital I/O, and 2 is the Relay port
		//compressor->Start();
		//relay = new Relay(1);
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){ 		
		float time, wait1 = 0.0f;
		float button1, p_button1 = false;
		bool canFire, isFiring = false;
		Latch   -> Set(DoubleSolenoid::kForward);
		//relay -> Set(Relay::kOn);
		//compressor->Start();
		
		while (IsOperatorControl()){			
			time = Timer::GetPPCTimestamp();
			button1 = xBox -> GetRawButton(1); //Button A	
			canFire = !isFiring ? true : false;
			if (button1 != p_button1 && button1 && canFire) wait1 = time + 0.25f;
			if (wait1 > 0.0f){
				isFiring = true;
				SetLatch (true);
				if (time > wait1){
					isFiring = false;
					SetLatch (false);
					wait1 = 0.0f;
				}
			}	
			
			p_button1 = button1;
		}		
	}

	void SetLatch (bool state) {
		if (state) Latch -> Set(DoubleSolenoid::kForward);
		else       Latch -> Set(DoubleSolenoid::kReverse);
	}
};

START_ROBOT_CLASS(DefaultRobot);
