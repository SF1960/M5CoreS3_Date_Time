#pragma once

/***************************************************************************
* globals.h
* local library to set up constants, variables and definitions
****************************************************************************/

// Wifi credentials
#define SSID "TALKTALKB032AA"
#define PWD "6RGUAHGP"

// colour definitions
#define colorDarkGrey 0x01EA
#define colGreen GREEN

// screen brightness variables
byte brightness[5] = {15, 30, 45, 60, 85};                 // define brightness steps
byte chosenBrightness = 1;                                 // set to 2nd brightness level

char timeStrbuff[64];                                      // variable for time
char dateStrbuff[64];                                      // variable for date
char dayStrbuff[12];                                       // variable for the day

// array to hold month names
String month[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};

int width  = 0;                                            // width screen size integers
int height = 0;                                            // height screen size integers

float battery = 0;                                         // battery power value

// seconds variable for second elapsed bars display
int seconds = 0;

// one second time delay without using delay() function
unsigned long PreviousMillis = 0;                            // for one second delay
const long UpdateInterval = 1000 ;                           // One second delay

bool SD_exists = false;                                      // boolean for the presents of an SD card

