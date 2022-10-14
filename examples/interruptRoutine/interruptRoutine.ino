/* AVRIO
 * Example: Interrupt Routine
 * This example shows how to attach and
 * detach interrupt routines with the Pin class
 */
#include <Arduino.h>
#include <Pin.h>

Pin pinA(2, INPUT_PULLUP); // Instantiates an input pull-up pin on Arduino's pin 2
Pin pinB(3, INPUT_PULLUP); // Instantiates an input pull-up pin on Arduino's pin 3
Pin pinC(13, OUTPUT);      // Instantiates an output pin on Arduino's pin 13

void foo()
{
  pinC.digitalWrite(HIGH); // Sets pinC's state to high
  delay(500);              // Waits for 500ms
  pinC.digitalWrite(LOW);  // Sets pinC's state to low
}
void bar()
{
  pinA.detachInterrupt(); // Detaches the interrupt routine from pinA
}
void setup()
{
  Serial.begin(9600);
  // Calls foo while pinA is on a low state
  pinA.attachInterrupt(LOW, foo);

  // Calls bar when a falling edge is detected on pinB
  // pinB.attachInterrupt(FALLING, bar);

  // Calls bar when a rising edge is detected on pinB
  // pinB.attachInterrupt(RISING, bar);

  // Calls bar when a state change is detected on pinB
  // pinB.attachInterrupt(CHANGE, bar);
}
void loop()
{
}