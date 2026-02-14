/*
 * Grant access to blue chain tag
 * Made by www.wiztaqnia.com
 * Modified date: 27/03/2022
 * Typical pin layout used:
 * ------------------------------------------------------
 *                                  MFRC522      NodeMCU      
 *  Signal                          Reader/PCD   ESP8266                 
 * ------------------------------------------------------
 * RST(Reset)                       RST          D3                             
 * SPI SS(Slave Select)             SDA(SS)      D8                                        
 * SPI MOSI(Master Out Slave In)    MOSI         D7                              
 * SPI MISO(Master In Slave Out)    MISO         D6                             
 * SPI SCK(Serial Clock)            SCK          D5                          
 */
#include <SPI.h>
#include <MFRC522.h>

const int RST_PIN = D3;         //Refer to the above pin layout
const int SS_PIN = D4;          //Refer to the above pin layout  

MFRC522 rfid(SS_PIN, RST_PIN); //Instance of the class
MFRC522::MIFARE_Key key;

String tag;

void setup() {
  Serial.begin(9600);  //Initialise UART Serial Communication with 9600 bits/s Baud Rate
  SPI.begin();         //Initialise SPI Bus
  rfid.PCD_Init();     //Initialise MFRC522
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D0, OUTPUT);
}
