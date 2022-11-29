/* Provides unit testing to
Target test board = Arduino Nano
AVRIO{                    |TEST_AVAILABLE|
  Pin::digitalWrite()     |       ✓      |
  Pin::digitalRead()      |       ✓      |
  Pin::attachInterrupt()  |       ✓      |
  Pin::detachInterrupt()  |       ✓      |
  Pin::analogRead()       |       ✓      |
  Pin::analogWrite()      |       ✓      |
  Pin::pinMode()          |       ✓      |
  Pin::getPin()           |       ✓      |
  Pin::Pin()              |       ✓      | // If all methods above work the constructor works
}
*/
#include <AVRIO.h>
#include <Arduino.h>
#include <unity.h>

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_pin_pin_mode(void) {
    const AVRIO::Pin pinA(8);  // This pin is not pwm capable
    const AVRIO::Pin pinB(9);  // This pin is pwm capable

    pinA.pinMode(AVRIO::pin_m::Input);
    TEST_ASSERT_BIT_LOW(PIN0, DDRB);
    TEST_ASSERT_BIT_LOW(PIN0, PORTB);

    pinA.pinMode(AVRIO::pin_m::InputPullup);
    TEST_ASSERT_BIT_LOW(PIN0, DDRB);
    TEST_ASSERT_BIT_HIGH(PIN0, PORTB);

    pinA.pinMode(AVRIO::pin_m::Output);
    TEST_ASSERT_BIT_HIGH(PIN0, DDRB);

    pinA.pinMode(AVRIO::pin_m::Pwm);
    TEST_ASSERT_BIT_LOW(PIN0, DDRB);
    pinB.pinMode(AVRIO::pin_m::Pwm);
    TEST_ASSERT_BIT_HIGH(PIN1, DDRB);
}

void test_pin_digital_write(void) {
    const AVRIO::Pin pin(2, AVRIO::pin_m::Output);

    pin.digitalWrite(AVRIO::write_t::Low);
    TEST_ASSERT_BIT_LOW_MESSAGE(PIN2, PORTD, "Failed to set pin's output to low");
    delay(100);

    pin.digitalWrite(AVRIO::write_t::High);
    TEST_ASSERT_BIT_HIGH_MESSAGE(PIN2, PORTD, "Failed to set pin's output to high");
    delay(100);

    pin.digitalWrite(AVRIO::write_t::Toggle);
    TEST_ASSERT_BIT_LOW_MESSAGE(PIN2, PORTD, "Failed to toggle pin's output");
    delay(100);

    pin.digitalWrite(AVRIO::write_t::Toggle);
    TEST_ASSERT_BIT_HIGH_MESSAGE(PIN2, PORTD, "Failed to toggle pin's output");
    delay(100);

    pin.digitalWrite(AVRIO::write_t::Low);
    TEST_ASSERT_BIT_LOW_MESSAGE(PIN2, PORTD, "Failed to toggle pin's output");
    delay(100);
}

void test_pin_digital_read(void) {
    const AVRIO::Pin pinOUT(5, AVRIO::pin_m::Output);
    const AVRIO::Pin pinIN(6, AVRIO::pin_m::Input);
    byte reading = 0;

    /* NO EDGE DETECTION */
    // Resets pin's internal state
    pinIN.digitalRead(AVRIO::edge_t::None);
    pinOUT.digitalWrite(AVRIO::write_t::Low);

    // Starts tests
    pinIN.digitalRead(AVRIO::edge_t::None);
    reading = pinIN.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, LOW);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    reading = pinIN.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = pinIN.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = pinIN.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    reading = pinIN.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, LOW);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    reading = pinIN.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    // FALLING EDGE DETECTION
    // Resets pin's internal state
    pinOUT.digitalWrite(AVRIO::write_t::Low);
    pinIN.digitalRead(AVRIO::edge_t::Falling);

    // Starts tests
    reading = pinIN.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    reading = pinIN.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = pinIN.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = pinIN.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    reading = pinIN.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, HIGH);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    reading = pinIN.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    /* RISING EDGE DETECTION */
    // Resets pin's internal state
    pinOUT.digitalWrite(AVRIO::write_t::Low);
    pinIN.digitalRead(AVRIO::edge_t::Rising);

    // Starts tests
    reading = pinIN.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    reading = pinIN.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = pinIN.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = pinIN.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    reading = pinIN.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    reading = pinIN.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, HIGH);

    /* ANY EDGE DETECTION */
    // Resets pin's internal state
    pinOUT.digitalWrite(AVRIO::write_t::Low);
    pinIN.digitalRead(AVRIO::edge_t::Change);

    // Starts tests
    reading = pinIN.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, LOW);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    reading = pinIN.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = pinIN.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = pinIN.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, LOW);

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    reading = pinIN.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, HIGH);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    reading = pinIN.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, HIGH);
}

