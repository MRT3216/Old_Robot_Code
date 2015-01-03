import java.io.*;
import java.net.*;

import processing.video.*;
import javax.imageio.*;
import java.awt.image.*; 
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;

import java.io.ByteArrayOutputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.IOException;

ServerSocket ss;
Socket s;
Capture cam;
PImage p;
int fR = 30;
byte[] packet;

void setup() {
  size(320,240);
  try {
    ss = new ServerSocket(60010);
    println("ServerSocket created");
  }
  catch(IOException e) { e.printStackTrace(); }  
  cam = new Capture( this, width,height,fR);
  cam.start();
  thread("sender");
}

void captureEvent( Capture c ) {
  c.read();
  // Whenever it gets a new image, send it!
  p = (PImage)c;
}

void draw() {
  image(cam,0,0);
}

void sender() {
  println("Sender thread started");
  while (true) {
    try {
      s = ss.accept();
      if (s!=null){
        println("Socket opened"); 
        OutputStream outToClient = s.getOutputStream();
        outToClient.write((
          "HTTP/1.1 200 OK" +
          "Content-Type: multipart/x-mixed-replace;" +
          "boundary=--boundary").getBytes());
        outToClient.flush();
        while (s!=null) {
          BufferedImage bimg = new BufferedImage( p.width,p.height, BufferedImage.TYPE_INT_RGB );
          p.loadPixels();
          bimg.setRGB( 0, 0, p.width, p.height, p.pixels, 0, p.width);
          ByteArrayOutputStream baStream  = new ByteArrayOutputStream();
          BufferedOutputStream bos    = new BufferedOutputStream(baStream);
          try { ImageIO.write(bimg, "jpg", bos); } 
          catch (IOException e) { e.printStackTrace(); }  
          packet = baStream.toByteArray();
          if (packet !=null){
            outToClient.write((
              "--boundary" +
              "Content-type: image/jpg" +
              "Content-Length: " +
              packet.length).getBytes());
            outToClient.write(packet);
            outToClient.flush();
            println("Byte[] created with " + packet.length + " bytes");
          }       
          Thread.sleep((int)(1000/fR));
        }
      }
    }
    //commented out because IOException errors ruin the feng shui of the console output
    catch(IOException e)         {  println("........");  e.printStackTrace(); }
    catch(InterruptedException e){  e.printStackTrace(); }
  }
}
