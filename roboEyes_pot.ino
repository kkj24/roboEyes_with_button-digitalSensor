/*
Hi, my name is khemal, i'm is student and learner who want to be engineer and programer/developer
you can follow my social media account @khemalkhantata with this link
https://www.instagram.com/khemalkhantata/
*/

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <FluxGarage_RoboEyes.h>

#define LCD_WIDTH 128
#define LCD_HEIGHT 64
#define LCD_ADDRESS 0x3C
#define LCD_RESET -1

#define FPS 100

#define moisture_sensor 14
#define light_sensor 13

Adafruit_SSD1306 lcd(LCD_WIDTH, LCD_HEIGHT, &Wire, LCD_RESET);
RoboEyes<Adafruit_SSD1306> eyes(lcd);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if(!lcd.begin(SSD1306_SWITCHCAPVCC, LCD_ADDRESS)) {
    Serial.println("Alocation failed..");
    for(;;);
  }

  eyes.begin(LCD_WIDTH, LCD_HEIGHT, FPS);
  eyes.setPosition(DEFAULT);
  eyes.close();
  eyes.setCuriosity(true);

  pinMode(moisture_sensor, INPUT);
  pinMode(light_sensor, INPUT);

}

void loop() {
  eyes.update();

  int value1 = analogRead(moisture_sensor);
  int value2 = analogRead(light_sensor);
  int kelembapan = map(value1, 0, 1023, 0, 100); //from 1023 to 100
  int keterangan = map(value2, 0, 1023, 0, 100); //from 1023 to 100
  static int persentase1 = 0;
  static int persentase2 = 0;

  static int ekspresi = 0;
  static int react = 0;

  if(kelembapan >= 50) ekspresi = 0;
  if(kelembapan >= 65) ekspresi = 1;
  if(kelembapan <= 15) ekspresi = 2;

  if(keterangan <= 35) react = 0;
  else if(keterangan >= 36) react = 1;

  if(persentase2 != keterangan) {
    Serial.print("Brightness:");
    Serial.println(String(keterangan) + "percent");
  }

  if(persentase1 != kelembapan) {
    Serial.print("Kelembapan:");
    Serial.println(String(kelembapan) + "percent");
    if(kelembapan >= 50) Serial.println("Ekspresi: Default");
    if(kelembapan >= 65) Serial.println("Ekspresi: Happy");
    if(kelembapan <= 25) Serial.println("Ekspresi: Angry");
  }

  switch(ekspresi) {
    case 0:
      eyes.open();
      eyes.setMood(DEFAULT);
      eyes.setAutoblinker(true);
    break;

    case 1:
      eyes.setMood(HAPPY);
      eyes.setAutoblinker(true);
      eyes.setIdleMode(true);
    break;

    case 2:
      eyes.setMood(ANGRY);
      eyes.setAutoblinker(true);
      eyes.setIdleMode(true);
    break;
  }

  switch(react) {
    case 0:
      eyes.open();
    break;

    case 1:
      eyes.close();
      eyes.setPosition(DEFAULT);
      eyes.setIdleMode(false);
      eyes.setAutoblinker(false);
    break;
  }

  persentase1 = kelembapan;
  persentase2 = keterangan;

  delay(1);
}
