/* AVRIO
 * Example: Pin Mode
 * This example shows how to use the pinMode
 * method to change the pin's mode
 */
#include <Arduino.h>
#include <Pin.h>

Pin pin(13); // Instantiates a pin on Arduino's pin 13

void setup()
{
  pin.pinMode(INPUT);        // Sets pin's mode as output
  pin.pinMode(INPUT_PULLUP); // Sets pin's mode as input pull-up
  pin.pinMode(OUTPUT);       // Sets pin's mode as output
  pin.pinMode(PWM);          // Sets pin's mode as pwm output
}
void loop()
{
}