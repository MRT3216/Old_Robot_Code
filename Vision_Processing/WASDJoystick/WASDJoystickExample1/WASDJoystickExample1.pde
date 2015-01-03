float x_Axis, y_Axis;
float w,a,s,d;
float fullspeed = 1.00f;  // "sprint speed"
float regspeed  = 0.75f;  // "non-sprint speed"
float val       = regspeed;
PVector pos = new PVector(500, 300);
void setup() {
  size(displayWidth, displayHeight);
  frameRate(60);
  noCursor();
}

void draw() {
  x_Axis = (a+d)*val;
  y_Axis = (s+w)*val;
  pos.x+=x_Axis*10;
  pos.y-=y_Axis*10;
  
  stroke(255);
  fill(255);
  background(0);
  rectMode(CENTER);
  rect(pos.x, pos.y, 10, 10);
  wrap(pos); 
}

void keyPressed() {
  if (key == 'a' || key == 'A' || keyCode == LEFT ) { a = -1.0f; }
  if (key == 'd' || key == 'D' || keyCode == RIGHT) { d =  1.0f; }
  if (key == 's' || key == 'S' || keyCode == DOWN ) { s = -1.0f; }
  if (key == 'w' || key == 'W' || keyCode == UP   ) { w =  1.0f; }
  if (keyCode ==  SHIFT) { val = fullspeed; }
}

void keyReleased() {
  if (key == 'a' || key == 'A' || keyCode == LEFT ) { a = 0.0f; }
  if (key == 'd' || key == 'D' || keyCode == RIGHT) { d = 0.0f; }
  if (key == 's' || key == 'S' || keyCode == DOWN ) { s = 0.0f; }
  if (key == 'w' || key == 'W' || keyCode == UP   ) { w = 0.0f; }
  if (keyCode ==  SHIFT) { val = regspeed; }
}

void wrap(PVector pos){
  if (pos.x>displayWidth)  { pos.x=0; }
  if (pos.x<0) {  pos.x=displayWidth; }
  if (pos.y>displayHeight) { pos.y=0; }
  if (pos.y<0) { pos.y=displayHeight; } 
}
