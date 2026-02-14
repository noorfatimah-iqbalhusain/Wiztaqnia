/*
 * Colour changing Bluetooth controlled WS2812B RGB ring light
 * Made by www.wiztaqnia.com
 * Modified date: 14/09/2023
 * Typical pin layout used:
 * --------------------------------------------------------------
 * Signal              MB102        16-Bit     HC-05     ESP8266          
 *                  Power Supply   WS2812B    Bluetooth  NodeMCU    
 *                    Module       RGB Ring    Module                  
 * ---------------------------------------------------------------
 * 3.3V                3.3V          V+         --         3V3      
 * 5V                   5V           --         VCC        VIN      
 * GND(Ground)          GND          GND        GND        GND                
 * DI(Data Input)       --           DI         --         D1       
 * RX(Receiver)         --           --         RX         TX            
 * TX(transmitter)      --           --         TX         RX         
 */
#include <Adafruit_NeoPixel.h>  
char key = 0;                 //Initialise a char type variable 'key' to zero
#define pixelcount 16         //Define 'pixelcount' with the Number of pixels in the WS2812B
#define PWM_pin D1            //Define 'PWM_pin' of ESP8266 to which the Data Input of WS2812B is connected
Adafruit_NeoPixel ring = Adafruit_NeoPixel(pixelcount,PWM_pin,NEO_GRB + NEO_KHZ800); //Adafruit_NeoPixel(pixel_count,PWM_pin,pixels are wired for GRB bitstream,800 KHz bitstream)

void setup() 
{
  Serial.begin(9600);       //Initialise USART communication with 9600 bits/s Baud Rate
  ring.begin();            
  ring.setBrightness(255); //Set the brightness of the pixels. The range of brightness for each WS2812B pixel is 0 to 255.
  ring.show();             //Initialise all pixels to 'off'                                                                                                  
}

void loop() {
  for(int i=0; i<pixelcount; i++)
    if(Serial.available() >0) //Check if ESP8266 has received any data from HC-05 Bluetooth Module.
    {
    key=Serial.read();       //Read the incoming data and store it in the 'key'variable
    
    if(key == 'C')           //Check if Button 'C' is pressed then display Cyan light                
     colorWipe(ring.Color(0,255,255),0); //colorWipe(ring.Color(R,G,B),delay in milliseconds)
    if(key =='O')            //Check if Button 'O' is pressed then display Orange light
    colorWipe(ring.Color(255,165,0),0); 

    if(key =='P')            //Check if Button 'P' is pressed then display Pink light
    colorWipe(ring.Color(255,0,255),0);
    
    if(key == 'V')           //Check if Button 'V' is pressed then display Violet Light
    colorWipe(ring.Color(143, 0, 255), 0);
    
    if(key == 'W'){         //Check if Button 'W' is pressed then play 3 cycles of white chasing light animation
     for(int i=0;i<3;i++)
      theaterChase(ring.Color(100,100,100),50);//theaterChase(ring.Color(R,G,B),delay in milliseconds)
    }
    if(key == 'R')         //Check if Button 'R' is pressed then play 3 cycles of rainbow effect animation
    rainbowCycle(15);  //rainbow(delay in milliseconds)
    }                               
}

// Fill the dots one after the other with a colour
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<ring.numPixels(); i++) {
    ring.setPixelColor(i, c);
    ring.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < ring.numPixels(); i=i+3) {
        ring.setPixelColor(i+q, c);    //turn every third pixel on
      }
      ring.show();

      delay(wait);

      for (int i=0; i < ring.numPixels(); i=i+3) {
        ring.setPixelColor(i+q, 0);     //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*3; j++) { // 3 cycles of all colours on wheel
    for(i=0; i< ring.numPixels(); i++) {
      ring.setPixelColor(i, Wheel(((i * 256 / ring.numPixels()) + j) & 255));
    }
    ring.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a colour value.
// The colours transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
