/* AVRIO
 * Example: Digital Write
 * This example shows different ways to write a
 * blink program using the digitalWrite method
 */
#include <Arduino.h>
#include <Pin.h>

Pin pinA(2, OUTPUT); // Instantiates an output pin on Arduino's pin 2
Pin pinB(3, OUTPUT); // Instantiates an output pin on Arduino's pin 3
void setup()
{
  pinB.digitalWrite(HIGH); // Sets pinB's state to high
}

void loop()
{
  // Blinking pinA with toggle functionality
  pinA.digitalWrite(TOGGLE); // Toggles pinA's state
  // Blinking pinB the traditional way
  pinB.digitalWrite(LOW);  // Sets pinB's state to low
  delay(500);              // Wait for 500 ms
  pinB.digitalWrite(HIGH); // Sets pinB's state to high
  delay(500);              // Wait for 500 ms
}