#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)

void setup() {
  // put your setup code here, to run once:
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.clearDisplay();
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
  display.clearDisplay();
  display.setTextColor(WHITE);  
  display.setTextSize(2);
  display.setCursor(0, 5); // (baris, kolom) (X, Y)
  display.println("Selamat");
  display.setCursor(0, 25); // (baris, kolom) (X, Y)
  display.println("Datang....");
  display.display(); // display dalam kondisi on 
  delay(1000);

}
