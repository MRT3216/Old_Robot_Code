/* The purpose of this file to is to provide examples and show solutions to problems
 * relating to time and cyclic functions.  This includes state machines.  The largest
 * benefit of using state machines and cyclic timers is that they can run concurrently,
 * helping to maintain the speed and flow of information within the code.
 */



	Timer::GetPPCTimestamp();                  // Option #1 - measured in seconds
	GetFPGATime()/1000                         // Option #2 - measured in milliseconds
	
 
 
/* First of all: acquiring the time.  There are two ways to get the real time from the
 * cRIO's internal clock, both of which are shown above.  The following two methods have 
 * different ways that they're set up to ensure functionality.  For both, its good 
 * practice to initialize a variable with its initial value and subtract it from the 
 * current value in case it doesn't initialize to 0 for some reason (it does happen 
 * sometimes).  Below are sample programs to demonstrate how to use them:
 */
 
 

 
	// Option #1
	#include <WPILib.h>
	class DefaultRobot : public SimpleRobot {    
		public:
		DefaultRobot(void){ } 	
		void OperatorControl(void){		
			float itime = Timer::GetPPCTimestamp(), ttime;		
			while (IsOperatorControl()){
				ttime = Timer::GetPPCTimestamp() - itime; //time since entering teleop
			}
		}
	};
	START_ROBOT_CLASS(DefaultRobot);
	 
	 

	 
	//Option #2
	#include <WPILib.h>
	#define GetMsecTime() (GetFPGATime()/1000)
	class DefaultRobot : public SimpleRobot {    
		public:
		DefaultRobot(void) { } 	
		void OperatorControl(void) {		
			float itime = GetMsecTime(), ttime;		
			while (IsOperatorControl()) {
				ttime = GetMsecTime() - itime; //time since entering teleop
			}
		}
	};
	START_ROBOT_CLASS(DefaultRobot);


	

/* Either will be sufficient, but the periodic loops only cycle at 50Hz (50 times per second),
 * so the code will update in increments of 20 milliseconds regardless of how precise you
 * want to be.
 *
 * The following snippet of code is a state machine - an efficient method of using time to perform 
 * actions without showing down the execution of the code (or stopping it by using Wait()
 * statements).  This can be done using either type of timer described above, but keep in mind
 * that they use different units - seconds and milliseconds - so set your values accordingly.  
 */
 
 


	#include <WPILib.h>
	class DefaultRobot : public SimpleRobot {    
		public:
		DefaultRobot(void){ } 	
		void OperatorControl(void){	
			bool condition = true;
			float itime    = Timer::GetPPCTimestamp();  //baseline time when entering teleop
			float ttime    = 0;                         //total time since entering teleop
			float wait     = 0;                         //the period of time for which to wait
			while (IsOperatorControl()){
				ttime = Timer::GetPPCTimestamp() - itime;               //or GetMsecTime() 
				if (condition) wait = ttime + 5.0f; condition = false;  //initiate an action to perform for 5 seconds
				if (wait > 0.0f) {
					//code here to perform an action for 5 seconds
					if (ttime > wait) {
						//code here to stop / change the action
						wait = 0;	
					}
				}
			}
		}
	};
	START_ROBOT_CLASS(DefaultRobot);




/* Sometimes there are times when you have to kill a state machine before its finished executing.
 * This is especially important when there are multiple state machines working together or if
 * the environment is dynamically changing (for instance, the state of a sensor could potentially
 * disable a state machine to perform a different action).  There's a few compact ways of making
 * state machines that stop at different times before being fully disabled.  The most common one
 * kills it before executing any of its internal commands.  
 */
 
 
 
	ttime = Timer::GetPPCTimestamp() - itime; //or GetMsecTime() - itime
	if (condition) wait = ttime + 5.0f;       //initiate an action to perform for 5 seconds
	if (wait > 0.0f) {
		if (exception_condition) wait = 0;
		else /*code here to perform an action*/ ;	
		if (ttime > wait && wait != 0) {
			//stop / change the action	
			wait = 0;	
		}
	}



