#include <WPILib.h>
#define NUMSHOTS 3
class DefaultRobot : public SimpleRobot {
	Joystick *xBox;	
	Relay *valves[NUMSHOTS];                       
	Compressor *comp;                     
	Victor *rise;                          
	Jaguar *right;
	Jaguar *left;
	RobotDrive *myRobot;
    
	public:

	DefaultRobot(void){
		xBox      = new Joystick(1);	
		valves[0] = new Relay(1);
		valves[1] = new Relay(2);
		valves[2] = new Relay(3);
		comp      = new Compressor(4, 1);
		rise      = new Victor(1);
		right     = new Jaguar(2);
		left      = new Jaguar(3);
		myRobot   = new RobotDrive(right, left);
		
		comp -> Start();
	}
	
	void Autonomous(void){
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void){
		DriverStationLCD *driverStationLCD = DriverStationLCD::GetInstance();
		float lYaxis = 0, rYaxis = 0, time, wait1 = 0;
		int index = 0;
		int lBumper, rBumper;
		bool button1, button2, button3, firing = false;
		bool p_button1 = true, p_button2 = true, p_button3 = true;
		bool canFire[] = {false, false, false}; //left, middle, right
		
		while (IsOperatorControl()){			
			time = Timer::GetPPCTimestamp();
			button1 = xBox -> GetRawButton(1); //file the next shot
			button2 = xBox -> GetRawButton(2); //set all canFire to false
			button3 = xBox -> GetRawButton(3); //set all canFire to true
			rBumper = xBox -> GetRawButton(5); //lower canons                  
			lBumper = xBox -> GetRawButton(6); //raise canons
			lYaxis  = xBox -> GetRawAxis(2);
			rYaxis  = xBox -> GetRawAxis(5);
			rise -> Set(rBumper - lBumper);
			myRobot->TankDrive(-lYaxis, -rYaxis);
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 0, "Can fire:");
			driverStationLCD->PrintfLine((DriverStationLCD::Line) 1,
					"|| L: %i || M: %i || R: %i ||", canFire[0], canFire[1], canFire[2]);
			driverStationLCD->UpdateLCD();
			
			if (button2 && !p_button2){
				canFire[0] = false;
				canFire[1] = false;
				canFire[2] = false;
			}
			
			if (button3 && !p_button3){
				canFire[0] = true;
				canFire[1] = true;
				canFire[2] = true;
			}
			
			if (button1 && !p_button1){
				if (!firing && (canFire[0] || canFire[1] || canFire[2])){
					//I may need something in here to double check -> avoid infinite loop...
					if (!canFire[index]){
						while (canFire[index] == false){
							index = index < 2 ? index + 1 : 0;
							// index = index < 2 ? index + 1 : 0;
						    // if (!canFire[index]) index = index < 2 ? index + 1 : 0;
						    // if (!canFire[index]) index = index < 2 ? index + 1 : 0;
						}
					}
					wait1  = time + 2;
					firing = true;
				}
			}
			
			if (wait1 > 0){
				valves[index] -> Set(Relay::kOn);
				if (time > wait1){
					valves[index]  -> Set(Relay::kOff);					
					canFire[index] =  false;					
					firing         =  false;
					wait1          =  0.0f;
				}
			}
			
			p_button1 = button1;
			p_button2 = button2;
			p_button3 = button3;
		}		
	}
};

START_ROBOT_CLASS(DefaultRobot);
