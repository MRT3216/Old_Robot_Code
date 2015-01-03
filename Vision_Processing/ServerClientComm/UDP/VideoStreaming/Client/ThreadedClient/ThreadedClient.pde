import java.io.*;
import java.net.*;
import javax.imageio.*;
import java.awt.image.*; 

PImage video;
ReceiverThread thread;

void setup() {
  size(displayWidth, displayHeight);
  video = createImage(320,240,RGB);
  thread = new ReceiverThread(video.width,video.height);
  thread.start();
}

 void draw() {
  if (thread.available()) {
    video = thread.getImage();
  }

  // Draw the image
  background(0);
  imageMode(CENTER);
  image(video,width/2,height/2);
}

boolean sketchFullScreen(){
  return true;
}



