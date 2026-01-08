#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <FluxGarage_RoboEyes.h>

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define LCD_RST -1
#define LCD_ADDRESS 0x3C //you can change your address oled

Adafruit_SSD1306 lcd(LCD_WIDTH, LCD_HEIGHT, &Wire, LCD_RST);
RoboEyes<Adafruit_SSD1306> eyes(lcd);

#define FPS 100 //FPS of eyes animation <Range::60-100|Recommended>

constexpr int in = 13; //button or digital sensor at pin 13

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if(!lcd.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS)) {
    Serial.println("Alocation failed.. try to change lcd address or check the wiring");
    return;
  }

  Serial.println("Animation ready");

  eyes.begin(LCD_WIDTH, LCD_HEIGHT, FPS);
  eyes.setPosition(DEFAULT);
  eyes.close();

  pinMode(in, INPUT_PULLUP);

  delay(1000);

  lcd.clearDisplay();

}

void loop() {
  eyes.update();
  eyes.open();

  bool input = digitalRead(in);
  static bool current = false;
  static int expression = 0;

  if(input && current) {
    expression++;
    if(expression > 3) expression = 0; //reset expression state
  }

  switch(expression) {
    case 0: //default expression = 0
      eyes.setMood(DEFAULT);
    break;

    case 1: //happy expression = 1
      eyes.setMood(HAPPY);
    break;

    case 2: //tired expression = 2
      eyes.setMood(TIRED);
    break;

    case 3:
      eyes.setMood(ANGRY);
    break;

  /*
  you also can add some expression or reaction at here with same format..
  */

  }

  current = !input;

}
