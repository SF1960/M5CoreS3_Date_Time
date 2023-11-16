/***************************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5CoreS3 sample source code
*
* Visit for more information: https://docs.m5stack.com/en/core/CoreS3
*
* Describe: Hello World
* Date: 2023/5/3
****************************************************************************************/
/***************************************************************************************
* Description:  Display the time and date from the internet using ntp server
* Sketch:       HelloWorld_2.ino
* Version:      2.0
* Version Desc: 1.0 Defualt sketch using ntp server, touch for brightness and EEPROM storage
*               2.0 Use the SD card to retrieve WiFi credentials from "config.txt" ** CANNOT GET THIS TO WORK **
*                   Draw rounded boxes around date and time
* Board:        M5 Core S3
* Author:       Steve Fuller
* Website:      sgfpcb@gmail.com 
* Comments      This code modified from Hello World sketch example from M5
***************************************************************************************/
#include <M5CoreS3.h>
#include <WiFi.h>     
#include <time.h>
#include <M5GFX.h>                                        // library to handle screen touches
#include <Preferences.h>                                  // library to store to EEPROM
#include <FS.h>                                           // file system library
#include <SD.h>                                           // SD card library
#include <SPI.h>                                          // SPI communications protocol

M5GFX display;                                            // set up an object called display
   
Preferences config;                                       // set up an object called config

#include "globals.h"
#include "M5Helper.h"
#include "WiFiHelper.h"
#include "prefHelper.h"

// screen brightness variables
byte brightness[5] = {15, 30, 45, 60, 85};                 // define brightness steps
byte chosenBrightness = 1;                                 // set to 2nd brightness level

// colour definitions
#define colorDarkGrey 0x01EA
#define colGreen GREEN

/***************************************************************************
* After M5CoreS3 is started or reset the program in the setUp()
* function will be run, and this part will only be run once.
****************************************************************************/
void setup() {

  Serial.begin(9600);
                 
  chosenBrightness = states::getBrightness();              // get the stored value of screen brightness from EEPROM
  display.init();                                          // initialised display object (for touch)
  coreS3::setup();                                         // set up the Core S3 display

  SD.begin(GPIO_NUM_4, SPI, 25000000);
  listDir(SD, "/", 0);                                     // list contents of SD card
  readFile(SD, "/config.txt");                             // read the config.txt file 

  if (wifi::connect(SSID, PWD)){                           // attempts to connect to the 1st WiFi network. Returns True or False.
    wifi::connected(SSID);                                 // if successful, display the SSID 
  } else {                                                 // if NOT successful, continue here
    wifi::connectError(SSID);                              // display an error message with the SSID
  }

  wifi::ntp();                                             // connect to the ntp server and obtain date and time
  coreS3::background();                                    // draw screen's background
  drawBars();                                              // draw brightness bars
  drawBGSecondBars();                                      // draw seconds bars

} // setup()

/********************************************************************************
* After the program in setup() runs, it runs the program in loop()
* The loop() function is an infinite loop in which the program runs repeatedly
*********************************************************************************/
void loop() {

  lgfx::touch_point_t tp[3];
  int touch = display.getTouchRaw(tp, 3);

  if (touch == 1){
    
    if (chosenBrightness++ >= 4){
      chosenBrightness = 0;
    }

    USBSerial.printf("Screen Brightness: %d%%\n", brightness[chosenBrightness]);
    drawBars();                                           // redraw the brightness bars
    delay(250);                                           // mini debounce                   
    states::saveBrightness(chosenBrightness);             // save the new brightness value to EEPROM

  }

  unsigned long CurrentMillis = millis();                  // get the system millis()
  if (CurrentMillis - PreviousMillis >= UpdateInterval) {  // when millis() is greater than UpdateInterval then execute code

    PreviousMillis = CurrentMillis;                        // save the last time was updated
    flushTime();                                           // get the time and display
    if (seconds++>=60){                                    // if seconds >= 60
      seconds = 0;                                         // reset the seconds counter 
      drawBGSecondBars();                                  // draw the background bars for seconds
    }
    drawSecondBars(seconds++);                             // draw the seconds bar

  }

  coreS3::displayBattery();
  coreS3::displaySD();

} // loop()

