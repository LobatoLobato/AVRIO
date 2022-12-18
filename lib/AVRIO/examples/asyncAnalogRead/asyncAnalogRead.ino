/* AVRIO
 * Example: Async Analog read
 * This example shows how to read
 * a pin as an analog input asynchronously
 */
#include <AVRIO.h>
#include <Arduino.h>

AVRIO::Pin pin(A0);  // Instantiates an input pin on Arduino's pin A0

void setup() {
    Serial.begin(9600);
}
void loop() {
    // There's two ways of using the asynchronous version of analog read
    // With a callback
    if (true) {
        pin.asyncAnalogRead([](uint16_t result) {
            Serial.println(result);
        });
    }
    // Dividing the ADC conversion in three steps
    else {
        auto ar = pin.asyncAnalogRead();
        if (ar.ready()) {
            Serial.println(ar.read());
        }
    }
    delay(1000);
}