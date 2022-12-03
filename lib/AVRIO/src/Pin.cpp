#include "AVRIO.h"

void cbi(uint8_t sfr, uint8_t bit) {
    _SFR_BYTE(sfr) &= ~_BV(bit);
}

void sbi(uint8_t sfr, uint8_t bit) {
    _SFR_BYTE(sfr) |= _BV(bit);
}

AVRIO::Pin::Pin() {}

AVRIO::Pin::Pin(byte pin, pin_m mode) {
    uint8_t port = digitalPinToPort(pin);  // Converts the pin number to the pin port

    this->portMode = portModeRegister(port);   // Gets the port mode register
    this->portOut = portOutputRegister(port);  // Gets the port output register
    this->portIn = portInputRegister(port);    // Gets the port input register

    this->pinMask = digitalPinToBitMask(pin);  // Gets the pin bit mask

    this->interruptNum = digitalPinToInterrupt(pin);                // Gets the pin's interrupt number or -1 if it's not interrupt capable
    this->isInterruptCapable = (interruptNum != NOT_AN_INTERRUPT);  // Sets the interrupt capable flag

    this->isPWMCapable = digitalPinHasPWM(pin);  // Sets the pwm capable flag
    this->pwmOn = false;                         // lalalalalalalalalalalalalalalalalalalala

    this->arduinoPin = pin;  // Stores the arduino pin passed in the constructor
    this->fState = 0;        // Sets the initial falling edge state
    this->rState = 1;        // Sets the initial rising edge state

    this->mode = (pin_m)255;  // Stores the pin mode

    turnOffPWM();   // Turns off pwm
    pinMode(mode);  // Sets the pin mode
}

void AVRIO::Pin::pinMode(const pin_m& mode) const {
    if (mode == this->mode)  // If mode is equal to current mode cancel the function
        return;

    this->mode = mode;  // Stores new mode

    byte oldSREG = SREG;  // NAO SEI

    noInterrupts();  // Disables interrupts

    turnOffPWM();                              // Turns off PWM on the pin
    Arduino_h::digitalRead(this->arduinoPin);  // TEMPORARY: Turns off pwm, method above doesn't work
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
            if (!turnOnPWM())
                pinMode(pin_m::Input);
            break;
    }

    SREG = oldSREG;
    interrupts();  // Enables interrupts
}

uint8_t AVRIO::Pin::digitalRead(const edge_t& mode) const {
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
}

void AVRIO::Pin::digitalWrite(const write_t& state) const {
    uint8_t oldSREG = SREG;  // NAO SEI
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

    SREG = oldSREG;  // NAO SEI
    interrupts();    // Enables interrupts
}

uint16_t AVRIO::Pin::analogRead() const {
    if (this->pwmOn)  // If pwm is on uashuashuahsuhuas
        return 0;

    return Arduino_h::analogRead(arduinoPin);  // Returns the analog reading on the pin
}

void AVRIO::Pin::analogWrite(uint16_t val) const {
    if (this->pwmOn)
        Arduino_h::analogWrite(this->arduinoPin, val);
}

uint8_t AVRIO::Pin::getPin() const {
    return this->arduinoPin;
}

bool AVRIO::Pin::attachInterrupt(edge_t mode, void (*callback)()) const {
    if (this->isInterruptCapable && this->isSetAsInput()) {
        EIFR = 0x01;
        Arduino_h::attachInterrupt(this->interruptNum, callback, (int)mode);
    }

    return isInterruptCapable;
}

bool AVRIO::Pin::detachInterrupt() const {
    if (this->isInterruptCapable) {
        Arduino_h::detachInterrupt(interruptNum);
    }
    return isInterruptCapable;
}

bool AVRIO::Pin::turnOnPWM() const {
    if (!this->isPWMCapable || this->isSetAsInput()) {
        return false;
    }

    Arduino_h::analogWrite(this->arduinoPin, 127);

    this->pwmOn = true;

    return true;
}

bool AVRIO::Pin::turnOffPWM() const {
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

bool AVRIO::Pin::isSetAsInput() const {
    return this->mode == pin_m::Input || this->mode == pin_m::InputPullup;
}