volatile bool ISRcallbackResult;
void ISRcallback() {
    ISRcallbackResult = true;
}
void test_pin_interrupt(void) {
    const AVRIO::Pin pinOUT(2, AVRIO::pin_m::Output);
    const AVRIO::Pin pinA(3, AVRIO::pin_m::Input);  // This pin is interrupt capable
    const AVRIO::Pin pinB(4, AVRIO::pin_m::Input);  // This pin is not interrupt capable
    bool result;

    // THIS PIN WONT WORK
    result = pinB.attachInterrupt(AVRIO::edge_t::Low, ISRcallback);
    TEST_ASSERT_FALSE(result);
    result = pinB.detachInterrupt();
    TEST_ASSERT_FALSE(result);

    // TESTS TESTS TESTS LOW
    pinOUT.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;

    result = pinA.attachInterrupt(AVRIO::edge_t::Low, ISRcallback);
    TEST_ASSERT_TRUE(result);

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    delay(50);
    TEST_ASSERT_TRUE(ISRcallbackResult);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;
    delay(50);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = pinA.detachInterrupt();
    TEST_ASSERT_TRUE(result);

    // TESTS TESTS TESTS FALLING
    pinOUT.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;

    result = pinA.attachInterrupt(AVRIO::edge_t::Falling, ISRcallback);
    TEST_ASSERT_TRUE(result);

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    delay(50);
    TEST_ASSERT_TRUE(ISRcallbackResult);

    ISRcallbackResult = false;
    pinOUT.digitalWrite(AVRIO::write_t::Low);
    delay(50);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    delay(50);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    delay(50);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    delay(50);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = pinA.detachInterrupt();
    TEST_ASSERT_TRUE(result);

    // TESTS TESTS TESTS RISING
    pinOUT.digitalWrite(AVRIO::write_t::Low);
    ISRcallbackResult = false;

    result = pinA.attachInterrupt(AVRIO::edge_t::Rising, ISRcallback);
    TEST_ASSERT_TRUE(result);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    delay(50);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    pinOUT.digitalWrite(AVRIO::write_t::High);
    delay(50);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    delay(50);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    delay(50);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    pinOUT.digitalWrite(AVRIO::write_t::High);
    delay(50);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = pinA.detachInterrupt();
    TEST_ASSERT_TRUE(result);

    // TESTS TESTS TESTS CHANGE
    pinOUT.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;

    result = pinA.attachInterrupt(AVRIO::edge_t::Change, ISRcallback);
    TEST_ASSERT_TRUE(result);

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    delay(50);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    delay(50);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    pinOUT.digitalWrite(AVRIO::write_t::High);
    delay(50);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    pinOUT.digitalWrite(AVRIO::write_t::High);
    delay(50);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    delay(50);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    pinOUT.digitalWrite(AVRIO::write_t::Low);
    delay(50);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = pinA.detachInterrupt();
    TEST_ASSERT_TRUE(result);
}

void test_pin_getPin(void) {
    AVRIO::Pin pin(7);
    TEST_ASSERT_EQUAL(pin.getPin(), 7);
}

void test_analog(void) {
    AVRIO::Pin aw_pin(10, AVRIO::pin_m::Pwm);
    AVRIO::Pin ar_pin(A0, AVRIO::pin_m::Input);
    uint16_t result;

    aw_pin.analogWrite(0);
    delay(100);
    result = ar_pin.analogRead();
    TEST_ASSERT_TRUE(result >= 0 && result <= 5);

    aw_pin.analogWrite(127);
    delay(100);
    result = ar_pin.analogRead();
    TEST_ASSERT_TRUE(result >= 400 && result <= 600);

    aw_pin.analogWrite(255);
    delay(100);
    result = ar_pin.analogRead();
    TEST_ASSERT_TRUE(result >= 1000 && result <= 1023);
}

void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    // Serial.begin(115200);
    UNITY_BEGIN();  // IMPORTANT LINE!

    RUN_TEST(test_pin_digital_write);
    RUN_TEST(test_pin_digital_read);
    RUN_TEST(test_pin_interrupt);
    RUN_TEST(test_pin_pin_mode);
    RUN_TEST(test_pin_getPin);
    RUN_TEST(test_analog);
    UNITY_END();  // stop unit testing
}

void loop() {
}