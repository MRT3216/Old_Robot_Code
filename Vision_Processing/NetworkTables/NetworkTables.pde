import edu.wpi.first.wpilibj.networktables.NetworkTable;
import java.util.logging.Level;
import java.util.logging.Logger;

int x, y;
NetworkTable table;
void setup() {
  size(320,240);
  // Initialize NetworkTables
  table = NetworkTable.getTable("datatable");
  thread("NetTableSender");
}

void draw() {
}

void NetTableSender(){
  println("thread running");
  while (true){
    try{ Thread.sleep(1000); }
    catch(InterruptedException ex){
      Logger.getLogger(NetworkTable.class.getName()).log(Level.SEVERE, null, ex);
    }
      
    table.putNumber("X", x);
    table.putNumber("Y", y);
    println("running...");
  }
}
