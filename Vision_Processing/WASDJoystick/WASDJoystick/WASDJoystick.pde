float x_Axis, y_Axis;
float val = 0.75;
float w,a,s,d;
void setup() {
  size(500,500);
}

void draw() {
  x_Axis = (a+d)*val;
  y_Axis = (s+w)*val;
  println(x_Axis+", "+y_Axis);
}

void keyPressed() {
  if (key == 'a' || key == 'A' || keyCode == LEFT ) { a = -1.0f; }
  if (key == 'd' || key == 'D' || keyCode == RIGHT) { d =  1.0f; }
  if (key == 's' || key == 'S' || keyCode == DOWN ) { s = -1.0f; }
  if (key == 'w' || key == 'W' || keyCode == UP   ) { w =  1.0f; }
  if (keyCode ==  SHIFT) { val = 1.0f; }
}

void keyReleased() {
  if (key == 'a' || key == 'A' || keyCode == LEFT ) { a = 0.0f; }
  if (key == 'd' || key == 'D' || keyCode == RIGHT) { d = 0.0f; }
  if (key == 's' || key == 'S' || keyCode == DOWN ) { s = 0.0f; }
  if (key == 'w' || key == 'W' || keyCode == UP   ) { w = 0.0f; }
  if (keyCode ==  SHIFT) { val = 0.75f; }
}

/*
   This isn't *exactly* how a real keyboard first person shooter works.
   For example, holding shift while trying to move backwrds will not
   increase your speed in that direction, and neither will holding
   shift while purely strafing.
*/
