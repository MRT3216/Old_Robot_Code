import processing.video.*;
import javax.imageio.*;
import java.awt.image.*; 
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;

import java.io.ByteArrayOutputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.IOException;

// Capture object
Capture cam;
void setup() {
  size(320,240);
  
  // Initialize camera
  cam = new Capture( this, width,height,30);
  cam.start();
}

void captureEvent( Capture c ) {
  c.read();
  // Whenever it gets a new image, send it!
  broadcast(c);
}

void draw() {
  image(cam,0,0);
}


// Convert PImage to byte[] to broadcast
void broadcast(PImage img) {

  // We need a buffered image to do the JPG encoding
  BufferedImage bimg = new BufferedImage( img.width,img.height, BufferedImage.TYPE_INT_RGB );

  // Transfer pixels from localFrame to the BufferedImage
  img.loadPixels();
  bimg.setRGB( 0, 0, img.width, img.height, img.pixels, 0, img.width);
  ByteArrayOutputStream baStream	= new ByteArrayOutputStream();
  BufferedOutputStream bos		= new BufferedOutputStream(baStream);
  // Turn the BufferedImage into a JPG and put it in the BufferedOutputStream
  // Requires try/catch
  try {
    ImageIO.write(bimg, "jpg", bos);
  } 
  catch (IOException e) {
    e.printStackTrace();
  }

  // Get the byte array, which it will send out via TCP!
  byte[] packet = baStream.toByteArray();
  println("Byte[] created with " + packet.length + " bytes");
}
