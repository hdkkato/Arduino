/*
Description
 processing program for Arduino DUE
 
 Arduino DUE send 2 byte signal with header "H" via serial communication
 such as "H1927H2158H2272..." 
 
 This program shape the signal like below
 1927
 2158
 2272
 ...
 
 finally write down the signal in a file
 
 refference:
 http://nakayasu.com/lecture/arduino%E5%9F%BA%E7%A4%8E/5617
 http://d.hatena.ne.jp/kougaku-navi/20141008/p1
 
 20150521 HK 
 20150605 HK fixed
 */

import processing.serial.*;
Serial myPort;

// format date to yyyymmdd
String dd = nf(day(), 2);
String mm = nf(month(), 2);
String yyyy = nf(year(), 4);
String rate = "20ksps";
String fileName = yyyy + mm + dd +"_due_" + rate + ".dat";

int data;
int cnt = 0; 
int maxcnt = int(pow(2, 15));  // = 32768

PrintWriter output;

void setup()
{
  // serch valid serial port and connect to Arduino
  // in serial device list, sirial device number is usually 0
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 115200);
  myPort.clear();
  output = createWriter(fileName);
  print("maxcnt: ");
  println(maxcnt);
  // specify the number of draw() function every second
  frameRate(10000);
}

void draw()
{ 

  // receive data via serial and write down
  if (myPort.available() >= 3) {
    if (myPort.read() == 'H') {
      int high = myPort.read();
      int low = myPort.read();
      data = high*256 + low;
      // file output takes much time, it takes about 5 min
      output.println(data);
    }
    print("count: ");
    println(cnt);
    cnt++;
  }

  // when cnt get maxcnt, exit from program
  if (cnt >= maxcnt) {
    println("end");
    output.close();
    exit();
  }
}

