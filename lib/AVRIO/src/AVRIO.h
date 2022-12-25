#pragma once
#ifndef __AVRIO_H__
#define __AVRIO_H__

/****************************************
 * @author Felipe Ribeiro Lobato
 * @brief Optimized IO library for AVR Architecture Micro Controllers.
 * @section Compatibility
 * This library is compatible with the **avr** architecture so you should be able to use it on the following Arduino boards:
 * - [Arduino Micro](https://store.arduino.cc/arduino-micro)
 * - [Arduino Leonardo](https://store.arduino.cc/arduino-leonardo-with-headers)
 * - [Arduino Mega](https://store.arduino.cc/arduino-mega-2560-rev3)
 * - [Arduino Nano](https://store.arduino.cc/arduino-nano)
 * - [Arduino Uno](https://store.arduino.cc/arduino-uno-rev3)
 * - [Arduino Yún](https://store.arduino.cc/arduino-yun-rev-2)
 ****************************************/

#include <Arduino.h>
#include <ArxTypeTraits.h>

namespace AVRIO {
enum class input_m : uint8_t {
    Input = 0,
    InputPullup = 2,
};
enum class output_m : uint8_t {
    Output = 1,
    Pwm = 3
};
enum class pin_m : uint8_t {
    Input = 0,
    InputPullup = 2,
    Output = 1,
    Pwm = 3
};
enum class edge_t : uint8_t {
    None = 0,
    Low = 0,
    Change = 1,
    Falling = 2,
    Rising = 3
};
enum class write_t : uint8_t {
    Low = 0,
    High = 1,
    Toggle = 2
};
enum class bit_order : uint8_t {
    MSBFirst = 1,
    LSBFirst = 0
};
enum class aref_t : uint8_t {
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    Default = 0,
    External = 1,
    Internal1V1 = 2,
    Internal = 2
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    Default = 0,
    External = 4,
    Internal1V1 = 8,
    Internal = 8,
    Internal2V56 = 9,
    Internal2V56ExtCap = 13
#else
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || \
    defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
    Internal1V1 = 2,
    Internal2V56 = 3
#else
    Internal = 3,
#endif
    Default = 1,
    External = 0
#endif
};

/// @brief Reads the vcc voltage being fed to the arduino
/// @warning Consider a +-10% tolerance on the voltage returned by this function
/// @warning Always use a resistor between external voltage references and the AREF pin if using this function.
/// If a voltage source is currently directly connected to the AREF pin
/// this function will cause a short circuit and damage the aref pin or kill the microcontroller.
/// @returns The vcc voltage in millivolts
/// @code{.cpp}
/// // Voltmeter example
/// AVRIO::Pin pin(A0);
///
/// float VREF = AVRIO::readVcc() / 1000; // Sets the voltage reference
/// float voltage = pin.analogRead() * VREF / 1024; // Reads the voltage in A0
/// @endcode
uint32_t readVcc();

// Bunda
class Pin {
   private:
    byte arduinoPin;          ///< Arduino Pin
    byte pinMask;             ///< Port pin Mask
    volatile byte* portOut;   ///< Port output pointer
    volatile byte* portIn;    ///< Port input pointer
    volatile byte* portMode;  ///< Port mode pointer

    mutable pin_m mode;  ///< Pin mode

    mutable bool initFlag;  ///< Init function flag

    mutable byte fState;  ///< Falling Edge detection state
    mutable byte rState;  ///< Rising Edge detection state

    int8_t interruptNum;      ///< Pin's interrupt number
    bool isInterruptCapable;  ///< Flag indicating whether the pin is interrupt capable or not

    mutable bool pwmOn;  ///< Storage for PWM state (ON/OFF)
    bool isPWMCapable;   ///< Flag indicating whether the pin is PWM capable or not

    int8_t adcChannel;                ///< Analog Pin
    bool isADCCapable;                ///< Flag indicating whether the pin is ADC capable or not
    static uint8_t analog_reference;  ///< Arduino's analog reference type
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    const static uint8_t arefShift = 4;
#else
    const static uint8_t arefShift = 6;
#endif

   public:
    /// @brief Calls init method on all pins passed as arguments
    static void initializePins() {}
    /// @brief Calls init method on all pins passed as arguments
    /// @code{.cpp}
    /// AVRIO::Pin pin1(1, AVRIO::pin_m::Input);
    /// AVRIO::Pin pin2(2, AVRIO::pin_m::InputPullup);
    /// AVRIO::Pin pin3(3, AVRIO::pin_m::Output);
    /// AVRIO::Pin pin4(4, AVRIO::pin_m::Pwm);
    ///
    /// void setup() {
    ///    // Calls Pin::init on pin[1...4]
    ///    AVRIO::Pin::initializePins(pin1, pin2, pin3, pin4);
    /// }
    /// @endcode
    template <typename... Pins>
    static void initializePins(Pin pin, Pins... pins) {
        pin.init();
        initializePins(pins...);
    }

