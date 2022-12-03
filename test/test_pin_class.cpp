#include "test_pin_class.h"

const AVRIO::Pin tPinSpecial(3);  // Nano's D3 |PWM/Interrupt capable
const AVRIO::Pin tPinNormal(4);   // Nano's D4 |Normal pin
const AVRIO::Pin tPinAnalog(A7);  // Nano's A7 |Analog pin

void test_pin_pin_mode(void) {
    // Will set pin as input
    tPinSpecial.pinMode(AVRIO::pin_m::Input);
    TEST_ASSERT_BIT_LOW(PIN3, DDRD);
    TEST_ASSERT_BIT_LOW(PIN3, PORTD);

    // Will set pin as input pull-up
    tPinSpecial.pinMode(AVRIO::pin_m::InputPullup);
    TEST_ASSERT_BIT_LOW(PIN3, DDRD);
    TEST_ASSERT_BIT_HIGH(PIN3, PORTD);

    // Will set pin as output
    tPinSpecial.pinMode(AVRIO::pin_m::Output);
    TEST_ASSERT_BIT_HIGH(PIN3, DDRD);

    // Will set pin as pwm output
    tPinSpecial.pinMode(AVRIO::pin_m::Pwm);
    TEST_ASSERT_BIT_HIGH(PIN3, DDRD);

    // Will fallback to input as the pin is not pwm capable
    tPinNormal.pinMode(AVRIO::pin_m::Pwm);
    TEST_ASSERT_BIT_LOW(PIN4, DDRD);
}

void test_pin_digital_write(void) {
    tPinSpecial.pinMode(AVRIO::pin_m::Output);

    tPinSpecial.digitalWrite(AVRIO::write_t::Low);
    TEST_ASSERT_BIT_LOW(PIN3, PORTD);

    tPinSpecial.digitalWrite(AVRIO::write_t::High);
    TEST_ASSERT_BIT_HIGH(PIN3, PORTD);

    tPinSpecial.digitalWrite(AVRIO::write_t::Toggle);
    TEST_ASSERT_BIT_LOW(PIN3, PORTD);

    tPinSpecial.digitalWrite(AVRIO::write_t::Toggle);
    TEST_ASSERT_BIT_HIGH(PIN3, PORTD);

    tPinSpecial.digitalWrite(AVRIO::write_t::Low);
    TEST_ASSERT_BIT_LOW(PIN3, PORTD);
}

void test_pin_digital_read(void) {
    byte reading = 0;
    tPinSpecial.pinMode(AVRIO::pin_m::Output);
    tPinNormal.pinMode(AVRIO::pin_m::Input);

    /* NO EDGE DETECTION */
    // Resets pin's internal state
    tPinNormal.digitalRead(AVRIO::edge_t::None);
    tPinSpecial.digitalWrite(AVRIO::write_t::Low);

    // Starts tests
    reading = tPinNormal.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(LOW, reading);

    tPinSpecial.digitalWrite(AVRIO::write_t::High);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = tPinNormal.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = tPinNormal.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    tPinSpecial.digitalWrite(AVRIO::write_t::Low);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, LOW);

    tPinSpecial.digitalWrite(AVRIO::write_t::High);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    // FALLING EDGE DETECTION
    // Resets pin's internal state
    tPinSpecial.digitalWrite(AVRIO::write_t::Low);
    tPinNormal.digitalRead(AVRIO::edge_t::Falling);

    // Starts tests
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    tPinSpecial.digitalWrite(AVRIO::write_t::High);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = tPinNormal.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = tPinNormal.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    tPinSpecial.digitalWrite(AVRIO::write_t::Low);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, HIGH);

    tPinSpecial.digitalWrite(AVRIO::write_t::High);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    /* RISING EDGE DETECTION */
    // Resets pin's internal state
    tPinSpecial.digitalWrite(AVRIO::write_t::Low);
    tPinNormal.digitalRead(AVRIO::edge_t::Rising);

    // Starts tests
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    tPinSpecial.digitalWrite(AVRIO::write_t::High);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = tPinNormal.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = tPinNormal.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    tPinSpecial.digitalWrite(AVRIO::write_t::Low);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    tPinSpecial.digitalWrite(AVRIO::write_t::High);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, HIGH);

    /* ANY EDGE DETECTION */
    // Resets pin's internal state
    tPinSpecial.digitalWrite(AVRIO::write_t::Low);
    tPinNormal.digitalRead(AVRIO::edge_t::Change);

    // Starts tests
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, LOW);

    tPinSpecial.digitalWrite(AVRIO::write_t::High);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = tPinNormal.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = tPinNormal.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, LOW);

    tPinSpecial.digitalWrite(AVRIO::write_t::Low);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, HIGH);

    tPinSpecial.digitalWrite(AVRIO::write_t::High);
    reading = tPinNormal.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, HIGH);
}

