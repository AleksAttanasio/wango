/**************************************************************************              
           .---.                                              
          /. ./|                                              
      .--'.  ' ;                  ,---,               ,---.   
     /__./ \ : |              ,-+-. /  |  ,----._,.  '   ,'\  
 .--'.  '   \' .  ,--.--.    ,--.'|'   | /   /  ' / /   /   | 
/___/ \ |    ' ' /       \  |   |  ,"' ||   :     |.   ; ,. : 
;   \  \;      :.--.  .-. | |   | /  | ||   | .\  .'   | |: : 
 \   ;  `      | \__\/: . . |   | |  | |.   ; ';  |'   | .; : 
  .   \    .\  ; ," .--.; | |   | |  |/ '   .   . ||   :    | 
   \   \   ' \ |/  /  ,.  | |   | |--'   `---`-'| | \   \  /  
    :   '  |--";  :   .'   \|   |/       .'__/\_: |  `----'   
     \   \ ;   |  ,     .-./'---'        |   :    :           
      '---"     `--`---'                  \   \  /            
                                           `--`-'             
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdio.h>
#include <string.h>

// ***
#define MAX_ENCODER_VALUE 128
#define MIN_ENCODER_VALUE 0

const int ENCODER_PIN_BUTTON = 8;
const int ENCODER_PIN_DT = 3;
const int ENCODER_PIN_CLK = 2;

// **** OLED CONFIG ****
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// **** VARIABLES ****
int counter = 0;
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
long lastencoderValue = 0;
int lastMSB = 0;
int lastLSB = 0;

/*********
 * SETUP *
 *********/

void setup() {
  Serial.begin(9600);

  // ENCODER SETUP
  pinMode(ENCODER_PIN_DT, INPUT_PULLUP);
  pinMode(ENCODER_PIN_CLK, INPUT_PULLUP);
  pinMode(ENCODER_PIN_BUTTON, INPUT_PULLUP);

  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);

  // SCREEN SETUP
  init_screen();

}

/**********
 ** LOOP **
 **********/

void loop() {
    int buttonValue = digitalRead(8);
    Serial.println(encoderValue);

    if (buttonValue == LOW){
      // If button pushed, turn LED on
      Serial.println("PRESS");
      text_on_screen("PRESS", 3, 0, 0, 0);
    }
    else{
      show_value_bar("Porcitudine di Dio", encoderValue, 128, 0);
    }
}

void init_screen(void){
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000); // Pause for 2 seconds
  
}
  
void updateEncoder() {
  int MSB = digitalRead(ENCODER_PIN_DT); //MSB = most significant bit
  int LSB = digitalRead(ENCODER_PIN_CLK); //LSB = least significant bit

  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  //if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  //if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
  if (sum == 0b1000) encoderValue ++;
  if (sum == 0b0010) encoderValue --;

  if (encoderValue < MIN_ENCODER_VALUE) encoderValue = MIN_ENCODER_VALUE;
  if (encoderValue > MAX_ENCODER_VALUE) encoderValue = MAX_ENCODER_VALUE;
  
  lastEncoded = encoded; //store this value for next time
}

void show_value_bar(char string[], int value, int max_value, int delay_time){
  display.clearDisplay();

  // FIRST LINE
  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(string);

  // BAR
  display.drawRoundRect(0,16, 128, 16, 4, SSD1306_WHITE);
  display.fillRoundRect(0, 16, value, 16, 4, SSD1306_WHITE);
  display.display();
  delay(delay_time);
}


void two_lines_on_screen(char string1[], char string2[], int delay_time) {
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  
  display.setCursor(0, 0);     // Start at top-left corner
  display.println(string1);

  display.setCursor(0, 16);
  display.println(string2);

  display.display();
  delay(delay_time);
}

void text_on_screen(char string[], int font_size, int delay_time, int x, int y) {
  display.clearDisplay();

  display.setTextSize(font_size);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(x, y);             // Start at top-left corner
  
  //display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.println(string);

  display.display();
  delay(delay_time);
}