    /// @brief Sets the arduino's analog reference type
    /// @param type The type of reference the arduino will use
    /// @code{.cpp}
    /// AVRIO::Pin pin(1);
    /// // Sets analog reference to default (vcc)
    /// AVRIO::Pin::setAnalogReference(AVRIO::aref_t::Default);
    /// pin.analogRead();
    /// // Sets analog reference to internal (1v1 on Uno)
    /// AVRIO::Pin::setAnalogReference(AVRIO::aref_t::Internal);
    /// pin.analogRead();
    /// // Sets analog reference to external
    /// AVRIO::Pin::setAnalogReference(AVRIO::aref_t::External);
    /// pin.analogRead();
    /// @endcode
    static void setAnalogReference(aref_t type) {
        analog_reference = (uint8_t)type << arefShift;
        Arduino_h::analogReference((uint8_t)type);
    }

    /// @brief Gets the analog reference value currently being used
    /// @return The current analog reference value
    /// @code{.cpp}
    /// AVRIO::Pin::setAnalogReference(AVRIO::aref_t::Default);
    /// // Var analogReference will receive ((uint8_t)aref_t::Default << someNumber)
    /// uint8_t analogReference = AVRIO::Pin::getAnalogReference();
    /// @endcode
    static uint8_t getAnalogReference() {
        return analog_reference;
    }

    /// @brief Reads serial data in |i.e: from a shift register
    /// @tparam T The data storage type (works best with unsigned integer types)
    /// @tparam
    /// @param dataPin The data input pin
    /// @param clockPin The clock output pin
    /// @param bitOrder The bit order (LSBFirst|MSBFirst)
    /// @return The data read from the shift
    /// @code{.cpp}
    /// @endcode
    template <typename T = uint8_t, typename = std::enable_if_t<std::is_unsigned<T>::value>>
    static T shiftIn(Pin dataPin, Pin clockPin, bit_order bitOrder = bit_order::LSBFirst) {
        T value = 0;

        for (T i = 0; i < sizeof(T) * 8; i++) {
            clockPin.digitalWrite(write_t::High);
            if (bitOrder == bit_order::LSBFirst)
                value |= (T)dataPin.digitalRead() << i;
            else
                value |= (T)dataPin.digitalRead() << ((sizeof(T) * 8) - 1 - i);

            clockPin.digitalWrite(write_t::Low);
        }
        return value;
    }

    /// @brief Reads serial data in with a delay |i.e: from a shift register
    /// @tparam T The data storage type (works best with unsigned integer types)
    /// @tparam
    /// @param dataPin The data input pin
    /// @param clockPin The clock output pin
    /// @param bitOrder The bit order (LSBFirst|MSBFirst)
    /// @param pulseDelay The time between clock pulse's low and high state
    /// @return The data read from the shift register
    /// @code{.cpp}
    /// @endcode
    template <typename T = uint8_t, typename = std::enable_if_t<std::is_unsigned<T>::value>>
    static T shiftIn(Pin dataPin, Pin clockPin, bit_order bitOrder, uint64_t pulseDelay, bool ms = false) {
        T value = 0;

        for (T i = 0; i < sizeof(T) * 8; i++) {
            clockPin.digitalWrite(write_t::High);
            uint64_t t = (ms ? millis() : micros());
            while ((ms ? millis() : micros()) < t + pulseDelay) {
                _NOP();
            }
            if (bitOrder == bit_order::LSBFirst)
                value |= (T)dataPin.digitalRead() << i;
            else
                value |= (T)dataPin.digitalRead() << ((sizeof(T) * 8) - 1 - i);

            clockPin.digitalWrite(write_t::Low);
        }
        return value;
    }

