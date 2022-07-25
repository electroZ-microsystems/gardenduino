/**************************************************************************
  Sketch: MENU NAVIGATION WITH JUST 4 BUTTONS

  This Sketch displays Menu without any Serial Communication and the
  navigation is performed by 4 buttons attached to D3, D8, D4 and D1.
  Also, attach led on D11 to control brightness from menu.
  Default brightness is 15% (check line 78)

   NOTE: By default, navigation buttons use INTERNAL_PULLUP feature.
         This can be changed by commenting the line 45 in "config.h" file

   Uses SSD1306Ascii Library(https://github.com/greiman/SSD1306Ascii)
   by Bill Grieman

   Created by Tamojit Saha
    Github: https://github.com/TamojitSaha
    Website: https://www.tamojitsaha.info/
***************************************************************************/
#include <Arduino.h>

// #include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#include <menu.h>
#include <menuIO/SSD1306AsciiOut.h>
#include <menuIO/keyIn.h>
//#include <menuIO/chainStream.h>
using namespace Menu;

SSD1306AsciiWire oled;

#include "config.h"


int wateringDuration = 20;   //Default LED brightness value
result adjustWateringDuration() {
   Serial.println(String("adjustWateringDuration ")+String(wateringDuration));
}

int IntervalTimeValue = 24;   //Default LED brightness value
result adjustIntervalTime() {
   Serial.println(String("adjustIntervalTime ") + String(IntervalTimeValue));
}

result startNow() {
   Serial.println("Watering !");
}

MENU(mainMenu, "Main menu", doNothing, noEvent, noStyle,
   FIELD(wateringDuration,   "Duration", "s", 0, 600, 5, 5, adjustWateringDuration, enterEvent, noStyle),
   FIELD(IntervalTimeValue, "Interval", "h", 0, 240, 12, 12, adjustIntervalTime, enterEvent, noStyle),
   OP("Start now !",startNow,enterEvent)
);



//describing a menu output device without macros
//define at least one panel for menu output
const panel panels[] MEMMODE = {{0, 0, 128 / fontW, 64 / fontH}};
navNode* nodes[sizeof(panels) / sizeof(panel)]; //navNodes to store navigation status
panelsList pList(panels, nodes, 1); //a list of panels and nodes
idx_t tops[MAX_DEPTH] = {0}; //store cursor positions for each level

#ifdef LARGE_FONT
SSD1306AsciiOut outOLED(&oled, tops, pList, 8, 2); //oled output device menu driver

#else
SSD1306AsciiOut outOLED(&oled, tops, pList, 5, 1); //oled output device menu driver
#endif

menuOut* constMEM outputs[]  MEMMODE  = {&outOLED}; //list of output devices
outputsList out(outputs, 1); //outputs list

//build a map of keys to menu commands
keyMap joystickBtn_map[] = {
  { -BTN_SEL, defaultNavCodes[enterCmd].ch} ,
  { -BTN_UP, defaultNavCodes[upCmd].ch} ,
  { -BTN_DOWN, defaultNavCodes[downCmd].ch} 
};
keyIn<TOTAL_NAV_BUTTONS> joystickBtns(joystickBtn_map);//the input driver


NAVROOT(nav, mainMenu, MAX_DEPTH, joystickBtns, out);


//when menu is suspended
result idle(menuOut &o, idleEvent e) {
  o.clear();
  
  return proceed;
}

void setup() {
  Serial.begin(115200);
  joystickBtns.begin();
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  oled.begin(&Adafruit128x64, OLED_I2C_ADDRESS); //check config
  oled.setFont(menuFont);
  oled.clear();
  nav.idleTask = idle; //point a function to be used when menu is suspended
}

void loop() {
  nav.poll();
  delay(1);//simulate a delay when other tasks are done
}
