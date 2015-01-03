//a slew rate limiter limits acceleration.  It can be applied to a joystick (like below) to ramp motor speed

change = joystick - limitedJoystick;
if (change>limit) change = limit;
else (if change<-limit) change = -limit;
limitedJoystick += change;


/* 'limit' is the amount of change you will allow every iteration (e.g. every 20 milliseconds for TeleOp).
 * 'limitedJoystick' is the rate-limited (processed) joystick value you use to control your motors. I.e.
 * motor->Set(limitedJoystick) somewhere in your code to utilize this.
 */
 
 
 /* To choose a good value for Command Limit, consider how long you want to take to accelerate AND that this 
  * code will execute ~50 times a second.  As an example, lets say that my robot can go from standstill to full 
  * forward in 2 seconds without slipping.  Since you want to go from 0 to 1 in {50/second * 2 seconds = 100 
  * iterations, 1/100 = 0.01 might be a good place to start.  Tweak it a bit to figure out what works.  Double
  * check the speed I mentioned, too.  The default may be 30Hz, not 50Hz
  */