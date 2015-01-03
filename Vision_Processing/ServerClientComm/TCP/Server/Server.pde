import java.io.*;
import java.net.*;
ServerSocket ss;
Socket s;
int x = 0;
void setup() {
  try {
    ss = new ServerSocket(60010);
    println("ServerSocket created");
  }
  catch(IOException e) { e.printStackTrace(); }  
  thread("sender");
}

void draw() {
  //nothing here!
}

void sender() {
  println("Sender thread started");
  while (true) {
    try {
      s = ss.accept();
      if (s!=null) { println("Socket opened"); }
      while (s!=null) {
        DataOutputStream outToClient = new DataOutputStream(s.getOutputStream()); //Should this really be created every iteration of the loop?
        outToClient.writeBytes("Recieving info from server: "+x+"   ");
        outToClient.flush(); //not sure if this is necessary or not...  all the examples I've found have it.  Code runs the same without
        println("Sending Packet "+x);
        x++;
        Thread.sleep(1000);
      }
    }
    //commented out because IOException errors ruin the feng shui of the console output
    catch(IOException e)         {  } //e.printStackTrace(); }
    catch(InterruptedException e){  e.printStackTrace(); }
  }
}

/*  Some problems..... ? 
       There's IOException erors that I can't explain... uncomment line to see.
       Access the server by opening a web browser and typing in "localhost:60010".  Hit cancel
       or refresh to stop and restart.  Whan the client closes before the server socket,
       an IOException is thrown ("Software caused program to abort").  Not sure if this
       is critical or not (and whather or not it can be prevented).  Restarting the client
       resumes the program, so I'm pretty sure it's okay.  In some of the examples I've found,
       the Socket can recognize that the client has closed and close the socket before it throws
       an IOException... I think....  Next version:  Send MJPEG!
*/

/*  How to use...
       In case you have no idea what you're doing or how to test this program out by yourself,
       here's an explanation of the two easiest ways to ceck that the program is in fact
       working.  First:  open an internet browser and type "localhost:60010" without quotes.
       You should see the text coming through after a little while.  Localhost is what is known
       as the "loopback interface", shich is a dedicated IP address (254.0.0.1) that can be used 
       to send network infromation to and from the same computer without a network in-between.  
       60010, in case you couldn't tell, is the port which has been chosen to run the server on.  
       Ports below 1040 ae often being utilized by other processes (Skype, Chrome, P2P, etc), so 
       picking any port above that should suffice (make sure you use that port when typing the
       server URL into the web browser).  The second option will allow you to send data to any
       computer connected to the network.  First, open command prompt one the server machine and 
       type in "ipconfig" (on Windows Vista/7) and scroll through the informationn until you get 
       to the part that says "IPv4 address: xxx.xxx.xxx.xxx".  This series of numbers is the IP 
       you will use to connect to the server.  n another computer, open an internet browser and 
       type that series of numbers followed by the server port like this: "xxx.xxx.xxx.xxx:60010" 
       without quites, and the x's replaced with your IPv4 address.  If it works the output should 
       be the same as the output wen using LocalHost.


