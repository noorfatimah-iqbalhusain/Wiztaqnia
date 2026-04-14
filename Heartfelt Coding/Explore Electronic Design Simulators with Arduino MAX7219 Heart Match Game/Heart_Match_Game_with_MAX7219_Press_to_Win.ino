/*
 * Heart Match Game with MAX7219: Press to Win
 * Made by www.wiztaqnia.com
 * Modified date: 26/01/2026
 * Typical pin layout used:
 * ----------------------------------------------------------------
 * Signal              MAX7219 LED     Arduino    Piezo      Push                
 *                      Dot Matrix      Nano      Buzzer    Button            
 *                    4-in-1 Display    Every              
 * ----------------------------------------------------------------
 * 5V                      5V           +5V         --        --
 * GND(Ground)             GND          GND        GND         4                
 * DIN (Data Input)        DIN          D11        --         --
 * CS (Chip Select)        CS           D10        --         --
 * CLK (Clock)             CLK          D12        --         --
 *Buzzer Output pin        --           D2         +          --
 * Button Input pin        --           D3         --          1
 */
 #include <LedControl.h> // control MAX7219 LED matrix modules

// Pin connections 
#define DIN_PIN    12  
#define CLK_PIN    11  
#define CS_PIN     10  
#define BUTTON_PIN 2   
#define BUZZER_PIN 3   

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 4); //LedControl(Data In, Clock, Chip Select, Number of MAX7219 LED matrix modules)
bool startGame = true; // Flag to show "GO!!" only once at start

// Bitmap for ❤️ Heart symbol (8x8 pixels)
byte heart[8] = {
  B00000000,
  B01100110,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000
};

// Bitmap for ❌ Cross symbol (8x8 pixels)
byte cross[8] = {
  B10000001,
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001
};

// Bitmap for letter G (corrected orientation)
byte G[8] = {
  B00111100,
  B01000010,
  B00000001,
  B01110001,
  B10000001,
  B10000010,
  B01000010,
  B00111100
};

// Bitmap for letter O
byte O[8] = {
  B00111100,
  B01000010,
  B10000001,
  B10000001,
  B10000001,
  B10000001,
  B01000010,
  B00111100
};

// Bitmap for exclamation mark (!)
byte EXCL[8] = {
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00011000,
  B00000000,
  B00011000,
  B00011000
};

void setup() {
  
  for (int i = 0; i < 4; i++) {
    lc.shutdown(i, false);    //Initialize LED Dot Matrix display 
    lc.setIntensity(i, 5);    //Set the brightness of the LED Dot Matrix. The range of brightness is 0 to 15
    lc.clearDisplay(i);       //Clear any previous data
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP); //Use internal pull-up resistor
  pinMode(BUZZER_PIN, OUTPUT);       //Set buzzer pin as output
  displayGO();                       //Show "GO!!" message at start
}

void loop() {

  // Check if button is pressed (LOW because of pull-up)
  if (digitalRead(BUTTON_PIN) == LOW) {
    while (digitalRead(BUTTON_PIN) == LOW); 
    tone(BUZZER_PIN, 1000, 100); // Regular buzzer tone

    // Clear "GO!!" message only on the first button press
    if (startGame) {
      clearAll();        
      startGame = false; 
    }

    int heartCount = 0; 

    // Randomly display either a heart or a cross on each module
    for (int i = 0; i < 4; i++) {
      bool showHeart = random(2); //Randomly choose 0 or 1
      if (showHeart) {
        displayShape(i, heart);  //Show heart
        heartCount++;
      } else {
        displayShape(i, cross);  //Show cross
      }
    }

    delay(1000);   //Pause to let user see the result

    // If all 4 modules show hearts, it's a jackpot!
    if (heartCount == 4) {
      tone(BUZZER_PIN, 1500, 800); // Longer beep for jackpot
      blinkHearts(3);              // Blink hearts 3 times
    } else {
      tone(BUZZER_PIN, 400, 300); // Error buzzer tone
      clearAll();                 //Clear display if not a jackpot
    }

    delay(1500);  //Pause before next round
  }
}

// Display a given 8x8 shape on a specific module (0–3)
void displayShape(int module, byte shape[8]) {
  for (int row = 0; row < 8; row++) {
    lc.setRow(module, row, shape[row]);
  }
}

// Show "GO!!" message across the 4 modules (right to left)
void displayGO() {
  displayShape(3, G);     //Rightmost module shows 'G'
  displayShape(2, O);     //Next shows 'O'
  displayShape(1, EXCL);  //Then '!'
  displayShape(0, EXCL);  //And another '!' for emphasis
}

// Clear all modules by displaying ❌ on each
void clearAll() {
  for (int i = 0; i < 4; i++) {
    displayShape(i, cross);
  }
}

// Blink all modules with hearts a given number of times
void blinkHearts(int times) {
  for (int t = 0; t < times; t++) {
    clearAll();                   
    delay(250);
    for (int i = 0; i < 4; i++) {
      displayShape(i, heart); 
    }
    delay(250);
  }
}
