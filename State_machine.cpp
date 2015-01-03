// declare the timeout variables at the top of your Robot.cpp
static double waitForwardTimeout = 0.0;
static double waitReverseTimeout = 0.0;

...

if (waitForwardTimeout > 0.0) {
    if (Timer::GetPPCTimestamp() >= waitForwardTimeout) {
        waitForwardTimeout = 0.0;
        // DO SOMETHING AFTER WAITING FOR FORWARD TIMEOUT
        Forward->Set(0);
        Reverse->Set(1);
        waitReverseTimeout = Timer::GetPPCTimestamp + 1.5;
    }
}

else if (waitReverseTimeout > 0.0) {
    if (Timer::GetPPCTimestamp() >= waitReverseTimeout) {
        waitReverseTimeout = 0.0;
        // DO SOMETHING AFTER WAITING FOR REVERSE TIMEOUT
    }
}

else if (LeftBumper && RightBumper) {  // xBox controller
    front->Set(-speed); // Talons
    back->Set(speed);
    if (ButtonA) {
        Forward->Set(1); // solenoids
        Reverse->Set(0);
        waitForwardTimeout = Timer::GetPPCTimestamp + 0.1;
    }
}


/* This is the code we used to spin up our firing wheels and shoot frisbees
 * last year during the competition.  This is absolutely the most important
 * thing you can lern to do when programming your robot.  THe alternative 
 * to using timestamp is to use Wait(milis), if used properly, isn't problematic,
 * but causes the entirety of your code to freeze without possiblity of resuming
 * until the desired number of millis runs out.  This means that if you call
 * Wait while driving, your robot will continue driving and ignore your input
 * until the Wait time runs out.  It's usually fine in autonomous.  There is other 
 * alternatives:  run the code that uses Wait statements in a separate thread, or
 * incriment a counter (from which you can determine the time by dividing by the Hz)
 */