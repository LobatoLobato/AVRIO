/* AVRIO
 * Example: Digital Read
 * This example shows how to use the different modes
 * of reading digital inputs with digitalRead method
 */

#include <Arduino.h>
#include <Pin.h>

Pin pinA(2, INPUT);        // Instantiates an input pin on Arduino's pin 2
Pin pinB(3, INPUT_PULLUP); // Instantiates an input pull-up pin on Arduino's pin 3
Pin pinC(4, INPUT_PULLUP); // Instantiates an input pull-up pin on Arduino's pin 4
Pin pinD(5, INPUT_PULLUP); // Instantiates an input pull-up pin on Arduino's pin 5

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (pinA.digitalRead())
  {
    Serial.println("Pin A is reading HIGH");
  }
  else
  {
    Serial.println("Pin A is reading LOW");
  }
  if (pinB.digitalRead(CHANGE))
  {
    Serial.println("Pin B changed state");
  }
  if (pinC.digitalRead(FALLING))
  {
    Serial.println("Pin C detected a falling edge");
  }
  if (pinD.digitalRead(RISING))
  {
    Serial.println("Pin D detected a rising edge");
  }
}