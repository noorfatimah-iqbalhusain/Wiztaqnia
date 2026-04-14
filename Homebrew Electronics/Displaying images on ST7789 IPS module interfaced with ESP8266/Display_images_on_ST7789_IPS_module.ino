/*
 *Display images on ST7789 IPS module
 *Made by www.wiztaqnia.com
 *Modified date: 29/09/2022
 *Typical pin layout used:
 * ---------------------------------------------
 * Signal               ST7789 IPS    ESP8266
 *                      Module        Interface
 *                      Pin           Pin
 * ---------------------------------------------
 * VCC(3.3V)            VCC           3V3
 * GND(Ground)          GND           GND
 * SDA(Serial Data)     DIN           D7(MOSI)
 * SLK(Serial Clock)    CLK           D5(SCLK)
 * CS(Chip Select)      CS            D8(CS)
 * DC(Data/Command)     DC            D0
 * RST(Reset)           RST           D1
 * BL(Backlight)        BL            D2
  */

#include <TFT_eSPI.h>
#include "Not_all_heros_wear_capes_lettering.h" //Image 1
#include "Metaverse.h"                          //Image 2
#include "Flower_pot_with_flowers.h"            //Image 3
TFT_eSPI ipsdisp= TFT_eSPI();           

void setup() {
 Serial.begin(115200);        //Initialise UART Communication with 115200 bits/s Baud Rate
 ipsdisp.begin();             //Initialise SPI Bus
 ipsdisp.init();              //Initialise ST7789
 ipsdisp.setRotation(2);      //Value 1 means landscape mode; Value 2 means portrait mode
 ipsdisp.setSwapBytes(true);  //Swap the byte order for pushImage() - corrects endianness
 ipsdisp.fillScreen(TFT_WHITE); 
}

void loop() {
 ipsdisp.pushImage(0,0,240,320,image1);    //ipsdisp.pushImage(x1,y1,x2,y2,array of image 1 containing 16-bit RGB565 data of each pixel) 
 ipsdisp.setCursor(0,295,4);               //ipsdisp.setCurser(x axis,y axis,text font style i.e. 1/2/4/6)
 ipsdisp.setTextColor(TFT_CYAN,TFT_BLACK); //ipsdisp.setTextColor(text color,text background color)
 ipsdisp.println(F("IMG TYP:Calligraphy  "));
 delay(10000);
 ipsdisp.pushImage(0,0,240,320,image2);    //ipsdisp.pushImage(x1,y1,x2,y2,array of image 2 containing 16-bit RGB565 data of each pixel) 
 ipsdisp.setCursor(0, 295,4);              //ipsdisp.setCurser(x axis,y axis,text font style i.e. 1/2/4/6)
 ipsdisp.setTextColor(TFT_CYAN,TFT_BLACK); //ipsdisp.setTextColor(text color,text background color)
 ipsdisp.println(F("IMG TYP:Animated     "));
 delay(10000); 
 ipsdisp.pushImage(0,0,240,320,image3);    //ipsdisp.pushImage(x1,y1,x2,y2,array of image 3 containing 16-bit RGB565 data of each pixel) 
 ipsdisp.setCursor(0,295,4);               //ipsdisp.setCurser(x axis,y axis,text font style i.e. 1/2/4/6)
 ipsdisp.setTextColor(TFT_CYAN,TFT_BLACK); //ipsdisp.setTextColor(text color,text background color)
 ipsdisp.println(F("IMG TYP:HD Photo     "));
 delay(10000);  
}
