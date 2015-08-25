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


int data;
int cnt = 0; 
int maxcnt = int(pow(2, 15));  // = 32768


float plotX1, plotY1;  // define graphic area
float plotX2, plotY2;  // 
float labelX, labelY;  // position of each labels

float dataMin = 0;
float dataMax = 4096;
float dataInterval = 1024;
float dataIntervalMinor = 512;

color col1;
PFont plotFont;

void setup()
{

  size(720, 405);

  plotX1 = 120; 
  plotX2 = width - 80;
  labelX = 50;
  plotY1 = 60;
  plotY2 = height - 70;
  labelY = height - 25;

  col1 = color(255, 127, 31);
  plotFont = createFont("SansSerif", 20);
  textFont(plotFont);
  initGraph();

  // serch valid serial port and connect to Arduino
  // in serial device list, sirial device number is usually 0
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 115200);
  myPort.clear();
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
    }

    float tx = map(cnt, 0, maxcnt, plotX1, plotX2);
    float ty = map(data, dataMin, dataMax, plotY2, plotY1);

    drawAxisLabels();
    drawVerticalLabels();
    drawHorizontalLabels();
    drawEllipse(tx, ty, col1);
    print("count: ");
    println(cnt);
    cnt++;
  }

  // when cnt get maxcnt, exit from program
  if (cnt >= maxcnt) {
    println("end");
    cnt = 0;
    initGraph();
  }
}


void initGraph() {
  fill(255);
  background(220);
  rectMode(CORNERS);
  noStroke();
  rect(plotX1, plotY1, plotX2, plotY2);
  cnt = 0;
}

void drawAxisLabels() {
  fill(0);
  textSize(13);
  textLeading(15);

  textAlign(CENTER, CENTER);
  text("Ampere [A]", labelX, (plotY1+plotY2)/2);
  textAlign(CENTER);
  text("Index", (plotX1+plotX2)/2, labelY);
}

void drawVerticalLabels() {
  fill(0);
  textSize(10);

  stroke(128);
  strokeWeight(1);

  for (float v = dataMin; v <= dataMax; v += dataIntervalMinor) {
    if (round(v % dataIntervalMinor) == 0) {
      float y = map(v, dataMin, dataMax, plotY2, plotY1);  
      if (round(v % dataInterval) == 0) {
        if (v == dataMin) {
          textAlign(RIGHT);
        } else if (v == dataMax) {
          textAlign(RIGHT, TOP);
        } else {
          textAlign(RIGHT, CENTER);
        }
        text(v, plotX1 - 10, y);
        line(plotX1 - 4, y, plotX1, y);
      } else {
        line(plotX1 - 2, y, plotX1, y);
      }
    }
  }
}

void drawHorizontalLabels() {
  fill(0);
  textSize(10);
  textAlign(CENTER, TOP);

  // Use thin, gray lines to draw the grid
  stroke(224);
  strokeWeight(1);

  for (int row = 0; row < width; row++) {
    if (row % 100 == 0) {
      float x = map(row, 0, width, plotX1, plotX2);
      text(row, x, plotY2 + 10);
      line(x, plotY1, x, plotY2);
    }
  }
}

void drawEllipse(float x, float y, color col) {
  noStroke();
  fill(col);
  ellipse(x, y, 4, 4);
}

