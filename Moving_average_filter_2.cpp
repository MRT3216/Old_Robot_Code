#include <WPILib.h>
class DefaultRobot : public SimpleRobot {	

	AnalogChannel *sonar;
	
public:

	DefaultRobot(void) {	
		sonar   = new AnalogChannel(3);
	}
	
	void Autonomous(void) {
	
		const int numReadings = 10;  // the size of the array of ranges
		int readings[numReadings];   // the readings from the analogue input
		int index = 0;               // the index of the current reading
		int total = 0;               // the running total
		int average = 0;             // the average		
		float vcc = 5;               //supplied voltage to sonar sensor (LV-MaxSonar series)
		float Vi=vcc/512;            //voltage to inch ratio
		
		//Clear average array (not really necessary)
		for (int thisReading = 0; thisReading < numReadings; thisReading++){
			readings[thisReading] = 0;  
		}
		
		while(IsAutonomous()) {		
			range = sonar->GetAverageVoltage()/Vi;  // gather range from sensor
			total = total - readings[index];        // subtract the last reading
			readings[index] = range;  				// read from the sensor
			total = total + readings[index];   		// add the reading to the total
			index = index + 1;      				// advance to the next position in the array
			if (index >= numReadings) index = 0;    // if at the end of the array, wrap around to the beginning                
			average = total / numReadings;          // calculate the average			
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);

/* This programme is a moving average to smooth erratic values from a 
 * rangefinder.  Each new value from the rangefinder is added to an 
 * array of values, bumping back the previous values and continuously 
 * averaging the values.  This can be applied to other signals than 
 * the rangefinder. Be wary of the number of samples to keep track of.  
 * The higher the number, the more the readings will be smoothed, but 
 * the slower the output will respond to the input.  
 */
