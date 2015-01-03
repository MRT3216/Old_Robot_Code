/* This should be the simplest programme in the list.  Just to
 * save you a few minutes coming up with this, here it is.  This
 * snippet detects the transition between deliberate rotation
 * and unintentional rotation (in this case.  It could be applied
 * to lots of things).  It's used for resetting the gyro
 * in one of the programmes further down the list.  The whole 
 * operation is done inside of a continuous loop (probably 
 * isOperatorEnabled()).  Initialize ptVal to 0. 
 */
 
 
 
 //initialize ptVal to be 0.
 tVal = xBox -> GetRawAxis(4);
 if (ptVal != tVal && tVal == 0){
	//Do something 
	//like reset
	//the gyro.
 }
 ptVal = tVal;