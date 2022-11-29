/* AVRIO
 * Example: Digital Write
 * This example shows different ways to write a
 * blink program using the digitalWrite method
 */
#include <AVRIO.h>
#include <Arduino.h>

AVRIO::Pin pinA(2, AVRIO::pin_m::Output);  // Instantiates an output pin on Arduino's pin 2
AVRIO::Pin pinB(3, AVRIO::pin_m::Output);  // Instantiates an output pin on Arduino's pin 3
void setup() {
    pinB.digitalWrite(AVRIO::write_t::High);  // Sets pinB's state to high
}

void loop() {
    // Blinking pinA with toggle functionality
    pinA.digitalWrite(AVRIO::write_t::Toggle);  // Toggles pinA's state
    // Blinking pinB the traditional way
    pinB.digitalWrite(AVRIO::write_t::Low);   // Sets pinB's state to low
    delay(500);                               // Wait for 500 ms
    pinB.digitalWrite(AVRIO::write_t::High);  // Sets pinB's state to high
    delay(500);                               // Wait for 500 ms
}