/* AVRIO
 * Example: Analog read
 * This example shows how to read
 * a pin as an analog input
 */
#include <AVRIO.h>
#include <Arduino.h>

AVRIO::Pin pin(A0);  // Instantiates an input pin on Arduino's pin A0

void setup() {
    Serial.begin(9600);
}
void loop() {
    int reading = pin.analogRead();  // Reads the analog input
    Serial.println(reading);         // Prints the analog reading
    delay(1000);                     // Waits 1 second
}