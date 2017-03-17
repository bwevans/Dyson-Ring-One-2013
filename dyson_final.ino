#include <Streaming.h>
#include <LPD8806.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

File file;

const byte data   = 0;
const byte clock  = 1;
const byte chrg   = 2;
const byte led    = 3;
const byte rtc5v  = 13;
const byte rtcGnd = 14;

const byte chipSelect = 23;
const byte hardwareSelect = 10;

const int DS1307 = 0x68;

const byte nLEDs = 160;
LPD8806 strip = LPD8806(nLEDs, data, clock);

int frameIndex;
const int nFrames = 2002;
byte oldFrame[162];
byte newFrame[162];

const boolean debug = false;

void setup() {
  pinMode(chrg, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  pinMode(rtc5v, OUTPUT);
  pinMode(rtcGnd, OUTPUT);
  
  digitalWrite(led, HIGH);
  digitalWrite(rtc5v, HIGH);
  digitalWrite(rtcGnd, LOW);
  
  Serial.begin(9600);
  Wire.begin();
  
  strip.begin();
  strip.show();
  
  if (debug) Serial << "\nInitializing SD card... ";
  pinMode(hardwareSelect, OUTPUT);
  
  if (!SD.begin(chipSelect)) {
    if (debug) Serial << "Card failed, or not present." << endl;
    return;
  }
  if (debug) Serial << "Card Initialized.\n" << endl;
}

void loop() {
  int sunset,sunrise;
  int currentTime = rtc("hour") * 100 + rtc("minute");
  if (rtc("year") == 13 && rtc("month") == 7 && rtc("monthday") == 17) sunset = 1900;
  else sunset = twilight("sunset");
  sunrise = twilight("sunrise");
  
  if (debug) Serial << "Current time: " << currentTime << " Sunset: "
  << sunset << " Sunrise: " << sunrise << endl;
  
  while (currentTime >= sunrise - 100 && currentTime <= sunset - 100) {
    delay(60000);
    currentTime = rtc("hour") * 100 + rtc("minute");
    if (debug) Serial << "Current time: " << currentTime << " Sunset: "
      << sunset << endl;
    for (int i=0; i<nLEDs; i++) strip.setPixelColor(i, 0,0,0);
    strip.show();
  }

  while (currentTime > sunset - 100 || currentTime < sunrise - 100) {
    for (int i=0; i<nLEDs; i++) {
      strip.setPixelColor(i, newFrame[i],newFrame[i],newFrame[i]);
    }
    strip.show();
    delay(50);
    for (int i=0; i<nLEDs; i++) oldFrame[i] = newFrame[i];
    frameIndex++;
    if (frameIndex == nFrames) frameIndex = 0;
    fillArray(frameIndex);
    for (int i=0; i<nLEDs; i++) {
      byte brightness;
      if (newFrame[i] > oldFrame[i]) brightness = ((newFrame[i]-oldFrame[i])/2)+oldFrame[i];
      if (newFrame[i] < oldFrame[i]) brightness = oldFrame[i]-((oldFrame[i]-newFrame[i])/2);
      if (newFrame[i] == oldFrame[i]) brightness = oldFrame[i];
      strip.setPixelColor(i, brightness,brightness,brightness);
    }
    strip.show();
    delay(50);
    currentTime = rtc("hour") * 100 + rtc("minute");
  }
  
  for (int i=0; i<nLEDs; i++) {
    strip.setPixelColor(i, 0,0,0);
  }
  strip.show();
}

void fillArray(unsigned int point) {
  byte incomingByte = 0;
  file = SD.open("ring.txt");
  if (file) {
    file.seek(point*160*4);
    for (int i=0; i<160; i++) {
       newFrame[i]=0;
       for (int j=0; j<3; j++) {
         incomingByte = file.read();
         if (incomingByte >= '0' && incomingByte <= '9')
         newFrame[i] = newFrame[i] * 10 + (incomingByte - '0');
       }
       incomingByte = file.read();
    }
    file.close();
  }
}

int twilight(char fileName[12]) {
  int hour,minute,index,time;
  byte incomingByte = 0;
  int increment = 186; // number of characters or bytes per 31 day months
  //if (hour==true) 
  index = ((rtc("month")*increment)-increment)+((rtc("monthday")*6)-6);
  //else index = ((rtc("month")*increment)-increment)+((rtc("monthday")*6)-3);
  if (fileName == "sunset") fileName = "sunset.txt";
  if (fileName == "sunrise") fileName = "sunrise.txt";
  
  file = SD.open(fileName);
  if (file) {
    file.seek(index);
    for (int i=0; i<2; i++) {
      incomingByte = file.read();
      if (incomingByte >= '0' && incomingByte <= '9')
      hour = hour * 10 + (incomingByte - '0');
    }
    incomingByte = file.read();
    for (int i=0; i<2; i++) {
      incomingByte = file.read();
      if (incomingByte >= '0' && incomingByte <= '9')
      minute = minute * 10 + (incomingByte - '0');
    }
    file.close();
    time = hour * 100 + minute;
    return time;
  }
}

byte bcdToDec(byte val) {
  return ((val/16*10) + (val%16));
}

void printTime() {
  char buffer[3];
  sprintf(buffer, "%02d", rtc("minute"));
  Serial << rtc("month") << "/" << rtc("monthday") << " " << rtc("hour") << ":" << buffer;
}

byte rtc(char unit[8]) {
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0));
  Wire.endTransmission();
  Wire.requestFrom(DS1307, 7);

  byte second = bcdToDec(Wire.read());
  byte minute = bcdToDec(Wire.read());
  byte hour = bcdToDec(Wire.read());
  byte weekday = bcdToDec(Wire.read());
  byte monthday = bcdToDec(Wire.read());
  byte month = bcdToDec(Wire.read());
  byte year = bcdToDec(Wire.read());
  
  if (unit=="second") return second;
  if (unit=="minute") return minute;
  if (unit=="hour") return hour;
  if (unit=="weekday") return weekday;
  if (unit=="monthday") return monthday;
  if (unit=="month") return month;
  if (unit=="year") return year;
}

