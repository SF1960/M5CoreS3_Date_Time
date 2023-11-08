/***************************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5CoreS3 sample source code
*
* Visit for more information: https://docs.m5stack.com/en/core/CoreS3
*
* Describe: RTC
* Date: 2023/5/1
*
* Updated by Steve Fuller Nov 2023
****************************************************************************************/
/***************************************************************************************
* Description:  RTC date and time display
* Sketch:       RTC_Date_Time.ino
* Version:      2.0
* Version Desc: 1.0 Default Version using hard-coded date and time
*               2.0 Using ntp server to connect to WiFi and obtain date and time
* Board:        M5 Core S3
* Author:       Stevee Fuller
* Website:      sgfpcb@gmail.com
* Comments      
***************************************************************************************/
#include <M5CoreS3.h>
#include <WiFi.h>
#include <time.h>
#include "WiFiHelper.h"

#define SSID "TALKTALKB032AA"
#define PWD "6RGUAHGP"

RTC_TimeTypeDef RTCtime;       // Object for Real Time Clock time
RTC_DateTypeDef RTCDate;       // Object for Real Time Clock date

char timeStrbuff[64];          // variable for time
char dateStrbuff[64];          // variable for date
char dayStrbuff[12];           // variable for the day

// array to hold month names
String month[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};

int width = 0;                 // variable to hold screen width
int height = 0;                // variable to hold screen height

#define CENTRE 4               // macro for text alignment

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

  // Stores real-time time and date data to timeStrbuff and dateStrbuff
  sprintf(dayStrbuff, "%s", dateDay);
  sprintf(timeStrbuff, "%s:%s:%s", timeHour, timeMins, timeSecs);
  sprintf(dateStrbuff, "%s %s %s", dateDate, dateMonth, dateYear);

  // print the date and time
  M5.Lcd.setTextSize(2);  // Set the text size. 
  M5.Lcd.drawString(dayStrbuff, width/2, 60, 1);   // print the day of the week centred
  M5.Lcd.drawString(dateStrbuff, width/2, 90, 1);  // print date centred on screen in font 1
  M5.Lcd.setTextSize(1);  // Set the text size.  
  M5.Lcd.drawString(timeStrbuff, width/2, 160, 7); // print time centred on screen in font 7
}

/*************************************************************************************
* After M5CoreS3 is started or reset the program in the setUp ()
* function will be run, and this part will only be run once.
**************************************************************************************/
void setup() {

  Serial.begin(115200);

  M5.begin(true, true, false);    // Init M5CoreS3. (LCDEnable, SerialEnable, I2CEnable)
  M5.Lcd.setTextSize(2);

  if (wifi::connect(SSID, PWD)){  // function that attempts to connect to the 1st WiFi network. Returns True or False.
    wifi::connected(SSID);        // if successful, display the SSID 
  } else {                        // if NOT successful, continue here
    wifi::connectError(SSID);     // display an error message with the SSID
  }

  wifi::ntp();                    // connect to the ntp server and obtain date and time

  width = M5.Lcd.width();         // get the screen's width
  height = M5.Lcd.height();       // get the screen's height
  M5.Lcd.setTextDatum(CENTRE);    // set the text alignment
  M5.Lcd.fillScreen(TFT_BLACK);   // clear screen
  M5.Lcd.drawRoundRect(0, 0, width, height, 10, TFT_BLUE);   // draw a rounded rectangle

  delay(1000);                    // one second delay

}

/*************************************************************************************
* After the program in setup() runs, it runs the program in loop()
* The loop() function is an infinite loop in which the program runs repeatedly
***************************************************************************************/
void loop() {
  flushTime();                 // get date and time and print to display
  delay(1000);                 // one second delay
}