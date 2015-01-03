PrintWriter output;
int w = 0, x = 1, y = 2, z = 3;
void setup() {
  frameRate(1);  //slow fR
  // Create a new file in the sketch directory
  output = createWriter("Output.txt");
  output.println("List1,List2,List3,List4"); 
}

void draw() {
  output.println(w+","+x+","+y+","+z); // Write the coordinate to the file
  w++; x++; y++; z++;
}

void keyPressed() {
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  exit(); // Stops the program
}

