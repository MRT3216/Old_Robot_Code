import ipcapture.*;
ipcapture.IPCapture cam;
public void setup() {
  size(displayWidth,displayHeight);
  cam = new ipcapture.IPCapture(this, "http://212.219.113.227/axis-cgi/mjpg/video.cgi?resolution=640x480", "", "");
  cam.start();
}

void draw() {
  if (cam.isAvailable()) {
    cam.read();
    image(cam,0,0);
  }
}

void keyPressed() {
  if (key == ' ') {
    if (cam.isAlive()) cam.stop();
    else cam.start();
  }
}












//http://10.32.16.2/axis-cgi/mjpg/video.cgi?resolution=640x480&compression=20&fps=25  
// this works as well:    
// cam = new IPCapture(this);
// cam.start("url", "username", "password");    
// It is possible to change the MJPEG stream by calling stop()
// on a running camera, and then start() it with the new
// url, username and password.
