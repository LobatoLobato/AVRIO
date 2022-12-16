#include "AVRIO.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

int8_t pinToChannel(uint8_t pin) {
#if defined(analogPinToChannel)
    return pin >= A0 ? analogPinToChannel(pin - A0) : -1;
#else
    return pin >= A0 ? pin -= A0 : -1;
#endif
}

namespace AVRIO {
uint8_t Pin::analog_reference = (uint8_t)aref_t::Default << Pin::arefShift;

Pin::Pin() {}

Pin::Pin(byte pin, pin_m mode) {
    uint8_t port = digitalPinToPort(pin);  // Converts the pin number to the pin port

    this->portMode = portModeRegister(port);   // Gets the port mode register
    this->portOut = portOutputRegister(port);  // Gets the port output register
    this->portIn = portInputRegister(port);    // Gets the port input register

    this->pinMask = digitalPinToBitMask(pin);  // Gets the pin bit mask

    this->interruptNum = digitalPinToInterrupt(pin);                // Gets the pin's interrupt number or -1 if it's not interrupt capable
    this->isInterruptCapable = (interruptNum != NOT_AN_INTERRUPT);  // Sets the interrupt capable flag

    this->isPWMCapable = digitalPinHasPWM(pin);  // Sets the pwm capable flag
    this->pwmOn = false;                         // Initializes PWM ON flag

    this->adcChannel = pinToChannel(pin);   // Stores the analog pin channel
    this->isADCCapable = adcChannel != -1;  // Sets the adc capable flag

    this->arduinoPin = pin;  // Stores the arduino pin passed in the constructor
    this->fState = 0;        // Sets the initial falling edge state
    this->rState = 1;        // Sets the initial rising edge state

    this->mode = mode;  // Stores the pin mode

    this->initFlag = false;  // Initializes initFlag to false
}

void Pin::init() const {
    this->initFlag = true;
    pinMode(this->mode);
    this->initFlag = false;
}

void Pin::pinMode(const pin_m& mode) const {
    if (!initFlag && mode == this->mode)  // If mode is equal to current mode return from the function
        return;

    if (this->mode == pin_m::Pwm)
        turnOffPWM();  // Turns off PWM on the pin

    this->mode = mode;  // Stores new mode

    byte oldSREG = SREG;  // Stores the status register

    noInterrupts();  // Disables interrupts

    switch (mode) {
        case pin_m::Input:  ///< Sets the pin as an input pin
            *portMode &= ~pinMask;
            *portOut &= ~pinMask;
            break;
        case pin_m::InputPullup:  ///< Sets the pin as an input pullup pin
            *portMode &= ~pinMask;
            *portOut |= pinMask;
            break;
        case pin_m::Output:  ///< Sets the pin as an output pin
            *portMode |= pinMask;
            *portOut &= ~pinMask;
            break;
        case pin_m::Pwm:  ///< Sets the pin as a PWM output pin
            if (turnOnPWM()) {
                // Sets pin as output pin
                *portMode |= pinMask;
                *portOut &= ~pinMask;
            } else {
                // Sets pin as input pin
                *portMode &= ~pinMask;
                *portOut &= ~pinMask;
            }
            break;
    }

    SREG = oldSREG;  // Sets the status register to stored value
    interrupts();    // Enables interrupts
}

uint8_t Pin::digitalRead(const edge_t& mode) const {
    uint8_t oldSREG = SREG;  // Stores the status register
    noInterrupts();          // Disables interrupts

    byte reading = (*portIn & pinMask) ? 1 : 0;  // The digital pin reading

    switch (mode) {
        case edge_t::Falling:  // Detects a falling edge on the pin's state
            fState = (3 & fState << 1) | (reading);
            return fState == 2;
        case edge_t::Rising:  // Detects a rising edge on the pin's state
            rState = (3 & rState << 1) | (reading);
            return rState == 1;
        case edge_t::Change:  // Detects a change on the pin's state
            rState = (3 & rState << 1) | (reading);
            fState = (3 & fState << 1) | (reading);
            return (fState == 2 || rState == 1);
        default:  // Returns the pin's state as is
            return reading;
    }
    SREG = oldSREG;  // Sets the status register to stored value
    interrupts();    // Enables interrupts
}

void Pin::digitalWrite(const write_t& state) const {
    uint8_t oldSREG = SREG;  // Stores the status register
    noInterrupts();          // Disables interrupts

    switch (state) {
        case write_t::Low:  ///< Sets the pin's state to low
            *portOut &= ~pinMask;
            break;
        case write_t::High:  ///< Sets the pin's state to high
            *portOut |= pinMask;
            break;
        case write_t::Toggle:  ///< Toggles the pin's state
            *portOut ^= pinMask;
            break;
    }

    SREG = oldSREG;  // Sets the status register to stored value
    interrupts();    // Enables interrupts
}

uint8_t Pin::getPin() const {
    return this->arduinoPin;
}

bool Pin::attachInterrupt(edge_t mode, void (*callback)()) const {
    if (this->isInterruptCapable && this->isSetAsInput()) {
        EIFR = 0x01;
        Arduino_h::attachInterrupt(this->interruptNum, callback, (int)mode);
    }

    return isInterruptCapable;
}

bool Pin::detachInterrupt() const {
    if (this->isInterruptCapable) {
        Arduino_h::detachInterrupt(interruptNum);
    }

    return isInterruptCapable;
}
uint16_t Pin::analogRead() const {
    if (this->pwmOn || !this->isADCCapable)  // If pwm is on return
        return 0;

    // Sets adc registers
    this->setADCRegisters();

    // Starts the conversion
    sbi(ADCSRA, ADSC);

    // Waits for the conversion to finish
    while (bit_is_set(ADCSRA, ADSC))
        ;

    uint8_t lowByte = ADCL;   // Reads ADC L byte
    uint8_t highByte = ADCH;  // Reads ADC H byte

    // Combine the two bytes and return reading
    return (highByte << 8) | lowByte;
}

bool Pin::asyncAnalogRead(std::function<void(uint16_t reading)> callback) const {
    // If PWM is on or pin does not have an ADC return
    if (this->pwmOn || !this->isADCCapable) {
        callback(0);
        return true;
    }
    static bool polling = false;
    static int8_t busy = -1;

    if (!polling && busy == -1) {
        // Set the registers
        this->setADCRegisters();
        // Start the conversion
        sbi(ADCSRA, ADSC);
        polling = true;
        busy = this->arduinoPin;
    }
    if (busy == this->arduinoPin && polling && !bit_is_set(ADCSRA, ADSC)) {
        uint8_t lowByte = ADCL, highByte = ADCH;
        callback((highByte << 8) | lowByte);
        polling = false;
        busy = -1;
        return true;
    }
    return false;
}
void Pin::analogWrite(uint16_t val) const {
    if (!this->pwmOn)
        return;

    // Arduino's analogWrite is already sufficiently fast
    // No need to rewrite it for now
    Arduino_h::analogWrite(this->arduinoPin, val);
}

void Pin::setADCRegisters() const {
#if defined(ADCSRB) && defined(MUX5)
    // the MUX5 bit of ADCSRB selects whether we're reading from channels
    // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
    ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((this->adcChannel >> 3) & 0x01) << MUX5);
#endif
    // set the analog reference and select the channel
#if defined(ADMUX)
    ADMUX = (Pin::analog_reference) | (this->adcChannel & 0x07);
#endif
}