/**********************************************************************************
* draw screen brightness bars
**********************************************************************************/
void drawBars(){

  // brightness gauge background bars
  for(int i=0; i<5; i++)
  M5.Lcd.fillRect(10+(i*6), 9, 4, 12, DARKGREEN);

  // brightness gauge bars
  for(int i=0; i<chosenBrightness+1; i++)
  M5.Lcd.fillRect(10+(i*6), 9, 4, 12, colGreen);

} // drawBars()

/***********************************************************************************
* draw Second Bars background
***********************************************************************************/
void drawBGSecondBars(){

  // brightness gauge background bars
  for(int i=0; i<60; i++)
  M5.Lcd.fillRect(10+(i*5), 220, 4, 12, colorDarkGrey);

} // drawBGSecondBars()

/***********************************************************************************
*  void draw Second Bars. It displays bars based on secs value
***********************************************************************************/
void drawSecondBars(int secs){

  // brightness gauge bars
  for(int i=0; i<secs; i++)
  M5.Lcd.fillRect(10+(i*5), 220, 4, 12, colGreen);

} // drawSecondBars()

/**********************************************************************************
*  Get the time and print to the display
***********************************************************************************/
void flushTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  // extract data from timeinfo variable returned by the ntp server
  char timeHour[3];   strftime(timeHour, 3,   "%H", &timeinfo);     // Set the hour
  char timeMins[3];   strftime(timeMins, 3,   "%M", &timeinfo);     // set the minute
  char timeSecs[3];   strftime(timeSecs, 3,   "%S", &timeinfo);     // set the seconds
  char dateDate[3];   strftime(dateDate, 3,   "%d", &timeinfo);     // set the date
  char dateDay[12];   strftime(dateDay, 12,   "%A", &timeinfo);     // set the day
  char dateMonth[10]; strftime(dateMonth, 10, "%B", &timeinfo);     // set the month
  char dateYear[5];   strftime(dateYear, 5,   "%Y", &timeinfo);     // set the year

  if (timeSecs==00){
    USBSerial.printf("timeSecs == 00\n");
    seconds = 0;}
  //USBSerial.printf("timeSecs: %s\n", timeSecs);

  // Stores real-time time and date data to timeStrbuff and dateStrbuff
  sprintf(dayStrbuff, "%s", dateDay);
  sprintf(timeStrbuff, "%s:%s:%s", timeHour, timeMins, timeSecs);
  sprintf(dateStrbuff, "%s %s %s", dateDate, dateMonth, dateYear);

  // print the date and time
  M5.Lcd.setTextSize(2);                                           // Set the text size. 
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.drawString(dayStrbuff, width/2, 60, 1);                   // print the day of the week centred
  M5.Lcd.drawString(dateStrbuff, width/2, 90, 1);                  // print date centred on screen in font 1
  M5.Lcd.setTextSize(1);                                           // Set the text size.  
  M5.Lcd.drawString(timeStrbuff, width/2, 140, 7);                 // print time centred on screen in font 7

} // flushTime()

void readFile(fs::FS &fs, const char * path){
  USBSerial.printf("Reading file: '%s'\n", path);
  M5.Lcd.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    USBSerial.printf("Failed to open file '%s' for reading\n", path);
    M5.Lcd.printf("Failed to open %s\n", path);
    return;
  }

  USBSerial.print("Read from file: ");
  while(file.available()){
    USBSerial.write(file.read());
  }
  file.close();
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  USBSerial.printf("Listing directory: %s\n", dirname);
  M5.Lcd.setCursor(0,10);
  M5.Lcd.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    USBSerial.println("Failed to open directory");
    M5.Lcd.printf("Failed to open directory\n");
    return;
  }
  if(!root.isDirectory()){
    USBSerial.println("Not a directory");
    M5.Lcd.printf("Not a directory\n");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      USBSerial.print("  DIR : ");
      USBSerial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      USBSerial.print("  FILE: ");
      USBSerial.print(file.name());
      USBSerial.print("  SIZE: ");
      USBSerial.println(file.size());
    }
     file = root.openNextFile();
  }
}