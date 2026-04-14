/*
 * Word Clock using MAX7219 LED Dot Matrix Display
 * Made by www.wiztaqnia.com
 * Modified date: 23/11/2025
 * Typical pin layout used:
 * ----------------------------------------------------------
 * Signal              MAX7219 LED       DS3231     Arduino          
 *                      Dot Matrix        RTC        Nano    
 *                    4-in-1 Display     Module      Every             
 * ----------------------------------------------------------
 * 3.3V                    --             VCC         3V3      
 * 5V                      5V             --          +5V      
 * GND(Ground)             GND            GND         GND                
 * DIN (Data Input)        DIN            --          D11
 * CS (Chip Select)        CS             --          D10       
 * CLK (Clock)             CLK            --          D13
 * SCL (Serial Clock)      --             SCL         A4
 * SDA (serial Data)       --             SDA         A5
 */
#include <MD_Parola.h>     //Handles text effects
#include <MD_MAX72XX.h>    //MAX7219 driver
#include <SPI.h>           
#include <Wire.h>          
#include <RTClib.h>        


#define DISPLAY_TYPE MD_MAX72XX::FC16_HW  //Define the type of LED Dot Matrix Display used i.e. GENERIC_HW or FC16_HW
#define MATRICES 8                        //Define the total number of LED Dot Matrix Modules used. Each matrix consists of 64 LEDs

// Define SPI pins
#define DATA_PIN 11
#define CLK_PIN  13
#define CS_PIN   10


MD_Parola clockFace = MD_Parola(DISPLAY_TYPE, CS_PIN, MATRICES);  //MD_Parola(HARDWARE_TYPE,CS_PIN,MAX_DEVICES)
RTC_DS3231 clock;

void setup() {
  Wire.begin();      //Initialize I2C communication
  clock.begin();     //Initialize RTC module

  // Set RTC to compile time if it lost power
  if (clock.lostPower()) {
    clock.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  
  clockFace.begin();                      //Initialize LED Dot Matrix display 
  clockFace.setIntensity(8);              //Set the brightness of the LED Dot Matrix. The range of brightness is 0 to 15
  clockFace.displayClear();               //Clear the LED Dot Matrix display 
  clockFace.setTextAlignment(PA_CENTER);  //Center align text
}

// Return time in words for every quarter of an hour
String timeInWords(int hour, int minute) {
  int h = hour % 12;
  if (h == 0) h = 12;

  switch (minute) {
    case 0:  return "It's " + String(h) + " o'clock";
    case 15: return "Quarter past " + String(h);
    case 30: return "Half past " + String(h);
    case 45: return "Quarter to " + String((h % 12) + 1);
    default: return "";
  }
}

void loop() {
  DateTime now = clock.now();
  int hour = now.hour();
  int minute = now.minute();
  bool isPM = hour >= 12;

  // Convert to 12-hour format
  int displayHour = hour % 12;
  if (displayHour == 0) displayHour = 12;
  String phrase = timeInWords(hour, minute);  //Show time in words for every quarter of an hour

  if (phrase != "") {
    clockFace.setTextAlignment(PA_CENTER);                                 //Center align text
    clockFace.displayScroll(phrase.c_str(), PA_LEFT, PA_SCROLL_LEFT, 50);  //display.displayscroll(pText,align.textEffect,speed)
    while (!clockFace.displayAnimate()) {                                  //Wait for scroll to finish
      
    }
  } else {

    char timeString[20];
    sprintf(timeString, "%02d%c%02d %s %d/%d",    //Time display Format: "HH:MM AM/PM Day/Month" with blinking colon
            displayHour,
            (now.second() % 2 == 0) ? ':' : ' ',  //Blink the colon
            minute,
            isPM ? "PM" : "AM",
            now.day(), now.month());

    clockFace.displayReset();                //Reset scroll mode
    clockFace.setTextAlignment(PA_CENTER);   //Center align text
    clockFace.displayText(timeString, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT); //displayText(pText,align.textEffect,startPosition,endPosition,displayMode.staticText,animationEffect.noTransition)
    clockFace.displayAnimate();              //Scroll the text 
  }

  delay(1000);  //Update every second
}
