/* AVRIO
 * Example: Digital Read
 * This example shows how to use the different modes
 * of reading digital inputs with digitalRead method
 */

#include <AVRIO.h>
#include <Arduino.h>

AVRIO::Pin pinA(2, AVRIO::pin_m::Input);        // Instantiates an input pin on Arduino's pin 2
AVRIO::Pin pinB(3, AVRIO::pin_m::InputPullup);  // Instantiates an input pull-up pin on Arduino's pin 3
AVRIO::Pin pinC(4, AVRIO::pin_m::InputPullup);  // Instantiates an input pull-up pin on Arduino's pin 4
AVRIO::Pin pinD(5, AVRIO::pin_m::InputPullup);  // Instantiates an input pull-up pin on Arduino's pin 5

void setup() {
    Serial.begin(9600);
}

void loop() {
    if (pinA.digitalRead()) {
        Serial.println("Pin A is reading HIGH");
    } else {
        Serial.println("Pin A is reading LOW");
    }
    if (pinB.digitalRead(AVRIO::edge_t::Change)) {
        Serial.println("Pin B changed state");
    }
    if (pinC.digitalRead(AVRIO::edge_t::Falling)) {
        Serial.println("Pin C detected a falling edge");
    }
    if (pinD.digitalRead(AVRIO::edge_t::Rising)) {
        Serial.println("Pin D detected a rising edge");
    }
}