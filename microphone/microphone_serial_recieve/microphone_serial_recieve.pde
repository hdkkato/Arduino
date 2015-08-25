/*
Description
processing program for Arduino DUE

Arduino DUE send 2 byte signal with header "H" via serial communication
such as H1927H2158H2272... 

This program shape signal like below
1927
2158
2272
...
*/

import processing.serial.*;
Serial myPort;

// format date to yyyymmdd
String dd = nf(day(), 2);
String mm = nf(month(), 2);
String yyyy = nf(year(), 4);
String rate = "8900sps";
String fileName = yyyy + mm + dd +"_due_" + rate + ".dat"

int data;
int cnt = 0;
int maxcnt = 512

PrintWriter output;

void setup()
{
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.clear();
  output = createWriter(fileName);
}

void draw()
{
  if (cnt > (maxcnt-1))
  {
    output.close();
    exit();
  }


  if (myPort.available() >= 3) {
    if (myPort.read() == 'H') {
      int high = myPort.read();
      int low = myPort.read();
      data = high*256 + low;
      output.println(data);
    }
    cnt++;
  }
}

