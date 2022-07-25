/*
 * timer_blink
 *
 * Blinks the built-in LED every second using the arduino-timer library.
 *
 */
#define TIMER_MAX_TASKS 2  //save memory
#include <timer.h> // https://github.com/contrem/arduino-timer  available from Arduino library management (arduino-timer).

auto timer1 = timer_create_default(); // create a timer with default settings
auto timer2 = timer_create_default(); // create a timer with default settings

unsigned long t1 = 1000; //ms
unsigned long t2 = 5000; //ms

unsigned long mn(unsigned long  mn){
   return mn * 60 * 1000;
}

unsigned long hour(unsigned long  h){
   return h * 3600 * 1000;
}

unsigned long day(unsigned long  d){
   return d * 3600 * 1000 *24;
}

bool OnTimer1(void *) 
{
  Serial.println("Hello timer 1");
  return true; // repeat? true
}

bool OnTimer2(void *) 
{
  Serial.println("Hello timer          2");
  return true; // repeat? true
}

void setup() 
{
  Serial.begin(115200);
  Serial.println("Start");
  timer1.every(mn(1),  OnTimer1); //1mn
  timer2.every(mn(10), OnTimer2); //10mn
}

void loop() 
{
  timer1.tick(); // tick the timer
  timer2.tick(); // tick the timer
}

