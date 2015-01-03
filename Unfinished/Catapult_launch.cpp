float time, wait1 = 0.0f;
float button1, button2, button3;
bool canFire, isFiring;
bool limitHigh, limitLow;
while (isOperatorControl()){
	time = Timer::GetPPCTimestamp();
	button1 = xBox -> GetRawButton(1);
	button2 = xBox -> GetRawButton(2);
	button3 = xBox -> GetRawButton(3);
	canFire = limitHigh && limitLow && !isFiring ? true : false;
	if (button1 && canFire) wait1 = time + 0.25f;
	if (wait1 > 0.0f){
		isFiring = true;
		SetLatch (true);
		if (time > wait1){
			isFiring = false;
			SetLatch (false);
			wait1 = 0.0f;
		}
	}
	
	if (!isFiring && (!limitHigh ||  button2)) ExtendReset( true);
	if (!isFiring &&   limitHigh && !button2 ) ExtendReset(false);
	
}

void ExtendReset (bool state) { 
	leftSolenoidIn   -> Set ( state);
	leftSolenoidOut  -> Set (!state);
	rightSolenoidIn  -> Set ( state);
	rightSolenoidOut -> Set (!state);
}

void SetLatch (bool state) { 
	LatchSolenoidIn  -> Set ( state);
	LatchSolenoidOut -> Set (!state);
}