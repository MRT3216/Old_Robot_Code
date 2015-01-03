/* The Iterative Robot base class assists with the most common 
 * code structure by handling the state transitions and looping 
 * in the base class instead of in the robot code. For each 
 * state (autonomous, teleop, disabled, test) there are two 
 * methods that are called:
 *
 * Init methods - The init method for a given state is called 
 * each time the corresponding state is entered (for example, a 
 * transition from disabled to teleop will call teleopInit()). 
 * Any initialization code or resetting of variables between 
 * modes should be placed here.
 *
 * Periodic methods - The periodic method for a given state is 
 * called each time the robot receives a Driver Station packet 
 * in the corresponding state, approximately every 20ms. This 
 * means that all of the code placed in each periodic method 
 * should finish executing in 20ms or less. The idea is to put 
 * code here that gets values from the driver station and updates 
 * the motors. You can read the joysticks and other driverstation 
 * inputs more often, but you’ll only get the previous value until 
 * a new update is received. By synchronizing with the received 
 * updates your program will put less of a load on the cRIO CPU 
 * leaving more time for other tasks such as camera processing.
 *
 * It also has a test method, just like SimpleRobot.  I have no
 * idea what it does, and it is disabled during competition.
 */
 
#include "WPILib.h"

/* imports go here.  Math.h, iostream, and fstream are common.
 * Importing libraries can give you a vast range of functions
 * without having to write them yourself.  The math library has
 * commonly used functions like abs(), fabs(), sin(), cos(), etc.
 */
 
class DefaultRobot : public IterativeRobot
{
	/* Make pointers to undeclared objects here by way of 
	 * 'Talon *frontLeft;' and then declaring it below in 
	 * DefaultRobot
	 */

public:

	DefaultRobot(void){  //The constructor for your main class
	
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
	
	void RobotInit(void){
	
		/* Called when the robot is first turned on.  This is a substitute
	     * for using the constructor.  This is only called once.
		 */
	
	}
	
	//new methods (such as  drive methods) can go here
	
	// ============= Autonomous Functions =====================

	void AutonomousInit(void){
	
		/* Called when the robot enter autonomous for the first time.
		 * This is called on a transition from any other state.  Things 
		 * written here execute once at the beginning of Autonomous mode.  
		 * Things such as declaring variables, resetting the gyro, opening 
		 * files, starting threads, and running initial checks go here.
		 */
		 
	}

	void AutonomousPeriodic(void){
	
		/* Called periodically during autonomous based on a periodic timer 
		 * for the class.  This is where all of your autonomous code goes.  
		 * Stuff placed in here will be executed by the robot alone without 
		 * operator interference.
		 */
	
	}

	//new methods (such as  drive methods) can go here
	
	// =================== Teleop Functions =====================

	void TeleopInit(void){
	
		/* Called when the robot enter teleop for the first time. This is called 
		 * on a transition from any other state.  Things written here execute once 
		 * at the beginning of Teleop mode.  Things such as declaring variables, 
		 * resetting the gyro, opening files, and running initial checks go here.
		 */
		 
	}
	
	void TeleopPeriodic(void){		
	
		/* Called periodically during teleop based on a periodic timer for the class.  
		 * Things written here execute once at the beginning of Teleop mode.  Things 
		 * such as declaring variables, resetting the gyro, opening files, zeroing loop 
		 * counters,and running initial checks go here.
		 */
	 
	}

	//new methods (such as  drive methods) can go here
	
	// ************* Disabled Functions ******************
	
	void DisabledInit(void)	{
	
		/* Called when the robot is disabled for the first time. Things written 
		 * here execute only once upon the transition.  Things such as closing
		 * threads,  zeroing loop counters, closing files, shutting down 
		 * coprocessors and proclaiming that the robot is disabled to the 
		 * driverstation go here.
		 */
	}
	
	void DisabledPeriodic(void) {
	
		/* There's not many productive things you can do while disabled... maybe
		 * gather some baseline sensor readings or send some calibration info
		 * to the camera or set parameters on the fly (such as a "disable
		 * autonomous" function).
		 */
		 
	}
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

/* IterativeRobot has a particular
 * way with time.  Because the loops
 * are executed at such a specific
 * interval, loop counters can be 
 * employed to keep track of time
 * and reduce the use of more 
 * complicated state machines.
 * Typically, in the init methods,
 * you will reset your loop counting
 * variable to 0, and then increment
 * it during every loop of the 
 * periodic methods.  You can then
 * manage your time (keeping in mind
 * that the periodic loops iterate
 * every 20ms or at 50Hz), you could
 * time actions by way of 
 * if(loopcounter%10){//do something}
 * wherein loopcounter is your
 * incrementing value and 10 is the
 * frequency (number of 20ms loops
 * you want to wait between your
 * actions).
 */


