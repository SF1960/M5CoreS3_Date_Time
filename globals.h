#pragma once

// Wifi credentials
#define SSID "TALKTALKB032AA"
#define PWD "6RGUAHGP"

char timeStrbuff[64];                                      // variable for time
char dateStrbuff[64];                                      // variable for date
char dayStrbuff[12];                                       // variable for the day

// array to hold month names
String month[12] = {"January","February","March","April","May","June","July","August","September","October","November","December"};

// screen size integers
int width  = 0;
int height = 0;

// battery power value
float battery = 0;

// seconds variable for second elapsed bars display
int seconds = 0;

// one second time delay without using delay() function
unsigned long PreviousMillis = 0;                            // for one second delay
const long UpdateInterval = 1000 ;                           // One second delay

bool SD_exists = false;                                      // boolean for the presents of an SD card

