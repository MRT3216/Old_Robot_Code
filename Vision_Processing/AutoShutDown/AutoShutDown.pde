import java.io.IOException;


void setup(){
    try { shutdown(); } 
    catch (IOException e) { e.printStackTrace(); } 
    catch (RuntimeException e) { e.printStackTrace(); }
}

void draw(){
}

void shutdown() throws RuntimeException, IOException {
    String shutdownCommand = null;
    String osName = System.getProperty("os.name");        
    if (osName.startsWith("Win")) {
      shutdownCommand = "shutdown.exe -s -t 0";
    } else if (osName.startsWith("Linux") || osName.startsWith("Mac")) {
      shutdownCommand = "shutdown -h now";
    } else {
      System.err.println("Shutdown unsupported operating system ...");
        System.exit(0);
    }
    Runtime.getRuntime().exec(shutdownCommand);
}

