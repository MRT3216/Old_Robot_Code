#include <WPILib.h>
#include <Math.h>
class DefaultRobot : public SimpleRobot {	
	AnalogChannel *range;
	
	public:

	DefaultRobot(void) {	
		left = new AnalogChannel(1,1);
	}
	
	void Autonomous(void) {
		while(IsAutonomous()) {
		}				
	}
	
	void OperatorControl(void) {		
		float vcc = 5;     //supplied voltage to sonar sensor (XL-MaxSonar Long Range series)
		float Vi=vcc/512;  //voltage to centimeter conversion
		while (IsOperatorControl())	{			
			distance = range->GetAverageVoltage()/Vi;
		}
	}
};

START_ROBOT_CLASS(DefaultRobot);

/* Simple test of an analogue rangefinder.  The equations for the main models
 * (whose equations are shown below) can be found on the Maxbotix website if you
 * need to check which you need.  
 *
 * LV-MaxSonar-EZ, LV-MaxSonar-WR, and LV-MaxSonar-WRC. All LV-MaxSonar 
 * sensors range to targets in inches. The formula for the voltage scaling on an 
 * LV-MaxSonar sensor family is: 
 *
 *       [(Vcc/512) = Vi]
 *      Vcc = Supplied Voltage
 *     V2cm = V per inch
 *
 * This is for the XL MaxSonar EZ, XL-MaxSonar-AE, XL MaxSonar WR, and XL MaxSonar 
 * WRC:
 *
 *       [(Vcc/1024) = Vcm]
 *      Vcc = Supplied Voltage
 *     V2cm = V per  cm
 *
 * For the 10meter long range sensors (XL-MaxSonar-WRL1/WRLA1, XL-MaxSonar-EZL0/AEL0, and 
 * XL-MaxSonar-EZL1/AEL1.) the calculation formula is:
 *
 *    [(Vm/Vcc/1024)=U] * 2 = cm
 *       Vm = Volts measured
 *     V2cm = V per every 2 cm
 *        U = half measurement
 */
