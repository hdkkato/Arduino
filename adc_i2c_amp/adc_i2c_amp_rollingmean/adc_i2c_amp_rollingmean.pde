/*
Description

convert digital received data to ampere unit
graph draw
calculate rollingmean

receive a data from Arduino UNO and convert it to ampere unit.
next calculate rolling mean of data and plot.
data is represent as header character 'H' and 2 byte digital data sach as 'Hxx'
ex. H944H1695H1700H1710...

Grove IIC ADC
VCC : +5.0V
SIG : 0/+6.0V

(Sig) +6.0V ->10k/10k -> +3.0V (Vin)

ADC121C021
Va :+3.0
Vin 0V/+3.0V
AD 12bit : 3.0/4096=0.73mV
Sig 10k/10k Vin 6.0/4096=1.46mV

ACS714 : +5A -5A current to voltage converter
VCC +5.0
Vout=Iin/185 + Vcc/2(2.5)
Iin = 0A -> 2.5V

Iin 50mA (5V 100ohm)
Vout  50/185 + Vcc/2(2.5)=2.5093V

function and class
void initGraph()
  intialize graph area and clean it, reset the counter cnt

void drawAxisLabels()
  draw axis labels

void drawVerticalLabels()
  dral vertical labels

void drawHorizontalLabels()
  draw hrizontal labels
  
void drawEllipse()
  draw ellipse in plot area
  
static class Queue
  field:
    - private int size          : length of queue
    - private int head          : head index of queue
    - private int tail          : tail index of queue
    - private float[size] data  : data array
  method:
    - Queue(int size)           : constructor
    - void enqueue(float item)  : add item to the tail of queue
    - float dequeue()           : get item from the head of queue
    - boolean isEmpty()         : judge wether queue is empty
    - boalean isFull()          : judge wether queue is full
    - float sum()               : calculate sum of queue
    - float mean()              : calculate mean of queue

2015/05/15 HK coding
2015/05/19 HK edit comment

reffernce:
describing plot area with axis: http://yoppa.org/bma10/1250.html
implementing queue: http://www.atmarkit.co.jp/ait/articles/1010/14/news127.html

*/

import processing.serial.*;

Serial myPort;

int data;           // digital value from adc of Arduino
int cnt = 0;        // count of plot area
float V_ref = 3.0;  // reference voltage of GROVE I2C ADC
float V_cc = 5.1;   // supply voltage on ammeter ACS714(-5A to +5A)
float V_out;        // analog output of ammeter
float amp;          // measured current
float amp_mean;
Queue buff = new Queue(500);

float plotX1, plotY1;  // define graphic area
float plotX2, plotY2;  // 
float labelX, labelY;  // position of each labels

float dataMin = -0.5;
float dataMax = 0.5;
float dataInterval = 0.25;
float dataIntervalMinor = 0.1;

color col1;
color col2;
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
  col2 = color(255, 31, 127);
  plotFont = createFont("SansSerif", 20);
  textFont(plotFont);
  frameRate(50);
  smooth();

  initGraph();

  // Serch a valid port
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.clear();
}

void draw()
{
  // Receive 2 byte integer with header character 'H' such as 'H1690'
  if (myPort.available() >= 3) {
    if (myPort.read() == 'H') {
      int high = myPort.read();
      int low = myPort.read();
      data = high*256 + low;
      // 
      V_out = data*2*V_ref/4096;
      amp = (V_out - 2.55)/0.185;

      float tx = map(cnt, 0, width, plotX1, plotX2);
      float ty = map(amp_mean, dataMin, dataMax, plotY2, plotY1);
      float ty2 = map(amp, dataMin, dataMax, plotY2, plotY1);

      drawAxisLabels();
      drawVerticalLabels();
      drawHorizontalLabels();
      drawEllipse(tx, ty, col1);
      drawEllipse(tx, ty2, col2);
      
      if (buff.isFull()) {
        buff.dequeue();
      }
      buff.enqueue(amp);
      amp_mean = buff.mean();
      println(amp_mean);
       
      if (cnt > width) {
        initGraph();
      }
      cnt++;
    }
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

static class Queue {
  private int size = 500;
  private float[] data = new float[size+1];
  private int head = 0;
  private int tail = 0;
  
  Queue(int size){
    // constructor
    this.size = size;
  }

  boolean enqueue(float item) {
    if (((tail+1) % data.length) == head) {
      return false;
    }
    data[tail++] = item;
    tail = tail % data.length;
    return true;
  }

  float dequeue() {
    float item = -1;
    if (tail != head) {
      item = data[head++];
      head = head % data.length;
    }
    return item;
  }

  float sum() {
    float sumx = 0;
    for (int i=0; i<size; i++) {
      sumx += data[i];
    }
    return sumx;
  }
  
  float mean() {
    return sum()/size;
  }

  boolean isEmpty() {
    return (tail == head);
  }
  
  boolean isFull() {
    return (((tail+1) % data.length) == head);
  }
}
