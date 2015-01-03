import processing.video.*;
import java.io.*;
import java.net.*;
import javax.imageio.*;
import javax.swing.JOptionPane;
import java.awt.image.*; 

// This is the port we are sending to
int clientPort = 9100; 
// This is our object that sends UDP out
DatagramSocket ds; 
// Capture object
Capture cam;
String ipv4;
void setup() {
  size(640,480);
  ipv4 = (String)JOptionPane.showInputDialog(
         null, "Complete the sentence:\n"
         + "Client IPv4 Address:",
         "Customized Dialog",
         JOptionPane.PLAIN_MESSAGE);
  // Setting up the DatagramSocket, requires try/catch
  try {
    ds = new DatagramSocket();
  } catch (SocketException e) {
    e.printStackTrace();
  }
  // Initialize Camera
  cam = new Capture( this, width,height,30);
  cam.start();
}

void captureEvent( Capture c ) {
  c.read();
  // Whenever we get a new image, send it!
  broadcast(c);
}

void draw() {
  image(cam,0,0);
}

void broadcast(PImage img) {

  // We need a buffered image to do the JPG encoding
  BufferedImage bimg = new BufferedImage( img.width,img.height, BufferedImage.TYPE_INT_RGB );

  // Transfer pixels from localFrame to the BufferedImage
  img.loadPixels();
  bimg.setRGB( 0, 0, img.width, img.height, img.pixels, 0, img.width);

  // Need these output streams to get image as bytes for UDP communication
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

  // Get the byte array, which we will send out via UDP!
  byte[] packet = baStream.toByteArray();

  // Send JPEG data as a datagram
  println("Sending datagram with " + packet.length + " bytes");
  try {
    ds.send(new DatagramPacket(packet,packet.length, InetAddress.getByName(ipv4),clientPort));
  } 
  catch (Exception e) {
    e.printStackTrace();
  }
}

