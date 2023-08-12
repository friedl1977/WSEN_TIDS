/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/friedl/Desktop/Projects/WSEN_TIDS/src/WSEN_TIDS.ino"
/*
  Compiled by :  F BASSON
  Company     :  FIREFLY PTY LTD
  WWW         :  https://www.fire-fly.co.za

  Project     : Particle Photon 2 Tutorials.

  WSEN-TIDS - Read Sensor

  This Turorial reads the temperature from the WSEN-TIDS sensor in
  the continuous mode and displays it on the TFT display.
  
  The LDO determins whehter it is day/night and adjusts the display accordingly.
  The light thresholds can be adjusted by means of variables.

  This code is loosely based on (in includes) the libraries from
  Würth Electronics and Adafruit Industries.
*/

void setup();
void get_temp();
void LDR();
void draw_wake_screen();
void draw_sleepy_screen();
void loop();
#line 20 "/Users/friedl/Desktop/Projects/WSEN_TIDS/src/WSEN_TIDS.ino"
SYSTEM_MODE(SEMI_AUTOMATIC);

// Include Würth TIDS temperature sensor library //
#include "WSEN_TIDS.h"

// Include ST7789 TFT Display libraries //
#include "../lib/Adafruit_GFX_RK/src/Adafruit_GFX.h"
#include "../lib/Adafruit_ST7735_RK/src/Adafruit_ST7789.h"
#include "../lib/Adafruit_GFX_RK/src/Org_01.h"
#include "../lib/Adafruit_GFX_RK/src/FreeMonoBold24pt7b.h"
#include "../lib/Adafruit_GFX_RK/src/FreeSansBold12pt7b.h"
#include <SPI.h>

// LDR definitions // 
#define LDR_1 A0

int light_level_1 = 0;
int Sleep_State = 1;
int night_level = 950;                                              // LDR reading at which you want display to SLEEP
int day_level = 1050;                                                // LDR reading at which you want display to WAKE

// ST7789 TFT  definitions // 
#define TFT_CS        S3                                            // Define CS pin for TFT display
#define TFT_RST       D6                                            // Define RST pin for TFT display
#define TFT_DC        D5                                            // Define DC pin for TFT display

// TIDS definitions // 
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);     // Hardware SPI
Sensor_TIDS sensor;                                                 // call TIDS sensor
int status;
int ODR = 25;                                                       // Set temperature read frequency: (10 000/ODR)  e.g. 25 = 4s
int print_temp = 0;                                                 // The Output Data Rate in Hz
int previous_temp = 0; 

void setup() {

  delay(1000);
  Serial.begin(115400); // (115400)

  pinMode (LDR_1, INPUT);                                             // Set LDR pins as INPUT pins

  tft.init(320, 240);                                                 // Init ST7789 320x240 
  tft.fillScreen(ST77XX_BLACK);                                       // creates black background in display
  tft.setRotation(3);                                                 // set screen in landscape mode

  sensor.init(TIDS_ADDRESS_I2C_1);                                    // Initialize the I2C interface
  status = sensor.SW_RESET();                                         // Perform a software reset
  
  if (WE_FAIL == status) {
    Serial.println("Error: SW_RESET(). STOP!");
    while(1);
    }

  status = sensor.set_continuous_mode(ODR);             // Set the free run mode with given ODR
  
  if (WE_FAIL == status) {
    Serial.println("Error: set_continuous_mode(). STOP!");
    while(1);
  }
}

void get_temp() {

  float temperature;                                      // Read and calculate the temperature
  
  status = sensor.read_temperature(&temperature);
  
  if (WE_FAIL == status) {
    Serial.println("Error: read_temperature(). STOP!");
    while(1);
  }

  if (Sleep_State == 2) {
    previous_temp = 0;
    Sleep_State = 1;
    
    } else if (Sleep_State == 1) { 
      print_temp = temperature;
      }
    
  Serial.print(print_temp);                               // Print the temperature on the serial monitor as INT
  Serial.println(" C");
  Serial.print(temperature);                              // Print the temperature on the serial monitor as FLOAT
  Serial.println(" C");

  int waitMillis = 10000 / ODR;                           // Waiting time between measurement
  delay(waitMillis);                                      // Wait before continuing with the next measurement
  
  if (print_temp != previous_temp) { 
    draw_wake_screen();

   }
}

void LDR() {

  light_level_1 = analogRead (LDR_1);

    if (light_level_1 > day_level) {
      //Sleep_State = 1;
      Serial.println (Sleep_State);
      Serial.println (light_level_1);                     // DEBUG
      get_temp();

        } else if (light_level_1 < night_level) {
          Sleep_State = 2;
          Serial.println (Sleep_State);
          Serial.println (light_level_1);                 //  DEBUG
          draw_sleepy_screen();
        }

  Serial.println (Sleep_State);
}

void draw_wake_screen() {

////// Temperature Reading display ///////
  tft.setFont(&Org_01);
  tft.setTextSize(29);                                                                    // set font size
  tft.setTextWrap(false);                                                                 // set text wrapping
  tft.setTextColor(ST77XX_BLACK);                                                         // set font colour
  tft.setCursor(0, 160);        //160                                                     // set sursor to start writing text
  tft.print(previous_temp);                                                               // overwrite current display to clear screen

  tft.setTextColor(ST77XX_ORANGE);
  tft.setCursor(0,160);                                                                  
  tft.print(print_temp);                                                                  // Print current temperature 

  previous_temp = print_temp;                                                             // Set the new temperature = previous temperature

}

void draw_sleepy_screen() {

////// Sleepy display ///////
  tft.fillScreen(ST77XX_BLACK);                                                           // creates black background in display
  tft.setFont(&FreeSansBold12pt7b);
  delay(100);  
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(6);                                                                    // set font size
  tft.setTextWrap(false);                                                                // set text wrapping
  tft.setCursor(80, 200);                                                                // set sursor to start writing text
  tft.print("Z");
  delay(250);                                                                            // overwrite current display to clear screen
  tft.setTextSize(3);  
  tft.setCursor(180, 120);
  tft.print("z");
  delay(250);  
  tft.setTextSize(2);  
  tft.setCursor(230, 90);
  tft.print("z");  

  delay(1000);  
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(6);                                                                    // set font size
  tft.setTextWrap(false);                                                                // set text wrapping
  tft.setCursor(80, 200);                                                                // set sursor to start writing text
  tft.print("Z");
  delay(100);                                                                            // overwrite current display to clear screen
  tft.setTextSize(3);  
  tft.setCursor(180, 120);
  tft.print("z");
  delay(100);  
  tft.setTextSize(2);  
  tft.setCursor(230, 90);
  tft.print("z");  

}

void loop() {

  LDR();

 } 