volatile bool ISRcallbackResult;
void ISRcallback() {
    ISRcallbackResult = true;
}
void test_pin_interrupt(void) {
    bool result;

    // THIS PIN WONT WORK
    tPinNormal.pinMode(AVRIO::pin_m::Input);
    result = tPinNormal.attachInterrupt(AVRIO::edge_t::Low, ISRcallback);
    TEST_ASSERT_FALSE(result);
    result = tPinNormal.detachInterrupt();
    TEST_ASSERT_FALSE(result);

    tPinNormal.pinMode(AVRIO::pin_m::Output);
    tPinSpecial.pinMode(AVRIO::pin_m::Input);
    // TESTS TESTS TESTS LOW
    tPinNormal.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;

    result = tPinSpecial.attachInterrupt(AVRIO::edge_t::Low, ISRcallback);
    TEST_ASSERT_TRUE(result);

    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);

    tPinNormal.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = tPinSpecial.detachInterrupt();
    TEST_ASSERT_TRUE(result);

    // TESTS TESTS TESTS FALLING
    tPinNormal.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;

    result = tPinSpecial.attachInterrupt(AVRIO::edge_t::Falling, ISRcallback);
    TEST_ASSERT_TRUE(result);

    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);

    ISRcallbackResult = false;
    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    tPinNormal.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = tPinSpecial.detachInterrupt();
    TEST_ASSERT_TRUE(result);

    // TESTS TESTS TESTS RISING
    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    ISRcallbackResult = false;

    result = tPinSpecial.attachInterrupt(AVRIO::edge_t::Rising, ISRcallback);
    TEST_ASSERT_TRUE(result);

    tPinNormal.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    tPinNormal.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    tPinNormal.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    tPinNormal.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = tPinSpecial.detachInterrupt();
    TEST_ASSERT_TRUE(result);

    // TESTS TESTS TESTS CHANGE
    tPinNormal.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;

    result = tPinSpecial.attachInterrupt(AVRIO::edge_t::Change, ISRcallback);
    TEST_ASSERT_TRUE(result);

    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    tPinNormal.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    tPinNormal.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    tPinNormal.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = tPinSpecial.detachInterrupt();
    TEST_ASSERT_TRUE(result);
}

void test_pin_getPin(void) {
    TEST_ASSERT_EQUAL(tPinSpecial.getPin(), 3);
}

void test_analog(void) {
    uint16_t result;
    tPinSpecial.pinMode(AVRIO::pin_m::Pwm);
    tPinNormal.pinMode(AVRIO::pin_m::Input);
    tPinAnalog.pinMode(AVRIO::pin_m::Input);

    tPinSpecial.analogWrite(0);
    delay(20);
    result = tPinAnalog.analogRead();
    TEST_ASSERT_TRUE_MESSAGE(result >= 0 && result <= 5, String(result).c_str());

    tPinSpecial.analogWrite(127);
    delay(20);
    result = tPinAnalog.analogRead();
    TEST_ASSERT_TRUE_MESSAGE(result >= 400 && result <= 600, String(result).c_str());

    tPinSpecial.analogWrite(255);
    delay(20);
    result = tPinAnalog.analogRead();
    TEST_ASSERT_TRUE_MESSAGE(result >= 1000 && result <= 1023, String(result).c_str());
}