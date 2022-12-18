/* AVRIO
 * Example: Simple Voltmeter
 * This example xxx a simple voltmeter
 * using the readVCC function and the
 * analog Read method
 */
#include <AVRIO.h>
#include <Arduino.h>

AVRIO::Pin pin(A0);  // Instantiates an input pin on Arduino's pin A0

void setup() {
    Serial.begin(9600);
}
void loop() {
    uint32_t VREF = AVRIO::readVcc();                // Reads voltage on arduino's VCC
    float voltage = pin.analogRead() * VREF / 1023;  // Reads voltage on analog pin and converts to a voltage

    Serial.print(String(voltage) + "V");

    delay(500);
}