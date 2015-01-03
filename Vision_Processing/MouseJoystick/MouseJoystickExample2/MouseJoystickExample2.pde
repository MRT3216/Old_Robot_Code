import java.awt.AWTException;
import java.awt.Robot;
Robot robot;
float tspeed, magnitude, heading, b;
PVector middle, dir;
float turnsns  = 100; //100 = mormal sensitivity, 1 = high, and width/2 = low
void setup() {
  size(displayWidth, displayHeight, P2D);
  noCursor();
  smooth();
  fill(255);
  rectMode(CENTER);
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
  
  background(102);
  pushMatrix();
  translate(width/2, height/2);
  rotate(b);  
  rect(0, 0, 50, 50);
  b+=tspeed/3;
  popMatrix();
  
}

PVector mx() {
  int dx = constrain(mouseX - width / 2, -(int)turnsns, (int)turnsns);
  return new PVector(dx, 0);
}

boolean sketchFullScreen(){
  return true;
}

