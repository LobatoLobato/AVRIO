# AVRIO

## Headers

This library exposes the AVRIO.h header containing the AVRIO namespace with all classes and functions

## Description

Fast Pin IO library for AVR Architecture. Abstracts pins into a Pin class that stores register adresses when instantiated for faster and easier pin manipulation.

### Compatibility

This library is compatible with the **avr** architecture so you should be able to use it on the following Arduino boards:

- [Arduino Micro](https://store.arduino.cc/arduino-micro)
- [Arduino Leonardo](https://store.arduino.cc/arduino-leonardo-with-headers)
- [Arduino Mega](https://store.arduino.cc/arduino-mega-2560-rev3)
- [Arduino Nano](https://store.arduino.cc/arduino-nano)
- [Arduino Uno](https://store.arduino.cc/arduino-uno-rev3)
- [Arduino Yún](https://store.arduino.cc/arduino-yun-rev-2)

## Methods

>## `void pinMode(const uint8_t &mode) const;`
>
>Sets the pin's mode
>
>### Parameters:
>
>Mode: can assume four integer values.
>
>- 0 | INPUT: Input,
>- 1 | OUTPUT: Output,
>- 2 | INPUT_PULLUP: Input Pull-up
>- 3 | PWM: Pulse Width Modulation Output
>
>### Returns
>
>Nothing.
>
>### Usage
>
>```C++
>Pin pin(1);
>pin.pinMode(INPUT); //Sets the pin as an input
>pin.pinMode(INPUT_PULLUP); //Sets the pin as an input with a pull-up resistor
>pin.pinMode(OUTPUT); //Sets the pin as an output
>pin.pinMode(PWM); //Sets the pin as a PWM output
>```

>## `    uint8_t digitalRead(const uint8_t &mode = 0) const;`
>
>Reads a digital value on the pin
>
>### Parameters:
>
>Mode: can assume four integer values, defaults to 0.
>
>- 0: No Edge Detection,
>- 1 | CHANGE: On Change,
>- 2 | FALLING: On Falling Edge,
>- 3 | RISING: On Rising Edge.
>
>### Returns
>
>The digital reading as an 8-bit unsigned integer value.
>
>### Usage
>
>```C++
>Pin pin(1, INPUT);
>int reading;
>reading = pin.digitalRead(); //Returns the pin's current state
>reading = pin.digitalRead(CHANGE); //Returns 1 whenever the pin's state changes
>reading = pin.digitalRead(FALLING); //Returns 1 when the pin's state goes from 1 to 0
>reading = pin.digitalRead(RISING); //Returns 1 when the pin's state goes from 0 to 1
>```

>## ` void digitalWrite(const uint8_t &state) const;`
>
>Sets the pin's state to one of two options: High or Low
>
>### Parameters:
>
>State: Can assume three integer values.
>
>- 0 | LOW: Low,
>- 1 | HIGH: High,
>- 2 | TOGGLE: Toggles com vina.
>
>### Returns
>
>Nothing.
>
>### Usage
>
>```C++
>Pin pin(1, OUTPUT);
>pin.digitalWrite(LOW); //Sets the pin's state to low
>pin.digitalWrite(HIGH); //Sets the pin's state to high
>pin.digitalWrite(TOGGLE); //Toggles the pin's current state
>```

>## `uint16_t analogRead() const;`
>
>Reads an analog value through the pin's ADC.
>
>Pin must have an ADC.
>
>Currently this method calls the Arduino's analogRead(int pin) function as I don't understand how it works hence I can't rewrite it.
>
>Will have, in the future, a "resolution" parameter to select different reading resolutions.
>
>### Parameters:
>
>None for now…
>
>### Returns
>
>The analog reading as a 16-bit unsigned integer value 
>
>### Usage
>
>```C++
>Pin pin(A0, INPUT);
>int reading = pin.analogRead(); //Returns a 10 bit analog reading
>```

 

>## `void analogWrite(uint8_t duty) const;`
>
>Sends a pwm signal on the pin.
>
>Pin must be pwm capable.
>
>Currently this method calls the Arduino's analogWrite(int pin, int val) function as I don't understand how it works hence I can't rewrite it.
>
>Will have, in the future, a "resolution" and a "frequency" parameter to change, respectively, the pwm's resolution and frequency.
>
>### Parameters:
>
>Duty: accepts an 8 bit integer value
>
>- [0-255]: [0%-100%] duty cycle.
>
>### Returns
>
>Nothing.
>
>### Usage
>
>```C++
>Pin pin(A0, INPUT);
>pin.analogWrite(127) //Sets the pwm's duty cycle to 50%
>```

>## ` bool attachInterrupt(uint8_t mode, void (*callback)()) const;`
>
>Attaches an interrupt routine to the pin.
>
>Pin must be interrupt capable.
>
>### Parameters:
>
>- Callback: The function to be called when the interrupt is activated
>- Mode: Integer value representing the type of trigger the interrupt will use
>  - 0 | LOW: Triggers while the pin is low,
>  - 1 | *CHANGE*: Triggers whenever the pin changes its state,
>  - 2 | FALLING: Triggers when the pin goes low,
>  - 3 | RISING: Triggers when the pin goes high.
>
>### Returns
>
>True if the pin is interrupt capable and False otherwise.
>
>### Usage
>
>```C++
>void foo(){
>    // Do something
>}
>Pin pin(2, INPUT);
>bool interruptAttached;
>interruptAttached = pin.attachInterrupt(LOW, foo); //Calls foo while the pin is low
>interruptAttached = pin.attachInterrupt(CHANGE, foo); //Calls foo whenever the pin' state changes
>interruptAttached = pin.attachInterrupt(FALLING, foo); //Calls foo when the pin goes low
>interruptAttached = pin.attachInterrupt(RISING, foo); //Calls foo when the pin goes high
>//interruptAttached will only be true if the pin is interrupt capable.
>```

>## `bool detachInterrupt() const;`
>
>Detaches the interrupt routine on the pin
>
>### Parameters:
>
>None.
>
>### Returns
>
>True if the pin is interrupt capable and False otherwise.
>
>### Usage
>
>```C++
>void foo(){
>    // Do something
>}
>Pin pin(2, INPUT);
>bool interruptAttached = pin.attachInterrupt(LOW, foo); //Calls foo while the pin is low
>bool interruptDetached = pin.detachInterrupt(); //Removes the interrupt routine attached above
>//interruptDetached will only be true if the pin is interrupt capable.
>```

>## `  uint8_t getPin() const;`
>
>Gets the pin number used when instantiating this class so it can be used somewhere else like a library or something idk.
>
>### Parameters:
>
>None.
>
>### Returns
>
>The arduino equivalent pin number.
>
>### Usage
>
>```C++
>Pin pin(1);
>//Dumb example but 
>int pinNumber = pin.getPin(); //Assigns 1, integer used in the constructor, to 'pinNumber'
>digitalWrite(pin.getPin(), LOW); //Sets the pin's state to low using Arduino's digitalWrite()
>```

