#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>
#include <SD.h>
#include <SPI.h>




#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define SD_CS 10
#define BUFFPIXEL 20



 #define TS_MINX  142
 #define TS_MINY  119
 #define TS_MAXX  974
 #define TS_MAXY  920




#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin


#define MINPRESSURE 10
#define MAXPRESSURE 1000

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);




#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x7BEF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

int ThermistorPin = 14;
int Vo;
int graphTemp, oldTemp, time, oldTime;
float R1 = 10000;
float logR2, R2, T, C;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

void setup() {
  //code for the setup of the screen
Serial.begin(9600);
tft.reset();
tft.begin(0x9325);
tft.setRotation(tft.getRotation()+1);
tft.setRotation(1);
tft.setTextColor(GREEN);
tft.setCursor(0, 5);
tft.setTextSize(3);
tft.fillScreen(BLACK);
//set up ints
time = 255;
oldTime = 5;
oldTemp = 234;
}

void loop() {

//code for finding temp
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  T = (T * 9.0)/ 5.0 + 32.0; 
  C = T -32;
  C = C * 5/9;

//code to send tempature data to computure
  Serial.print("Temperature: "); 
  Serial.print(" C: ");
  Serial.print(C);
  Serial.print(" F: ");
  Serial.println(T); 

//code to write temp data to tft lcd screen
  tft.fillRect(0,0,220,100,BLACK); //clear old writings
  tft.setCursor(0, 5);
  tft.setTextSize(3);
  tft.println("Temperature: ");
  tft.print(" C: ");
  tft.println(C);
  tft.print(" F: ");
  tft.println(T);

  //map temp data to graph data
  graphTemp = map(C,0,40,235,125);

  //setup for graph
  if(time == 255){
    time = 10;
    oldTime = 5;
    //drawbox for graph of temp data/ clear old data
    tft.fillRect(6, 126, 248, 108, BLACK);
    tft.drawRect(5,125,250,110,GREEN);

    // write peramiters of graph
    tft.setCursor(229, 128);
    tft.setTextSize(1);
    tft.print("40 C");
    tft.setCursor(235, 225);
    tft.print("0 C");
  }

  //draw the line of data
  tft.drawLine(oldTime, oldTemp, time, graphTemp,GREEN);

  //clear and draw new data at end of graph
  tft.fillRect(255, 125, 36, 110, BLACK);
  tft.drawLine(255, graphTemp, 260, graphTemp, GREEN);
  tft.setCursor(262, graphTemp - 5);
  tft.setTextSize(1);
  tft.print(C);

  //update old data
  oldTime = time;
  oldTemp = graphTemp;
 
  time = time + 5; //note the increse of time by 5 seconds

  delay(5000);
}