/* AVRIO
 * Example: Analog Write
 * This example shows how to instantiate
 * and use a pin on pwm mode
 */
#include <AVRIO.h>
#include <Arduino.h>

AVRIO::Pin pin(13, AVRIO::pin_m::Pwm);  // Instantiates a PWM output pin on Arduino's pin 13

void setup() {
}
void loop() {
    static unsigned char duty = 0;  // 8-bit duty cycle value
    pin.analogWrite(duty);          // Sets the pwm output to the specified duty cycle value
    duty++;                         // Increments the duty cycle value
    delay(250);                     // Waits for 250 ms
}