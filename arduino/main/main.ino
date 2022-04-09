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

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  init_screen();
}

void loop() {
  for(int16_t i=0; i<128; i+=1) {
    int value = i % 128;
    
    char incipit[] = "Porcitudine ";
    char value_str[3];
    sprintf(value_str, "%d", value);
    //itoa(num, snum, 10);
    strcat(incipit,value_str);
    
    show_value_bar("Porcitudine di Dio", value, 0);
    }
  //two_lines_on_screen("dio", "cane", 0);
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

void show_value_bar(char string[], int value, int delay_time){
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
  display.setCursor(x, y);     // Start at top-left corner
  
  //display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.println(string);

  display.display();
  delay(delay_time);
}
