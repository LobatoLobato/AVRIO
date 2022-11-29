/* AVRIO
 * Example: Pin Mode
 * This example shows how to use the pinMode
 * method to change the pin's mode
 */
#include <AVRIO.h>
#include <Arduino.h>

AVRIO::Pin pin(13);  // Instantiates a pin on Arduino's pin 13

void setup() {
    pin.pinMode(AVRIO::pin_m::Input);        // Sets pin's mode as output
    pin.pinMode(AVRIO::pin_m::InputPullup);  // Sets pin's mode as input pull-up
    pin.pinMode(AVRIO::pin_m::Output);       // Sets pin's mode as output
    pin.pinMode(AVRIO::pin_m::Pwm);          // Sets pin's mode as pwm output
}
void loop() {
}