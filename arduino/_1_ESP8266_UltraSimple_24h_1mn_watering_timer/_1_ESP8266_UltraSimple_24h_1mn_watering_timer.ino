/*
    ESP8266 D1 Board as irrigation timer
*/

/*Electronic
 *                      Vin 12V -----------+------------------O
 *                                         |
 *                                       ##### 470uF/25V
 *                                       -----                         PUMP
 *                                         |
 *                                        ---
 * 
 *                         BD137
 *                           +-------+------------------------O 
 *          220R             |       |
 *             ____        |/        | 
 * D2 --------|____|-------|        ____
 *                         |\>       /\   1N4002
 *                           |       |
 *                          ---     --- 
 */

//Target WeMos D1 R1 (ESP8266)

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(D2, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  
}

void delay_one_min(void)
{
   delay(60 * 1000);
}

void delay_min(unsigned int m)
{
   for (unsigned int i = 0; i<m; i++)
      delay_one_min();
}

void delay_hour(unsigned int h)
{
   for (unsigned int i = 0; i<h; i++)
      delay_min(60);
}

// the loop function runs over and over again forever
void loop() {
   
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  digitalWrite(D2, HIGH);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is active low on the ESP-01)
  delay_min(1);            
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  digitalWrite(D2, LOW);  // Turn the LED off by making the voltage HIGH
                       // Wait for two seconds (to demonstrate the active low LED)
  delay_hour(24);
}
