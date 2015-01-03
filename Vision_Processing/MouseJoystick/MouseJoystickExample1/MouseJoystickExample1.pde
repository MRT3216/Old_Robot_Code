import java.awt.AWTException;
import java.awt.Robot;
Robot robot;
float tspeed;
PVector middle;

float turnsns  = 100; //100 = mormal sensitivity, 1 is high, and width/2 is low


void setup() {
  size(displayWidth, displayHeight, P2D);
  textAlign(LEFT, TOP);
  textSize(30);
  strokeWeight(3);
  noCursor();
  smooth();
  middle = new PVector(width/2, height/2);
  try { robot = new Robot();  } 
  catch (AWTException e) { e.printStackTrace(); }
}

void draw() {
  PVector dir = mx();
  float magnitude = dir.mag();
  background(255);
  fill(0);
  String info = "Degrees: " + int(degrees(dir.heading())) + "\nMagnitude: " + int(magnitude);
  text(info, 5, 5);
  translate(middle.x, middle.y);
  line(0, 0, dir.x, dir.y);
  robot.mouseMove(int(middle.x), int(middle.y));
  float heading = int(degrees(dir.heading()));
  
  if (heading==180) {
    tspeed = -(magnitude)/turnsns;
  } else if (heading == 0 && mouseX!=displayWidth/2) {
    tspeed = magnitude / turnsns;
  } else {
    tspeed = 0;
  }
  
  text("Speed:"+tspeed, 100, 100);
}

PVector mx() {
  int dx = constrain(mouseX - width / 2, -(int)turnsns, (int)turnsns);
  return new PVector(dx, 0);
}

boolean sketchFullScreen() {
  return true;
}
