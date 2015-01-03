#include <WPILib.h>
class DefaultRobot : public SimpleRobot {	

	Compressor *compressorPointer;
	
public:

	DefaultRobot(void) {	
		compressorPointer = new Compressor(1, 4); //1 is the Digital I/O, and 4 is the Relay port
		compressorPointer->Start();
	}
	
	void Autonomous(void) {
		while(IsAutonomous()){
		}				
	}
	
	void OperatorControl(void) {
		while (IsOperatorControl()) {	
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);
