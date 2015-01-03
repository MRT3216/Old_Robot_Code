import processing.video.*;
// Capture object
Capture cam;
void setup() {
  size(320,240);
  cam = new Capture( this, width,height,30);
  cam.start();
}

void captureEvent( Capture c ) {
  c.read();
}

void draw() {
  image(cam,0,0);
  saveFrame();
}

