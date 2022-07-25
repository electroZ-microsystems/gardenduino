//GardenDuino 1.0

//edz 1.2019
// Target UNO / Mega


// use this Putty version to debug: http://www.extraputty.com/download.php
#include <Arduino.h>
#include <EEPROM.h>
#define EEPROM_Duration 0
#define EEPROM_Interval 1

#define VERSION "WATERING 1.0"

#define PUMP 2


#define TIMER_MAX_TASKS 2  
#include <timer.h> // https://github.com/contrem/arduino-timer  available from Arduino library management (arduino-timer).

void(* reset) (void) = 0; //declare reset function @ address 0

auto timer1_Interval = timer_create_default(); // create a timer with default settings
auto timer2_Watering = timer_create_default(); // create a timer with default settings


unsigned char wateringDuration = EEPROM.read(EEPROM_Duration);
unsigned char IntervalTime = EEPROM.read(EEPROM_Interval);



// #include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#include <menu.h>  //ArduinoMenu Library by Rui Azevedo
#include <menuIO/SSD1306AsciiOut.h>
#include <menuIO/keyIn.h>
//#include <menuIO/chainStream.h>
using namespace Menu;

SSD1306AsciiWire oled;

#include "config.h"

unsigned long sec (unsigned long  s){ return s                * 1000;}
unsigned long mn  (unsigned long  m){ return m           * 60 * 1000;}
unsigned long hour(unsigned long  h){ return h      * 60 * 60 * 1000;}
unsigned long day (unsigned long  d){ return d * 24 * 60 * 60 * 1000;}

bool OnTimer1_Interval(void *) 
{
  timer2_Watering.in(sec(wateringDuration), OnTimer2_StopWatering); 
  Serial.println("Starting Watering ... ");
  digitalWrite(PUMP, HIGH);
  
  return true; // repeat? true
}

bool OnTimer2_StopWatering(void *) 
{
  Serial.println("Stopping watering");
  digitalWrite(PUMP, LOW);
  return false; // repeat? true
}

result OnTimer2_StopWateringAndReset( void *)
{
   Serial.println("Stop watering and Reset");
   digitalWrite(PUMP, LOW);
   delay(100);
   reset();
   return 0;  //never reach this point
}

result startNow() {
   timer2_Watering.in(sec(wateringDuration), OnTimer2_StopWateringAndReset); 
   Serial.println("Force watering ... ");
   digitalWrite(PUMP, HIGH);
   //OnTimer1_Interval(NULL); //Do watering action imediately
//   timer1_Interval.remove(0);
   return 0;
}

result adjustWateringDuration() {
   Serial.println(String("adjustWateringDuration ")+String(wateringDuration));
}

result adjustIntervalTime() {
   Serial.println(String("adjustIntervalTime ") + String(IntervalTime));
}

result saveToEEPROM(){
   EEPROM.write(EEPROM_Duration,wateringDuration);   
   EEPROM.write(EEPROM_Interval,IntervalTime);
   return 0;
}

MENU(mainMenu, VERSION , doNothing, noEvent, wrapStyle,
   FIELD(wateringDuration, "Duration", "s", 1, 240, 1, 5, adjustWateringDuration, enterEvent, noStyle),
   FIELD(IntervalTime,     "Interval", "h", 1, 240, 1, 12, adjustIntervalTime, enterEvent, noStyle),
   OP("Save",saveToEEPROM,enterEvent),
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
  if (wateringDuration == 255 ) 
  {
     wateringDuration = 20;
     EEPROM.write(EEPROM_Duration,wateringDuration);   
        
  }
  if (IntervalTime == 255 ) 
  {
     IntervalTime = 24;
     EEPROM.write(EEPROM_Interval,IntervalTime);
  }
  Serial.begin(115200);
  Serial.println("=== Simple watering system ===");
  joystickBtns.begin();
  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  oled.begin(&Adafruit128x64, OLED_I2C_ADDRESS); //check config
  oled.setFont(menuFont);
  oled.clear();
  nav.idleTask = idle; //point a function to be used when menu is suspended
  timer1_Interval.every(hour(IntervalTime), OnTimer1_Interval);
  //timer1_Interval.every(mn(IntervalTime), OnTimer1_Interval);
}

void loop() {
  nav.poll();
  timer1_Interval.tick();
  timer2_Watering.tick();
}