    /// @brief Sends serial data out |i.e: to a shift register
    /// @tparam T The data storage type (works best with unsigned integer types)
    /// @tparam
    /// @param dataPin  The data output pin
    /// @param clockPin The clock output pin
    /// @param val The value to be sent
    /// @param bitOrder The bit order (LSBFirst|MSBFirst)
    /// @code{.cpp}
    /// @endcode
    template <typename T = uint8_t, typename = std::enable_if_t<std::is_unsigned<T>::value>>
    static void shiftOut(Pin dataPin, Pin clockPin, T val, bit_order bitOrder = bit_order::LSBFirst) {
        T mod = -1;
        mod = (mod / 2) + 1;
        for (uint8_t i = 0; i < sizeof(T) * 8; i++) {
            if (bitOrder == bit_order::LSBFirst) {
                dataPin.digitalWrite((AVRIO::write_t)(val & 1));
                val >>= 1;
            } else {
                dataPin.digitalWrite((AVRIO::write_t)((val & mod) != 0));
                val <<= 1;
            }
            clockPin.digitalWrite(AVRIO::write_t::High);
            clockPin.digitalWrite(AVRIO::write_t::Low);
        }
    }

    /// @brief Sends serial data out with delays |i.e: to a shift register
    /// @tparam T The data storage type (works best with unsigned integer types)
    /// @tparam
    /// @param dataPin  The data output pin
    /// @param clockPin The clock output pin
    /// @param val The value to be sent
    /// @param bitOrder The bit order (LSBFirst|MSBFirst)
    /// @param pulseDelay The time between clock pulse's low and high state
    /// @param dataDelay The time between each bit write
    /// @code{.cpp}
    /// @endcode
    template <typename T = uint8_t, typename = std::enable_if_t<std::is_unsigned<T>::value>>
    static void shiftOut(Pin dataPin, Pin clockPin, T val, bit_order bitOrder, uint64_t pulseDelay, uint64_t dataDelay, bool ms = false) {
        T mod = -1;
        mod = (mod / 2) + 1;
        for (uint8_t i = 0; i < sizeof(T) * 8; i++) {
            uint64_t t = ms ? millis() : micros();
            while ((ms ? millis() : micros()) < t + dataDelay) {
                _NOP();
            }
            if (bitOrder == bit_order::LSBFirst) {
                dataPin.digitalWrite((AVRIO::write_t)(val & 1));
                val >>= 1;
            } else {
                dataPin.digitalWrite((AVRIO::write_t)((val & mod) != 0));
                val <<= 1;
            }
            clockPin.digitalWrite(AVRIO::write_t::High);
            t = ms ? millis() : micros();
            while ((ms ? millis() : micros()) < t + pulseDelay) {
                _NOP();
            }
            clockPin.digitalWrite(AVRIO::write_t::Low);
        }
    }

    // Construtores
    Pin();

    /// @brief Pin Constructor
    /// @param pin The arduino pin
    /// @param mode The pin mode
    /// @code{.cpp}
    /// @endcode
    Pin(byte pin, pin_m mode = pin_m::Input);

    /// @brief Sets the pin's mode
    /// Only needed if the pin was declared at the global scope and
    /// pin mode was passed to the constructor
    /// @code{.cpp}
    /// @endcode
    void init() const;

    /// @brief Sets the pin's mode
    /// @param mode The Pin Mode
    /// @code{.cpp}
    /// Pin pin(1);
    /// pin.pinMode(INPUT); //Sets the pin as an input
    /// pin.pinMode(INPUT_PULLUP); //Sets the pin as an input with a pull-up resistor
    /// pin.pinMode(OUTPUT); //Sets the pin as an output
    /// pin.pinMode(PWM); //Sets the pin as a PWM output
    /// @endcode
    void pinMode(const pin_m& mode) const;

    /// @brief Reads a digital value on the pin
    /// @param mode Digital Read Mode
    /// @return Digital value [1] | [0]
    /// @code{.cpp}
    /// Pin pin(1, INPUT);
    /// int reading;
    /// reading = pin.digitalRead(); //Returns the pin's current state
    /// reading = pin.digitalRead(CHANGE); //Returns 1 whenever the pin's state changes
    /// reading = pin.digitalRead(FALLING); //Returns 1 when the pin's state goes from 1 to 0
    /// reading = pin.digitalRead(RISING); //Returns 1 when the pin's state goes from 0 to 1
    /// @endcode
    uint8_t digitalRead(const edge_t& mode = edge_t::None) const;

    /// @brief Writes a digital value to the pin [1, 0]
    /// @param state State to set the pin
    /// @code{.cpp}
    /// Pin pin(1, OUTPUT);
    /// pin.digitalWrite(LOW); //Sets the pin's state to low
    /// pin.digitalWrite(HIGH); //Sets the pin's state to high
    /// pin.digitalWrite(TOGGLE); //Toggles the pin's current state
    /// @endcode
    void digitalWrite(const write_t& state) const;

    /// @brief Reads an analog value through the arduino's adc
    /// @return 10 bit analog reading
    /// @code{.cpp}
    /// Pin pin(A0, INPUT);
    /// int reading = pin.analogRead(); //Returns a 10 bit analog reading
    /// @endcode
    uint16_t analogRead() const;

