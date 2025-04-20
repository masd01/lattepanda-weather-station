#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Ρυθμίσεις OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C  // Διεύθυνση I2C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// Δομή για τα δεδομένα από το Python
struct DisplayData {
  String time;
  String date;
  String date2;
  String temp;
  String humidity;
  String wind;
  String wind_arrow;
  String weather;  // "SUN", "RAIN", "CLOUDS", etc.
};

DisplayData receivedData;

const uint8_t PROGMEM weather_icons[][32] = {
  //Sun
  {0b00000000, 0b10000000,
  0b00000001, 0b11000000,
  0b00011100, 0b00011100,
  0b00011011, 0b11101100,
  0b00010111, 0b11110100,
  0b00001111, 0b11111000,
  0b00101111, 0b11111010,
  0b01101111, 0b11111011,
  0b00101111, 0b11111010,
  0b00001111, 0b11111000,
  0b00010111, 0b11110100,
  0b00011011, 0b11101100,
  0b00011100, 0b00011100,
  0b00000001, 0b11000000,
  0b00000000, 0b10000000,
  0b00000000, 0b00000000},
  //Cloud 
  {0b00000000, 0b00000000,
  0b00000111,  0b11000000,
  0b00001000, 0b00100000,
  0b00110000, 0b00010000,
  0b01000000, 0b00001100,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b01000000, 0b00000001,
  0b00100000, 0b00000001,
  0b00010000, 0b00000010,
  0b00001111, 0b11111100,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000},
  //Rain
  {0b00000000, 0b00000000,
  0b00000111,  0b11000000,
  0b00001000, 0b00100000,
  0b00110000, 0b00010000,
  0b01000000, 0b00001100,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b01000000, 0b00000001,
  0b00100000, 0b00000001,
  0b00010000, 0b00000010,
  0b00001111, 0b11111100,
  0b00000000, 0b00000000,
  0b00010010, 0b01001000,
  0b00100100, 0b10010000,
  0b01001001, 0b00100000,
  0b00000000, 0b00000000},
  //Storm
  {0b00000000, 0b00000000,
  0b00000111,  0b11000000,
  0b00001000, 0b00100000,
  0b00110000, 0b00010000,
  0b01000000, 0b00001100,
  0b01000000, 0b00000010,
  0b01000000, 0b00000010,
  0b01000000, 0b00000001,
  0b00100000, 0b01000001,
  0b00010000, 0b10000010,
  0b00001001, 0b00011100,
  0b00000011, 0b11000000,
  0b00000000, 0b10000000,
  0b00000001, 0b00000000,
  0b00000010, 0b00000000,
  0b00000100, 0b00000000},
  //Snow
  {0b00000000, 0b00000000,
  0b00001001, 0b00100000,
  0b00100101, 0b01001000,
  0b10100011, 0b10001010,
  0b01100001, 0b00001100,
  0b11110001, 0b00011110,
  0b00001101, 0b01100000,
  0b00000011, 0b10000000,
  0b00000011, 0b10000000,
  0b00001101, 0b01100000,
  0b11110001, 0b00011110,
  0b01100001, 0b00001100,
  0b10100011, 0b10001010,
  0b00100101, 0b01001000,
  0b00001001, 0b00100000,
  0b00000000, 0b00000000}
};

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Εκκίνηση οθόνης
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("SSD1306 initialization failed!");
    while (1);
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.display();
  Serial.println("OLED Ready!");
}

// Αναδιάρθρωση δεδομένων από το Python
void parseData(String input) {
  int pos = 0;
  while ((pos = input.indexOf('|')) != -1) {
    String key = input.substring(0, pos);
    input = input.substring(pos + 1);
    
    pos = input.indexOf('|');
    if (pos == -1) pos = input.length();
    String value = input.substring(0, pos);
    
    if (key == "TIME") receivedData.time = value;
    else if (key == "DATE") receivedData.date = value;
    else if (key == "DATE2") receivedData.date2 = value;
    else if (key == "TEMP") receivedData.temp = value;
    else if (key == "HUM") receivedData.humidity = value;
    else if (key == "WIND") receivedData.wind = value;
    else if (key == "WIND_ARROW") receivedData.wind_arrow = value;
    else if (key == "WEATHER") receivedData.weather = value;
    
    if (pos != input.length()) input = input.substring(pos + 1);
  }
}

// Σχεδίαση εικονιδίου καιρού
void drawWeatherIcon(int x, int y) {
  int icon_index = 0; // Προεπιλεγμένο (SUN)
  
  if (receivedData.weather == "CLOUD") icon_index = 1;
  else if (receivedData.weather == "RAIN") icon_index = 2;
  else if (receivedData.weather == "STORM") icon_index = 3;
  else if (receivedData.weather == "SNOW") icon_index = 4;
  
  display.drawBitmap(x, y, weather_icons[icon_index], 16, 16, SSD1306_WHITE); // icon size 
}

// Σχεδίαση στην OLED
void drawDisplay() {
  display.clearDisplay();
  
  // Χρόνος (μεγάλη γραμματοσειρά)
  display.setCursor(10, 0);
  display.setTextSize(2);
  display.println(receivedData.time);
  
  // Ημερομηνία
  display.setCursor(0, 18);
  display.println(receivedData.date);
  
  // Θερμοκρασία & Υγρασία
  display.setCursor(0, 36);
  // Εικονίδιο καιρού 
  drawWeatherIcon(60, 36);
  display.print(receivedData.temp);
  display.setTextSize(1);
  display.setCursor(80, 36);
  display.println(receivedData.date2);
  //display.print("     2025");
  
  // Άνεμος 
  display.setCursor(0, 55);
  display.print(receivedData.humidity);
  display.print("  ");
  display.print(receivedData.wind);
  display.print(" ");
  display.print(receivedData.wind_arrow);

  display.display();
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    parseData(data);
    drawDisplay();
  }
  delay(100);
}
