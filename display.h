#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_WIDTH 128 // sets length and width
#define OLED_HEIGHT 64

#define OLED_ADDR 0x3C // gives i2c adress

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

void displayBootAnimation() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 6);
  display.println("Robot van");
  display.setTextSize(4);
  display.setCursor(2, 26);
  display.println("INF1A");
  display.display();
  delay(2000);
}

void displayText(String text1, int cursorWeight = 2, int cursorHeight = 30) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(cursorWeight, cursorHeight);
  display.println(text1);
  display.display();
}

void displayText(String text1, String text2) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 6);
  display.println(text1);
  display.setCursor(0, 36);
  display.println(text2);
  display.display();
}