    /// @brief Non blocking version of analog read, works by starting the conversion when called,
    /// then polling the result in subsequent calls until the conversion is finished,
    /// then, when it is finished, calling the callback function passed as a parameter.
    /// @warning Do not use this function in multiple pins simultanously, this won't work since the arduino only has one ADC inside it.
    /// This method is written in a way that prevents this from breaking anything inside the arduino, but the reading result may come out wrong
    /// so it is preferred not to use it this way.
    /// @param callback The function that will be executed when the adc conversion is complete
    /// @return The status of the conversion, true if ended, false if polling
    /// @code{.cpp}
    /// @endcode
    bool asyncAnalogRead(std::function<void(uint16_t result)> callback) const;

    struct asyncADCReturnType {
        std::function<bool()> ready;
        std::function<uint16_t()> read;
    };
    /// @brief Non blocking version of analog read, starts the conversion when called
    /// and returns functions for checking if conversion is complete and for reading the conversion result.
    /// @returns A struct containing ready() and read() functions, ready() returns true when
    /// conversion is ready, and read() returns the conversion result.
    /// @code{.cpp}
    /// @endcode
    asyncADCReturnType asyncAnalogRead() const;

    /// @brief Writes an analog value through pwm
    /// @param val  8 bit 'pwm' value
    /// @code{.cpp}
    /// Pin pin(A0, INPUT);
    /// pin.analogWrite(127) //Sets the pwm's duty cycle to 50%
    /// @endcode
    void analogWrite(uint16_t val) const;

    /// @brief Attaches an interrupt routine to the pin
    /// @param callback The callback function
    /// @param mode The type of trigger
    /// @returns True if the pin is interrupt capable and False otherwise
    /// @code{.cpp}
    /// void foo(){
    ///    // Do something
    /// }
    /// Pin pin(2, INPUT);
    /// bool interruptAttached;
    /// interruptAttached = pin.attachInterrupt(LOW, foo); //Calls foo while the pin is low
    /// interruptAttached = pin.attachInterrupt(CHANGE, foo); //Calls foo whenever the pin' state changes
    /// interruptAttached = pin.attachInterrupt(FALLING, foo); //Calls foo when the pin goes low
    /// interruptAttached = pin.attachInterrupt(RISING, foo); //Calls foo when the pin goes high
    /// //interruptAttached will only be true if the pin is interrupt capable.
    /// @endcode
    bool attachInterrupt(edge_t mode, void (*callback)()) const;

    /// @brief Detaches the interrupt routine on the pin
    /// @returns True if the pin is interrupt capable and False otherwise
    /// @code{.cpp}
    /// void foo(){
    ///    // Do something
    /// }
    /// Pin pin(2, INPUT);
    /// bool interruptAttached = pin.attachInterrupt(LOW, foo); //Calls foo while the pin is low
    /// bool interruptDetached = pin.detachInterrupt(); //Removes the interrupt routine attached above
    /// //interruptDetached will only be true if the pin is interrupt capable.
    /// @endcode
    bool detachInterrupt() const;

    /// @brief Getter for the arduino pin number
    /// @return Arduino pin number
    /// @code{.cpp}
    /// Pin pin(1);
    /// //Dumb example but
    /// int pinNumber = pin.getPin(); //Assigns 1, integer used in the constructor, to 'pinNumber'
    /// digitalWrite(pin.getPin(), LOW); //Sets the pin's state to low using Arduino's digitalWrite()
    /// @endcode
    uint8_t getPin() const;

   protected:
    /// @brief Turns pwm on
    /// @return True if pin is pwm capable and False otherwise.
    bool turnOnPWM() const;

    /// @brief Turns
    /// pwm
    /// off
    /// @return True if pin is pwm capable and False otherwise.
    bool turnOffPWM() const;

   private:
    /// @brief Verifies if the pin is set as an input or input pull-up
    /// @return True if pin is an input and False otherwise
    bool isSetAsInput() const;
    void setADCRegisters() const;
    void startADCConversion() const;
};

// /// @brief Class representing a switch
// class Switch {
//    private:
//     Pin _pin;
//     mutable input_m i_mode;
//     mutable edge_t r_mode;

//    public:
//     Switch();
//     Switch(Pin, input_m = input_m::InputPullup, edge_t = edge_t::None);

//     void setInputMode(input_m) const;
//     void setReadMode(edge_t) const;
//     uint8_t read() const;
// };

}  // namespace AVRIO
#endif