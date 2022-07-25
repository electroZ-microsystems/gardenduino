// Simple I2C test for ebay 128x64 oled.
// 3 button for menu DOWN OK UP
//edz 11.1.2019

#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

// Define proper RST_PIN if required.
#define RST_PIN -1

SSD1306AsciiWire oled;

#define DOWN 11
#define OK   10
#define UP    9


//------------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  pinMode(DOWN, INPUT_PULLUP);
  pinMode(OK  , INPUT_PULLUP);
  pinMode(UP  , INPUT_PULLUP);
  Serial.print ("Hello");
  
  Wire.begin();
  Wire.setClock(400000L);

#if RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS, RST_PIN);
#else // RST_PIN >= 0
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
#endif // RST_PIN >= 0

  oled.setFont(Adafruit5x7);

  uint32_t m = micros();
  oled.clear();
  //oled.set2X();
  oled.println("Test\n buttons");
  //oled.set1X();
  
}
//------------------------------------------------------------------------------
void loop() 
{
  if (digitalRead(DOWN) == 0)
  {
     oled.clear();
     oled.println("DOWN");
     Serial.print("DOWN");
     while (digitalRead(DOWN) == 0){}
  }
  if (digitalRead(OK) == 0)
  { 
     oled.clear();
     oled.println("OK  ");
     Serial.print("OK  ");
     while (digitalRead(OK) == 0){}
  }
  if (digitalRead(UP) == 0)
  {
     oled.clear(); 
     oled.println("UP  ");
     Serial.print("UP  ");
     while (digitalRead(UP) == 0){}
  }
}
