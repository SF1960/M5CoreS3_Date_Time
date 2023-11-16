#pragma once

/***************************************************************************
* M5Helper.h
* local library to set up and handle the M5
****************************************************************************/

#include "globals.h"

namespace coreS3{

  void setup(){
    M5.begin(true, true, true);                              // Init M5CoreS3.  (bool LCDEnable, bool USBSerialEnable, bool I2CEnable)
    width = M5.Lcd.width();                                  // save the screen's width
    height = M5.Lcd.height();                                // save the screen's height
    M5.Lcd.setTextSize(2);                                   // Set text size. 
    M5.Lcd.setTextDatum(CC_DATUM);                           // set drawString justification to centre
    M5.Lcd.fillScreen(BLACK);                                // Black screen
  }

  void background(){
    M5.Lcd.fillScreen(BLACK);                                // Black screen
    M5.Lcd.fillRect(5, 5, width -5, 25, BLUE);               // box around title   
    M5.Lcd.fillRoundRect(5, 25, width -5, height -30, 5, BLACK);// fill rounded black box
    M5.Lcd.drawRoundRect(5, 25, width -5, height -30, 5, CYAN);// cyan rounded box at screen extremties
    M5.Lcd.drawRoundRect(48, 133, 225, 60, 3, DARKGREY);     // box around time 
    M5.Lcd.drawRoundRect(48,  53, 225, 60, 3, DARKGREY);     // box around day and date 
         
  }
 
  // display the battery level top right of screen
  void displayBattery(){
    battery = M5.Axp.getBatLevel();
    M5.Lcd.setTextColor(WHITE, BLUE);
    M5.Lcd.drawFloat(battery, 2, 295, 11);
    M5.Lcd.drawString("%", 310, 11, 1);
  }

  // display SD Card status
  void displaySD(){
    SD_exists = M5.Axp.isSDCardExist();
    M5.Lcd.setTextColor(WHITE, BLUE);
    if (SD_exists){
      M5.Lcd.drawString("[SD]", 160, 7, 2);
    } else {
      M5.Lcd.drawString("[  ]", 160, 7, 2);
    }
  }

}