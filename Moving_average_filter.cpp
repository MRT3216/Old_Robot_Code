#include <WPILib.h>
class DefaultRobot : public SimpleRobot {	

	AnalogChannel *sonar;
	
public:

	DefaultRobot(void) {	
		sonar   = new AnalogChannel(3);
	}
	
	void Autonomous(void) {
		int sampleSize = 30;
		float average;// = 0;
		float distance[sampleSize];
		
		float vcc = 5;  //supplied voltage to sonar sensor (LV-MaxSonar series)
		float Vi=vcc/512; //voltage to inch ratio
		
		while(IsAutonomous()) {
		
/////////////////////////////Moving Average////////////////////
			for (int i = sampleSize - 1; i > 0; i--) {
				distance[i] = distance[i - 1];
			}
			distance[0] = (sonar->GetAverageVoltage()/Vi);    			
			average = 0;      
			for (int i = 0; i < sampleSize; i++) {
				average += distance[i];
			}      
			average /= sampleSize;
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);

/* This programme is a moving average we worked on last year to smooth
 * the erratic values from a rangefinder.  Each new value from the
 * rangefinder is added to an array of values, bumping back the previous
 * values and continuously averaging the values.  This can be applied
 * to other signals than the rangefinder.  Moving_average_filter_2 is a
 * MUCH simpler and faster version of the same programme.
 */
