/*
 *NodeMCU programmed to fade an LED
 * Made by www.wiztaqnia.com
 * Modified date: 12/03/2024
 * Typical pin layout used:
 * -----------------------------------
 * Signal          NodeMCU      LED                                  
 * -----------------------------------
 * Digital PWM        D0        +ve     
 * GND(Ground)       GND        -ve                   
 */
int led = D0;           
int fade = 250;    
int bright = 5;    

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  analogWrite(led, fade);
  fade = fade + bright;
  if (fade == 0 || fade == 255) {
    bright = -bright;
  }
  delay(30);
}