bool Pin::turnOnPWM() const {
    if (!this->isPWMCapable || this->isSetAsInput()) {
        return false;
    }

    this->pwmOn = true;

    return true;
}

bool Pin::turnOffPWM() const {
    if (!this->isPWMCapable || !this->pwmOn) {
        return false;
    }

    uint8_t timer = digitalPinToTimer(this->arduinoPin);

    switch (timer) {
#if defined(TCCR1A) && defined(COM1A1)
        case TIMER1A:
            cbi(TCCR1A, COM1A1);
            break;
#endif
#if defined(TCCR1A) && defined(COM1B1)
        case TIMER1B:
            cbi(TCCR1A, COM1B1);
            break;
#endif
#if defined(TCCR1A) && defined(COM1C1)
        case TIMER1C:
            cbi(TCCR1A, COM1C1);
            break;
#endif

#if defined(TCCR2) && defined(COM21)
        case TIMER2:
            cbi(TCCR2, COM21);
            break;
#endif

#if defined(TCCR0A) && defined(COM0A1)
        case TIMER0A:
            cbi(TCCR0A, COM0A1);
            break;
#endif

#if defined(TCCR0A) && defined(COM0B1)
        case TIMER0B:
            cbi(TCCR0A, COM0B1);
            break;
#endif
#if defined(TCCR2A) && defined(COM2A1)
        case TIMER2A:
            cbi(TCCR2A, COM2A1);
            break;
#endif
#if defined(TCCR2A) && defined(COM2B1)
        case TIMER2B:
            cbi(TCCR2A, COM2B1);
            break;
#endif

#if defined(TCCR3A) && defined(COM3A1)
        case TIMER3A:
            cbi(TCCR3A, COM3A1);
            break;
#endif
#if defined(TCCR3A) && defined(COM3B1)
        case TIMER3B:
            cbi(TCCR3A, COM3B1);
            break;
#endif
#if defined(TCCR3A) && defined(COM3C1)
        case TIMER3C:
            cbi(TCCR3A, COM3C1);
            break;
#endif

#if defined(TCCR4A) && defined(COM4A1)
        case TIMER4A:
            cbi(TCCR4A, COM4A1);
            break;
#endif
#if defined(TCCR4A) && defined(COM4B1)
        case TIMER4B:
            cbi(TCCR4A, COM4B1);
            break;
#endif
#if defined(TCCR4A) && defined(COM4C1)
        case TIMER4C:
            cbi(TCCR4A, COM4C1);
            break;
#endif
#if defined(TCCR4C) && defined(COM4D1)
        case TIMER4D:
            cbi(TCCR4C, COM4D1);
            break;
#endif

#if defined(TCCR5A)
        case TIMER5A:
            cbi(TCCR5A, COM5A1);
            break;
        case TIMER5B:
            cbi(TCCR5A, COM5B1);
            break;
        case TIMER5C:
            cbi(TCCR5A, COM5C1);
            break;
#endif
    }

    this->pwmOn = false;

    return true;
}

bool Pin::isSetAsInput() const {
    return this->mode == pin_m::Input || this->mode == pin_m::InputPullup;
}
}  // namespace AVRIO
