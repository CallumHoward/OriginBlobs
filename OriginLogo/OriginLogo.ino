#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16


void setup()   {
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  // init done

  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  display.drawLine(21, 27, 21, 8, WHITE);
  display.drawLine(21, 27, 4, 37, WHITE);
  display.drawLine(21, 27, 38, 37, WHITE);

  display.drawLine(47, 8, 47, 37, WHITE);
  display.drawLine(57, 8, 57, 37, WHITE);

  display.display();
  delay(2000);
  display.clearDisplay();

}


void loop() {
}
