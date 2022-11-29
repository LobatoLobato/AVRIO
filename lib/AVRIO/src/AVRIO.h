#pragma once

#include <Arduino.h>

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
    Output = 1,
    InputPullup = 2,
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

/// @brief Class Representing An arduino Pin
class Pin {
   private:
    byte arduinoPin;          ///< Arduino Pin
    byte pinMask;             ///< Port pin Mask
    volatile byte* portOut;   ///< Port output pointer
    volatile byte* portIn;    ///< Port input pointer
    volatile byte* portMode;  ///< Port mode pointer

    mutable pin_m mode;  ///< Pin mode

    mutable byte fState;  ///< Falling Edge detection state
    mutable byte rState;  ///< Rising Edge detection state

    int8_t interruptNum;      ///< Pin's interrupt number
    bool isInterruptCapable;  ///< Flag indicating whether the pin is interrupt capable or not

    mutable bool pwmOn;  ///< Storage for pwm state (ON/OFF)
    bool isPWMCapable;   ///< Flag indicating whether the pin is pwm capable or not

   public:
    // Construtores
    Pin();

    /// @brief Pin Constructor
    /// @param pin The arduino pin
    /// @param mode The pin mode
    Pin(byte pin, pin_m mode = pin_m::Input);

    /// @brief Getter for the arduino pin val
    /// @return Arduino pin val
    uint8_t getPin() const;

    /// @brief Reads a digital value on the pin
    /// @param mode Digital Read Mode:
    /// @param 0: No Edge Detection,
    /// @param 1: On Change,
    /// @param 2: On Falling Edge,
    /// @param 3: On Rising Edge.
    /// @return Digital value [1] | [0]
    uint8_t digitalRead(const edge_t& mode = edge_t::Low) const;

    /// @brief Writes a digital value to the pin [1, 0]
    /// @param state State to set the pin
    /// @param 0: Low,
    /// @param 1: High,
    /// @param 2: Toggle.
    void digitalWrite(const write_t& state) const;

    /// @brief Sets the pin's mode
    /// @param mode The Pin Mode:
    /// @param 0: Input,
    /// @param 1: Output,
    /// @param 2: Input Pullup,
    /// @param 3: PWM Output.
    void pinMode(const pin_m& mode) const;

    /// @brief Reads an analog value through the arduino's adc
    /// @return 10 bit analog reading
    uint16_t analogRead() const;

    /// @brief Writes an analog value through pwm
    /// @param val  8 bit 'pwm' value
    void analogWrite(uint16_t val) const;

    /// @brief Attaches an interrupt routine to the pin
    /// @param callback The function to be called
    /// @param mode The type of trigger:
    /// @param LOW: Triggers while the pin is low
    /// @param FALLING: Triggers when the pin goes low
    /// @param RISING: Triggers when the pin goes high
    /// @param CHANGE: Triggers whenever the pin changes its state
    /// @returns True if the pin is interrupt capable and False otherwise
    bool attachInterrupt(edge_t mode, void (*callback)()) const;

    /// @brief Detaches the interrupt routine on the pin
    /// @returns True if the pin is interrupt capable and False otherwise
    bool detachInterrupt() const;

   protected:
    /// @brief Turns pwm on
    /// @return True if pin is pwm capable and False otherwise.
    bool turnOnPWM() const;

    /// @brief Turns pwm off
    /// @return True if pin is pwm capable and False otherwise.
    bool turnOffPWM() const;

   private:
    /// @brief Verifies if the pin is set as an input or input pull-up
    /// @return True if pin is an input and False otherwise
    bool isSetAsInput() const;
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
