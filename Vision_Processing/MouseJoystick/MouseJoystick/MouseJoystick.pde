import java.awt.AWTException;
import java.awt.Robot;
Robot robot;
float tspeed, magnitude, heading;
PVector middle, dir;
float turnsns  = 100; //100 = mormal sensitivity, 1 is high, and width/2 is low
void setup() {
  size(displayWidth, displayHeight, P2D);
  noCursor();
  smooth();
  middle = new PVector(width/2, height/2);
  try { robot = new Robot();  } 
  catch (AWTException e) { e.printStackTrace(); }
}

void draw() {
  dir = mx();
  magnitude = dir.mag();
  heading = int(degrees(dir.heading()));
  robot.mouseMove(int(middle.x), int(middle.y));
  
  if (heading==180) {
    tspeed = -(magnitude)/turnsns;
  } else if (heading == 0 && mouseX!=displayWidth/2) {
    tspeed = magnitude / turnsns;
  } else {
    tspeed = 0;
  } 
}

PVector mx() {
  int dx = constrain(mouseX - width / 2, -(int)turnsns, (int)turnsns);
  return new PVector(dx, 0);
}

boolean sketchFullScreen(){
  return true;
}