/* Below are two state machines that die at different points.  The first allows the first part of
 * the command to execute, skipping the second, and the second only allows the second half to 
 * execute, skipping the first.  These can come in handy when doing tasks that only need to be
 * called once, such as when toggling a solenoid, where duration isn't as important as timing.
 */
 
 
 
 
	ttime = Timer::GetPPCTimestamp() - itime; //or GetMsecTime() - itime
	if (condition) wait = ttime + 5.0f;       //initiate an action to perform for 5 seconds
	if (wait > 0.0f) {
		if (exception_condition) wait = 0;	
		//perform an action
		if (ttime > wait && wait != 0) {
			//stop / change the action
			wait = 0;	
		}
	}
	
	
	
	
	ttime = Timer::GetPPCTimestamp() - itime; //or GetMsecTime() - itime
	if (condition) wait = ttime + 5.0f;       //initiate an action to perform for 5 seconds
	if (wait > 0.0f) {
		if (exception_condition) wait = 0;
		else /*perform an action*/ ;	
		if (time > wait) {
			//stop / change the action
			wait = 0;	
		}
	}
	
	
	
	
/* The following is a consecutively executing chain of state machines, each activating the next and
 * looping back to the beginning.  For the sake of simplicity, these state machines are not being
 * run concurrently, nor are they capable of being interrupted.  Its easy enough, though, to modify
 * this code to allow for that.
 */

	
	
	
	ttime = Timer::GetPPCTimestamp() - itime; //or GetMsecTime() - itime
	if (condition) wait = ttime + 5.0f;       //initiate an action to perform for 5 seconds
	if (wait > 0.0f) {
		//code here to perform an action
		if (ttime > wait) {
			//stop / change the action	
			wait  = 0;	
			wait2 = ttime + 5.0f;
		}
	}
	
	if (wait2 > 0.0f) {
		//code here to perform an action
		if (ttime > wait2) {
			//stop / change the action	
			wait2  = 0;	
			wait3 = ttime + 6.0f;
		}
	}
	
	if (wait3 > 0.0f) {
		//code here to perform an action
		if (ttime > wait3) {
			//stop / change the action	
			wait3  = 0;	
			wait4 = ttime + 10.0f;
		}
	}
	
	if (wait4 > 0.0f) {
		//code here to perform an action
		if (ttime > wait4) {
			//stop / change the action	
			wait4  = 0;	
			wait = ttime + 5.0f; //remove this to simply run through the series once every time the first one is initiated
		}
	}
	
	
	
	
/* State machines aren't the only way of keeping track of time.  Many embedded systems without the capability
 * of measuring time use loop counters to determine the time.  It's not quite as accurate because certain things
 * within the code can cause it to hang at certain points, but it can be quite useful for coarsely maintained
 * tasks, things that need to execute at a fairly regular interval, and things that that need to execute for
 * a specific number of cycles.  You can also infer the elapsed time by multiplying the loop count by the refresh
 * rate (20 milliseconds for 50Hz). Below are three examples of loop counters.  The first executes a command once
 * per cycle for the first # cycles.  The second executes only once ever (useful for printing information only 
 * at the instant of entering a loop or querying one-time things), and the third executes a command every # cycles 
 * for the life of the program.  They're all extremely simple and relatively easy to figure out, but sometimes in
 * the heat of competition these kind of things escape you, so its nice to have a reference
 */

 
 
	#include <WPILib.h>
	class DefaultRobot : public SimpleRobot {    
		public:
		DefaultRobot(void){ } 	
		void OperatorControl(void){
			int loopcount  = 0;			
			while (IsOperatorControl()){
				if (loopcount < 10){
					//code here to perform an action for the first ten cycles
					loopcount++;
				}
			}
		}
	};
	START_ROBOT_CLASS(DefaultRobot);
	
	
	
	
	#include <WPILib.h>
	class DefaultRobot : public SimpleRobot {    
		public:
		DefaultRobot(void){ } 	
		void OperatorControl(void){
			bool condition = true;			
			while (IsOperatorControl()){
				if (condition){
					//code here to perform an action only once
					condition = false;
				}
			}
		}
	};
	START_ROBOT_CLASS(DefaultRobot);


	
	
	#include <WPILib.h>
	class DefaultRobot : public SimpleRobot {    
		public:
		DefaultRobot(void){ } 	
		void OperatorControl(void){
			int loopcount  = 0;			
			while (IsOperatorControl()){
				if (loopcount % 100 == 0){
					//code here to perform an action every 100 cycles (every 2 seconds at 50Hz)
				}
				loopcount++;
			}
		}
	};
	START_ROBOT_CLASS(DefaultRobot);