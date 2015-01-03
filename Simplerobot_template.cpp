#include <WPILib.h> //required for everything

/* imports go here.  Math.h, iostream, and fstream are common.
 * Importing libraries can give you a vast range of functions
 * without having to write them yourself.  The math library has
 * commonly used functions like abs(), fabs(), sin(), cos(), etc.
 */

class DefaultRobot : public SimpleRobot {
	
	/* Make pointers to undeclared objects here by way of 
	 * 'Talon *frontLeft;' and then declaring it below in 
	 * DefaultRobot
	 */
	
	public:
	
	DefaultRobot(void){ //The constructor for your main class
		
		//MUST BE DECLARED IN THE ORDER THEY WERE INITIALIZED (above)
		
		/* Declare objects here by way of 'frontLeft = new Talon(1,1);'
		 * where frontleft is the name of the undeclared talon object from 
		 * earlier in the code and (1,1) is the module on the cRIO and 
		 * sidecar PWM port into which it is plugged.  Other things don't
		 * use the same ports or the same convention.  On a 4-slot cRIO,
		 * you don't need to declare the module number (because there is
		 * only one of each).  Analogue inputs, Analogue outputs, Digital
		 * inputs, Digital outputs, relays, compressors, and solenoids 
		 * all have their own unique physical location into which they 
		 * are plugged (varying locations on the digital sidecar or 
		 * breakout boards).
		 */
		
	}
	
	//new methods (such as  drive methods) can go here
	
	void Autonomous(void){
	
		/* Things written here execute once at the beginning of Autonomous
		 * mode.  Things such as declaring variables, resetting the gyro, 
		 * opening files, and running initial checks go here.
		 */
		 
		while(IsAutonomous()){
		
			/* This is where all of your autonomous code goes.  Stuff placed
			 * in here will be executed by the robot alone without operator
			 * interference.
			 */
		
		}				
	}
	
	//new methods (such as  drive methods) can go here
	
	void OperatorControl(void){ 
	
		/* Things written here execute once at the beginning of Teleop
		 * mode.  Things such as declaring variables, resetting the gyro, 
		 * opening files, stating threads, and running initial checks go 
		 * here.
		 */
	
		while (IsOperatorControl()){
	
			/* This is where the majority of your code (that isn't
			 * contained by methods) will end up.  Teleop is typiclly the 
			 * longest portion of the game in which the driver tells the 
			 * robot to do what it was built for.  This is where drive 
			 * code, GUI integration, manually operated tasks, and 
			 * autonomous subroutines go.
			 */
	
		}
	}
	
	//new methods (such as  drive methods) can go here
	
};

START_ROBOT_CLASS(DefaultRobot);  /* Replace DefaultRobot with the name of
                                   * your project / main programme.  Typically
								   * a new programme is created by going to
								   * File->New->Example->VxWorks Downloadable
								   * Kernel Module Sample Project->FRC Default
								   * Program (with current imaging tool), which
								   * is where DefaultRobot comes from.  You have
								   * to change the name of the *.out file to be
								   * sent to the cRIO if you change the name (so
								   * don't)
								   */
