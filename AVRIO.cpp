#include "AVRIO.h"

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

Pin::Pin()
{
}

Pin::Pin(byte pin, uint8_t mode)
{
    uint8_t port = digitalPinToPort(pin);

    this->portMode = portModeRegister(port);
    this->portOut = portOutputRegister(port);
    this->portIn = portInputRegister(port);

    this->pinMask = digitalPinToBitMask(pin);

    this->interruptNum = digitalPinToInterrupt(pin);
    this->isInterruptCapable = (interruptNum != -1);

    this->isPWMCapable = digitalPinHasPWM(pin);
    this->pwmOn = false;

    this->arduinoPin = pin;
    this->fState = 0;
    this->rState = 1;

    this->mode = 255;

    turnOffPWM();
    pinMode(mode);
}

uint8_t Pin::digitalRead(const uint8_t &mode) const
{
    // Leitura do pino digital
    byte reading = (*portIn & pinMask) ? 1 : 0;

    switch (mode)
    {
    case FALLING:
        fState = (3 & fState << 1) | (reading);
        return fState == 2;
    case RISING:
        rState = (3 & rState << 1) | (reading);
        return rState == 1;
    case CHANGE:
        rState = (3 & rState << 1) | (reading);
        fState = (3 & fState << 1) | (reading);
        return (fState == 2 || rState == 1);
    default:
        return reading;
    }
}

void Pin::digitalWrite(const uint8_t &state) const
{
    uint8_t oldSREG = SREG;
    cli();

    switch (state)
    {
    case LOW: ///< Sets the pin's state to low
        *portOut &= ~pinMask;
        break;
    case HIGH: ///< Sets the pin's state to high
        *portOut |= pinMask;
        break;
    case TOGGLE: ///< Toggles the pin's state
        *portOut ^= pinMask;
        break;
    }

    SREG = oldSREG;
}

void Pin::pinMode(const uint8_t &mode) const
{
    if (mode == this->mode)
    {
        return;
    }

    this->mode = mode;

    byte oldSREG = SREG;

    cli();

    turnOffPWM();

    switch (mode)
    {
    case INPUT: ///< Sets the pin as an input pin
        *portMode &= ~pinMask;
        *portOut &= ~pinMask;
        break;
    case INPUT_PULLUP: ///< Sets the pin as an input pullup pin
        *portMode &= ~pinMask;
        *portOut |= pinMask;
        break;
    case OUTPUT: ///< Sets the pin as an output pin
        *portMode |= pinMask;
        *portOut &= ~pinMask;
        break;
    case PWM:
        if (!turnOnPWM())
            pinMode(INPUT);
        break;
    }

    SREG = oldSREG;
}

uint16_t Pin::analogRead() const
{
    if (this->pwmOn)
        return 0;

    return Arduino_h::analogRead(arduinoPin);
}

void Pin::analogWrite(uint16_t val) const
{
    if (this->pwmOn)
        Arduino_h::analogWrite(this->arduinoPin, val);
}

uint8_t Pin::getPin() const
{
    return this->arduinoPin;
}

bool Pin::attachInterrupt(uint8_t mode, void (*callback)()) const
{
    bool isSetAsInput = (this->mode == INPUT || this->mode == INPUT_PULLUP);

    if (this->isInterruptCapable && isSetAsInput)
    {
        Arduino_h::attachInterrupt(this->interruptNum, callback, mode);
    }

    return isInterruptCapable;
}

bool Pin::detachInterrupt() const
{
    if (this->isInterruptCapable)
    {
        Arduino_h::detachInterrupt(interruptNum);
    }
    return isInterruptCapable;
}

bool Pin::turnOnPWM() const
{
    if (!this->isPWMCapable || this->mode == INPUT || this->mode == INPUT_PULLUP)
    {
        return false;
    }

    Arduino_h::analogWrite(this->arduinoPin, 127);

    this->pwmOn = true;

    return true;
}

bool Pin::turnOffPWM() const
{
    if (!this->isPWMCapable || !this->pwmOn)
    {
        return false;
    }

    uint8_t timer = digitalPinToTimer(this->arduinoPin);

    switch (timer)
    {
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
