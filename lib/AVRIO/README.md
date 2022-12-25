# AVRIO

## Headers

This library exposes the AVRIO.h header containing the AVRIO namespace.

## Description

Optimized IO library for AVR Architecture Micro Controllers.

### Compatibility

This library is compatible with the **avr** architecture so you should be able to use it on the following Arduino boards:

- [Arduino Micro](https://store.arduino.cc/arduino-micro)
- [Arduino Leonardo](https://store.arduino.cc/arduino-leonardo-with-headers)
- [Arduino Mega](https://store.arduino.cc/arduino-mega-2560-rev3)
- [Arduino Nano](https://store.arduino.cc/arduino-nano)
- [Arduino Uno](https://store.arduino.cc/arduino-uno-rev3)
- [Arduino Yún](https://store.arduino.cc/arduino-yun-rev-2)

## Types

> ## `enum class AVRIO::input_m : uint8_t;`

> ## `enum class AVRIO::output_m : uint8_t;`

> ## `enum class AVRIO::pin_m : uint8_t;`

> ## `enum class AVRIO::edge_t : uint8_t;`

> ## `enum class AVRIO::write_t : uint8_t;`

> ## `enum class AVRIO::bit_order : uint8_t;`

> ## `enum class AVRIO::aref_t : uint8_t;`

> ## `struct AVRIO::Pin::asyncADCReturnType;`

## Functions

> ## `uint32_t AVRIO::readVcc();`
>
> Reads the vcc voltage being fed to the arduino
>
> ### Warning
>
> - Consider a +-10% tolerance on the voltage returned by this function
> - Always use a resistor between external voltage references and the AREF pin if using this function. If a voltage source is currently directly connected to the AREF pin this function will cause a short circuit and damage the aref pin or kill the microcontroller.
>
> ### Parameters:
>
> None
>
> ### Returns
>
> The vcc voltage in millivolts
>
> ### Usage
>
> ```cpp
> // Voltmeter example
> AVRIO::Pin pin(A0);
>
> float VREF = AVRIO::readVcc() / 1000; // Sets the voltage reference
> float voltage = pin.analogRead() * VREF / 1024; // Reads the voltage in A0
> ```
>
> ㅤ

## Classes

> ## AVRIO::Pin
>
> > ## `static void initializePins();`
> >
> > Calls init method on all pins passed as arguments
> >
> > ### Parameters:
> >
> > None
> >
> > ### Returns
> >
> > Nothing
> >
> > ### Usage
> >
> > No examples here
>
> > ## `static void initializePins(Pin pin, Pins... pins);`
> >
> > Calls init method on all pins passed as arguments
> >
> > ### Parameters:
> >
> > None
> >
> > ### Returns
> >
> > Nothing
> >
> > ### Usage
> >
> > ```cpp
> > AVRIO::Pin pin1(1, AVRIO::pin_m::Input);
> > AVRIO::Pin pin2(2, AVRIO::pin_m::InputPullup);
> > AVRIO::Pin pin3(3, AVRIO::pin_m::Output);
> > AVRIO::Pin pin4(4, AVRIO::pin_m::Pwm);
> >
> > void setup() {
> >   // Calls Pin::init on pin[1...4]
> >   AVRIO::Pin::initializePins(pin1, pin2, pin3, pin4);
> > }
> > ```
> >
> > ㅤ
>
> > ## `static void setAnalogReference(aref_t type);`
> >
> > Sets the arduino's analog reference type
> >
> > ### Parameters:
> >
> > - `type`: The type of reference the arduino will use
> >
> > ### Returns
> >
> > Nothing
> >
> > ### Usage
> >
> > ```cpp
> > AVRIO::Pin pin(1);
> > // Sets analog reference to default (vcc)
> > AVRIO::Pin::setAnalogReference(AVRIO::aref_t::Default);
> > pin.analogRead();
> > // Sets analog reference to internal (1v1 on Uno)
> > AVRIO::Pin::setAnalogReference(AVRIO::aref_t::Internal);
> > pin.analogRead();
> > // Sets analog reference to external
> > AVRIO::Pin::setAnalogReference(AVRIO::aref_t::External);
> > pin.analogRead();
> > ```
> >
> > ㅤ
>
> > ## `static uint8_t getAnalogReference();`
> >
> > Gets the analog reference value currently being used
> >
> > ### Parameters:
> >
> > None
> >
> > ### Returns
> >
> > The current analog reference value
> >
> > ### Usage
> >
> > ```cpp
> > AVRIO::Pin::setAnalogReference(AVRIO::aref_t::Default);
> > // Var analogReference will receive ((uint8_t)aref_t::Default << someNumber)
> > uint8_t analogReference = AVRIO::Pin::getAnalogReference();
> > ```
> >
> > ㅤ
>
> > ## `static T shiftIn(Pin dataPin, Pin clockPin, bit_order bitOrder = bit_order::LSBFirst);`
> >
> > Reads serial data in |i.e: from a shift register
> >
> > ### Parameters:
> >
> > - `dataPin`: The data input pin
> > - `clockPin`: The clock output pin
> > - `bitOrder`: The bit order (LSBFirst|MSBFirst)
> >
> > ### Returns
> >
> > The data read from the shift
> >
> > ### Usage
> >
> > ```cpp
> >
> > ```
> >
> > ㅤ
>
> > ## `static T shiftIn(Pin dataPin, Pin clockPin, bit_order bitOrder, uint64_t pulseDelay, bool ms = false);`
> >
> > Reads serial data in with a delay |i.e: from a shift register
> >
> > ### Parameters:
> >
> > - `dataPin`: The data input pin
> > - `clockPin`: The clock output pin
> > - `bitOrder`: The bit order (LSBFirst|MSBFirst)
> > - `pulseDelay`: The time between clock pulse's low and high state
> >
> > ### Returns
> >
> > The data read from the shift register
> >
> > ### Usage
> >
> > ```cpp
> >
> > ```
> >
> > ㅤ
>
> > ## `static void shiftOut(Pin dataPin, Pin clockPin, T val, bit_order bitOrder = bit_order::LSBFirst);`
> >
> > Sends serial data out |i.e: to a shift register
> >
> > ### Parameters:
> >
> > - `dataPin`: The data output pin
> > - `clockPin`: The clock output pin
> > - `val`: The value to be sent
> > - `bitOrder`: The bit order (LSBFirst|MSBFirst)
> >
> > ### Returns
> >
> > Nothing
> >
> > ### Usage
> >
> > ```cpp
> >
> > ```
> >
> > ㅤ
>
> > ## `static void shiftOut(Pin dataPin, Pin clockPin, T val, bit_order bitOrder, uint64_t pulseDelay, uint64_t dataDelay, bool ms = false);`
> >
> > Sends serial data out with delays |i.e: to a shift register
> >
> > ### Parameters:
> >
> > - `dataPin`: The data output pin
> > - `clockPin`: The clock output pin
> > - `val`: The value to be sent
> > - `bitOrder`: The bit order (LSBFirst|MSBFirst)
> > - `pulseDelay`: The time between clock pulse's low and high state
> > - `dataDelay`: The time between each bit write
> >
> > ### Returns
> >
> > Nothing
> >
> > ### Usage
> >
> > ```cpp
> >
> > ```
> >
> > ㅤ
>
> > ## `void init() const;`
> >
> > Pin Constructor
> > Sets the pin's mode Only needed if the pin was declared at the global scope and pin mode was passed to the constructor
> >
> > ### Parameters:
> >
> > - `pin`: The arduino pin
> > - `mode`: The pin mode
> >
> > ### Returns
> >
> > Nothing
> >
> > ### Usage
> >
> > ```cpp
> >
> > ```
> >
> > ㅤ
>
> > ## `void pinMode(const pin_m& mode) const;`
> >
> > Sets the pin's mode
> >
> > ### Parameters:
> >
> > - `mode`: The Pin Mode
> >
> > ### Returns
> >
> > Nothing
> >
> > ### Usage
> >
> > ```cpp
> > Pin pin(1);
> > pin.pinMode(INPUT); //Sets the pin as an input
> > pin.pinMode(INPUT_PULLUP); //Sets the pin as an input with a pull-up resistor
> > pin.pinMode(OUTPUT); //Sets the pin as an output
> > pin.pinMode(PWM); //Sets the pin as a PWM output
> > ```
> >
> > ㅤ
>
> > ## `uint8_t digitalRead(const edge_t& mode = edge_t::None) const;`
> >
> > Reads a digital value on the pin
> >
> > ### Parameters:
> >
> > - `mode`: Digital Read Mode
> >
> > ### Returns
> >
> > Digital value [1] | [0]
> >
> > ### Usage
> >
> > ```cpp
> > Pin pin(1, INPUT);
> > int reading;
> > reading = pin.digitalRead(); //Returns the pin's current state
> > reading = pin.digitalRead(CHANGE); //Returns 1 whenever the pin's state changes
> > reading = pin.digitalRead(FALLING); //Returns 1 when the pin's state goes from 1 to 0
> > reading = pin.digitalRead(RISING); //Returns 1 when the pin's state goes from 0 to 1
> > ```
> >
> > ㅤ
>
> > ## `void digitalWrite(const write_t& state) const;`
> >
> > Writes a digital value to the pin [1, 0]
> >
> > ### Parameters:
> >
> > - `state`: State to set the pin
> >
> > ### Returns
> >
> > Nothing
> >
> > ### Usage
> >
> > ```cpp
> > Pin pin(1, OUTPUT);
> > pin.digitalWrite(LOW); //Sets the pin's state to low
> > pin.digitalWrite(HIGH); //Sets the pin's state to high
> > pin.digitalWrite(TOGGLE); //Toggles the pin's current state
> > ```
> >
> > ㅤ
>
> > ## `uint16_t analogRead() const;`
> >
> > Reads an analog value through the arduino's adc
> >
> > ### Parameters:
> >
> > None
> >
> > ### Returns
> >
> > 10 bit analog reading
> >
> > ### Usage
> >
> > ```cpp
> > Pin pin(A0, INPUT);
> > int reading = pin.analogRead(); //Returns a 10 bit analog reading
> > ```
> >
> > ㅤ
>
> > ## `bool asyncAnalogRead(std::function<void(uint16_t result)> callback) const;`
> >
> > Non blocking version of analog read, works by starting the conversion when called, then polling the result in subsequent calls until the conversion is finished, then, when it is finished, calling the callback function passed as a parameter.
> >
> > ### Warning
> >
> > - Do not use this function in multiple pins simultanously, this won't work since the arduino only has one ADC inside it. This method is written in a way that prevents this from breaking anything inside the arduino, but the reading result may come out wrong so it is preferred not to use it this way.
> >
> > ### Parameters:
> >
> > - `callback`: The function that will be executed when the adc conversion is complete
> >
> > ### Returns
> >
> > The status of the conversion, true if ended, false if polling
> >
> > ### Usage
> >
> > ```cpp
> >
> > ```
> >
> > ㅤ
>
> > ## `asyncADCReturnType asyncAnalogRead() const;`
> >
> > Non blocking version of analog read, starts the conversion when called and returns functions for checking if conversion is complete and for reading the conversion result.
> >
> > ### Parameters:
> >
> > None
> >
> > ### Returns
> >
> > A struct containing ready() and read() functions, ready() returns true when conversion is ready, and read() returns the conversion result.
> >
> > ### Usage
> >
> > ```cpp
> >
> > ```
> >
> > ㅤ
>
> > ## `void analogWrite(uint16_t val) const;`
> >
> > Writes an analog value through pwm
> >
> > ### Parameters:
> >
> > - `val`: 8 bit 'pwm' value
> >
> > ### Returns
> >
> > Nothing
> >
> > ### Usage
> >
> > ```cpp
> > Pin pin(A0, INPUT);
> > pin.analogWrite(127) //Sets the pwm's duty cycle to 50%
> > ```
> >
> > ㅤ
>
> > ## `bool attachInterrupt(edge_t mode, void (*callback)()) const;`
> >
> > Attaches an interrupt routine to the pin
> >
> > ### Parameters:
> >
> > - `callback`: The callback function
> > - `mode`: The type of trigger
> >
> > ### Returns
> >
> > True if the pin is interrupt capable and False otherwise
> >
> > ### Usage
> >
> > ```cpp
> > void foo(){
> >   // Do something
> > }
> > Pin pin(2, INPUT);
> > bool interruptAttached;
> > interruptAttached = pin.attachInterrupt(LOW, foo); //Calls foo while the pin is low
> > interruptAttached = pin.attachInterrupt(CHANGE, foo); //Calls foo whenever the pin' state changes
> > interruptAttached = pin.attachInterrupt(FALLING, foo); //Calls foo when the pin goes low
> > interruptAttached = pin.attachInterrupt(RISING, foo); //Calls foo when the pin goes high
> > //interruptAttached will only be true if the pin is interrupt capable.
> > ```
> >
> > ㅤ
>
> > ## `bool detachInterrupt() const;`
> >
> > Detaches the interrupt routine on the pin
> >
> > ### Parameters:
> >
> > None
> >
> > ### Returns
> >
> > True if the pin is interrupt capable and False otherwise
> >
> > ### Usage
> >
> > ```cpp
> > void foo(){
> >   // Do something
> > }
> > Pin pin(2, INPUT);
> > bool interruptAttached = pin.attachInterrupt(LOW, foo); //Calls foo while the pin is low
> > bool interruptDetached = pin.detachInterrupt(); //Removes the interrupt routine attached above
> > //interruptDetached will only be true if the pin is interrupt capable.
> > ```
> >
> > ㅤ
>
> > ## `uint8_t getPin() const;`
> >
> > Getter for the arduino pin number
> >
> > ### Parameters:
> >
> > None
> >
> > ### Returns
> >
> > Arduino pin number
> >
> > ### Usage
> >
> > ```cpp
> > Pin pin(1);
> > //Dumb example but
> > int pinNumber = pin.getPin(); //Assigns 1, integer used in the constructor, to 'pinNumber'
> > digitalWrite(pin.getPin(), LOW); //Sets the pin's state to low using Arduino's digitalWrite()
> > ```
> >
> > ㅤ
