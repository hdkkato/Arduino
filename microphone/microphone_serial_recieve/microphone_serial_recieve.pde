import processing.serial.*;
Serial myPort;
int data;
PrintWriter output;
int counter = 0; 

void setup()
{
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.clear();
  output = createWriter("log.txt");
}

void draw()
{
  if(counter > 512)
  {
    //output.flush();
    output.close();
    exit();
  }
  
  
  if(myPort.available() >= 3) {
    if(myPort.read() == 'H') {
      int high = myPort.read();
      int low = myPort.read();
      data = high*256 + low;
    output.println(data);
    }
    counter++;
  }
}
