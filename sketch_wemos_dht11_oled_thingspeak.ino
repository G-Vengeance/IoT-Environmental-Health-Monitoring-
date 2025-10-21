#include <ESP8266WiFi.h>
// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "69RXZO291YF9EYWH"; 
const char* ssid = "Redmi 9";
const char* password = "qwerty12345";
const char* server = "api.thingspeak.com";
WiFiClient client;

#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Declaring the display name (display)
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);   
  WiFi.begin(ssid, password);   
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Start the OLED display
  display.clearDisplay();
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
  float h = dht.readHumidity();  
  float t = dht.readTemperature();  
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  if (client.connect(server,80)) 
  {
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(h);
    postStr +="&field2=";
    postStr += String(t);
    postStr +="&field3=";
    postStr += String(hic);
    postStr += "\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    // Display to OLED
    display.clearDisplay();
    display.setTextColor(WHITE);  
    display.setTextSize(2);
    
    // -- Humidity --
    display.setCursor(0, 5);
    display.println("H: ");
    display.setCursor(25, 5);
    display.println(h);
    display.setCursor(90, 5);
    display.println("%");
    
    // -- Temperature --
    display.setCursor(0, 25);
    display.println("T: ");
    display.setCursor(25, 25);
    display.println(t);
    display.setCursor(90, 25);
    display.println("C");
    
    // -- Heat Index --
    display.setCursor(0, 45);
    display.println("HI");
    display.setCursor(25, 45);
    display.println(hic);
    display.setCursor(90, 45);
    display.println("C");  
    display.display(); 
    
    // Display to Serial Monitor ---------
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));
  }
  client.stop();
  delay(10000);
}
