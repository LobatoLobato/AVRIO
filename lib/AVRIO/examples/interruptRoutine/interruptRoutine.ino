/* AVRIO
 * Example: Interrupt Routine
 * This example shows how to attach and
 * detach interrupt routines with the Pin class
 */
#include <AVRIO.h>
#include <Arduino.h>

AVRIO::Pin pinA(2, AVRIO::pin_m::InputPullup);  // Instantiates an input pull-up pin on Arduino's pin 2
AVRIO::Pin pinB(3, AVRIO::pin_m::InputPullup);  // Instantiates an input pull-up pin on Arduino's pin 3
AVRIO::Pin pinC(13, AVRIO::pin_m::Output);      // Instantiates an output pin on Arduino's pin 13

void foo() {
    pinC.digitalWrite(AVRIO::write_t::High);  // Sets pinC's state to high
    delay(500);                               // Waits for 500ms
    pinC.digitalWrite(AVRIO::write_t::Low);   // Sets pinC's state to low
}
void bar() {
    pinA.detachInterrupt();  // Detaches the interrupt routine from pinA
}
void setup() {
    Serial.begin(9600);
    // Calls foo while pinA is on a low state
    pinA.attachInterrupt(AVRIO::edge_t::Low, foo);

    // Calls bar when a falling edge is detected on pinB
    // pinB.attachInterrupt(AVRIO::edge_t::Falling, bar);

    // Calls bar when a rising edge is detected on pinB
    // pinB.attachInterrupt(AVRIO::edge_t::Rising, bar);

    // Calls bar when a state change is detected on pinB
    // pinB.attachInterrupt(AVRIO::edge_t::Change, bar);
}
void loop() {
}