import processing.serial.*;

Serial myPort;

int data;
int cnt = 0; 
float V_ref = 3.0;
float V_cc = 5.1;
float V_out;
float amp;
float amp_mean;
Queue buff = new Queue();

float plotX1, plotY1;
float plotX2, plotY2;
float labelX, labelY;

float dataMin = -0.5;
float dataMax = 0.5;
float dataInterval = 0.25;
float dataIntervalMinor = 0.1;

color col;
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

  col = color(255, 127, 31);
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
  // Receive 2 byte integer with header such as 'H1690'
  if (myPort.available() >= 3) {
    if (myPort.read() == 'H') {
      int high = myPort.read();
      int low = myPort.read();
      data = high*256 + low;
      V_out = data*2*V_ref/4096;
      amp = (V_out - 2.55)/0.185;

      float tx = map(cnt, 0, width, plotX1, plotX2);
      float ty = map(amp_mean, dataMin, dataMax, plotY2, plotY1);

      drawAxisLabels();
      drawVerticalLabels();
      drawHorizontalLabels();
      drawEllipse(tx, ty);
      //println(amp);
      if (buff.isFull()) {
        buff.dequeue();
      }
      buff.enqueue(amp);
      amp_mean = buff.mean();
       println(amp_mean);
      // println(V_out);
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

void drawEllipse(float x, float y) {
  noStroke();
  fill(col);
  ellipse(x, y, 4, 4);
}

static class Queue {
  final int SIZE = 500;
  private float[] data = new float[SIZE+1];
  private int head = 0;
  private int tail = 0;

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
    for (int i=0; i<SIZE; i++) {
      sumx += data[i];
    }
    return sumx;
  }
  
  float mean() {
    return sum()/SIZE;
  }
  

  boolean isEmpty() {
    return (tail == head);
  }
  
  boolean isFull() {
    return (((tail+1) % data.length) == head);
  }
}

