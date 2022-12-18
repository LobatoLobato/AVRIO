#include "test_pin_class.h"
const AVRIO::Pin DPIN3(3);   // Nano's D3 | PWM/Interrupt capable
const AVRIO::Pin DPIN4(4);   // Nano's D4 | Normal pin
const AVRIO::Pin APIN7(A7);  // Nano's A7 | Analog pin

const AVRIO::Pin DPIN5(5, AVRIO::pin_m::Output);  // Nano's D5
const AVRIO::Pin DPIN2(2, AVRIO::pin_m::Input);   // Nano's D2

void test_pin_static_initialize_pins(void) {
    AVRIO::Pin::initializePins(DPIN5, DPIN2);
    uint8_t reading;

    DPIN5.digitalWrite(AVRIO::write_t::High);
    reading = DPIN2.digitalRead();
    TEST_ASSERT_EQUAL(HIGH, reading);

    DPIN5.digitalWrite(AVRIO::write_t::Low);
    reading = DPIN2.digitalRead();
    TEST_ASSERT_EQUAL(LOW, reading);

    DPIN5.digitalWrite(AVRIO::write_t::High);
    reading = DPIN2.digitalRead();
    TEST_ASSERT_EQUAL(HIGH, reading);
}
void test_pin_pin_mode(void) {
    // Will set pin as input
    DPIN3.pinMode(AVRIO::pin_m::Input);
    TEST_ASSERT_BIT_LOW(PIN3, DDRD);
    TEST_ASSERT_BIT_LOW(PIN3, PORTD);

    // Will set pin as input pull-up
    DPIN3.pinMode(AVRIO::pin_m::InputPullup);
    TEST_ASSERT_BIT_LOW(PIN3, DDRD);
    TEST_ASSERT_BIT_HIGH(PIN3, PORTD);

    // Will set pin as output
    DPIN3.pinMode(AVRIO::pin_m::Output);
    TEST_ASSERT_BIT_HIGH(PIN3, DDRD);

    // Will set pin as pwm output
    DPIN3.pinMode(AVRIO::pin_m::Pwm);
    TEST_ASSERT_BIT_HIGH(PIN3, DDRD);

    // Will fallback to input as the pin is not pwm capable
    DPIN4.pinMode(AVRIO::pin_m::Pwm);
    TEST_ASSERT_BIT_LOW(PIN4, DDRD);
}

void test_pin_digital_write(void) {
    DPIN3.pinMode(AVRIO::pin_m::Output);

    DPIN3.digitalWrite(AVRIO::write_t::Low);
    TEST_ASSERT_BIT_LOW(PIN3, PORTD);

    DPIN3.digitalWrite(AVRIO::write_t::High);
    TEST_ASSERT_BIT_HIGH(PIN3, PORTD);

    DPIN3.digitalWrite(AVRIO::write_t::Toggle);
    TEST_ASSERT_BIT_LOW(PIN3, PORTD);

    DPIN3.digitalWrite(AVRIO::write_t::Toggle);
    TEST_ASSERT_BIT_HIGH(PIN3, PORTD);

    DPIN3.digitalWrite(AVRIO::write_t::Low);
    TEST_ASSERT_BIT_LOW(PIN3, PORTD);
}

void test_pin_digital_read(void) {
    byte reading = 0;
    DPIN3.pinMode(AVRIO::pin_m::Output);
    DPIN4.pinMode(AVRIO::pin_m::Input);

    /* NO EDGE DETECTION */
    // Resets pin's internal state
    DPIN4.digitalRead(AVRIO::edge_t::None);
    DPIN3.digitalWrite(AVRIO::write_t::Low);

    // Starts tests
    reading = DPIN4.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(LOW, reading);

    DPIN3.digitalWrite(AVRIO::write_t::High);
    reading = DPIN4.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = DPIN4.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = DPIN4.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    DPIN3.digitalWrite(AVRIO::write_t::Low);
    reading = DPIN4.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, LOW);

    DPIN3.digitalWrite(AVRIO::write_t::High);
    reading = DPIN4.digitalRead(AVRIO::edge_t::None);
    TEST_ASSERT_EQUAL(reading, HIGH);

    // FALLING EDGE DETECTION
    // Resets pin's internal state
    DPIN3.digitalWrite(AVRIO::write_t::Low);
    DPIN4.digitalRead(AVRIO::edge_t::Falling);

    // Starts tests
    reading = DPIN4.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    DPIN3.digitalWrite(AVRIO::write_t::High);
    reading = DPIN4.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = DPIN4.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = DPIN4.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    DPIN3.digitalWrite(AVRIO::write_t::Low);
    reading = DPIN4.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, HIGH);

    DPIN3.digitalWrite(AVRIO::write_t::High);
    reading = DPIN4.digitalRead(AVRIO::edge_t::Falling);
    TEST_ASSERT_EQUAL(reading, LOW);

    /* RISING EDGE DETECTION */
    // Resets pin's internal state
    DPIN3.digitalWrite(AVRIO::write_t::Low);
    DPIN4.digitalRead(AVRIO::edge_t::Rising);

    // Starts tests
    reading = DPIN4.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    DPIN3.digitalWrite(AVRIO::write_t::High);
    reading = DPIN4.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = DPIN4.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = DPIN4.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    DPIN3.digitalWrite(AVRIO::write_t::Low);
    reading = DPIN4.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, LOW);

    DPIN3.digitalWrite(AVRIO::write_t::High);
    reading = DPIN4.digitalRead(AVRIO::edge_t::Rising);
    TEST_ASSERT_EQUAL(reading, HIGH);

    /* ANY EDGE DETECTION */
    // Resets pin's internal state
    DPIN3.digitalWrite(AVRIO::write_t::Low);
    DPIN4.digitalRead(AVRIO::edge_t::Change);

    // Starts tests
    reading = DPIN4.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, LOW);

    DPIN3.digitalWrite(AVRIO::write_t::High);
    reading = DPIN4.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, HIGH);

    reading = DPIN4.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, LOW);

    reading = DPIN4.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, LOW);

    DPIN3.digitalWrite(AVRIO::write_t::Low);
    reading = DPIN4.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, HIGH);

    DPIN3.digitalWrite(AVRIO::write_t::High);
    reading = DPIN4.digitalRead(AVRIO::edge_t::Change);
    TEST_ASSERT_EQUAL(reading, HIGH);
}

volatile bool ISRcallbackResult;
void ISRcallback() {
    ISRcallbackResult = true;
}
void test_pin_interrupt(void) {
    bool result;

    // THIS PIN WONT WORK
    DPIN4.pinMode(AVRIO::pin_m::Input);
    result = DPIN4.attachInterrupt(AVRIO::edge_t::Low, ISRcallback);
    TEST_ASSERT_FALSE(result);
    result = DPIN4.detachInterrupt();
    TEST_ASSERT_FALSE(result);

    DPIN4.pinMode(AVRIO::pin_m::Output);
    DPIN3.pinMode(AVRIO::pin_m::Input);
    // TESTS TESTS TESTS LOW
    DPIN4.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;

    result = DPIN3.attachInterrupt(AVRIO::edge_t::Low, ISRcallback);
    TEST_ASSERT_TRUE(result);

    DPIN4.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);

    DPIN4.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = DPIN3.detachInterrupt();
    TEST_ASSERT_TRUE(result);

    // TESTS TESTS TESTS FALLING
    DPIN4.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;

    result = DPIN3.attachInterrupt(AVRIO::edge_t::Falling, ISRcallback);
    TEST_ASSERT_TRUE(result);

    DPIN4.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);

    ISRcallbackResult = false;
    DPIN4.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    DPIN4.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    DPIN4.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    DPIN4.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = DPIN3.detachInterrupt();
    TEST_ASSERT_TRUE(result);

    // TESTS TESTS TESTS RISING
    DPIN4.digitalWrite(AVRIO::write_t::Low);
    ISRcallbackResult = false;

    result = DPIN3.attachInterrupt(AVRIO::edge_t::Rising, ISRcallback);
    TEST_ASSERT_TRUE(result);

    DPIN4.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    DPIN4.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    DPIN4.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    DPIN4.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    DPIN4.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = DPIN3.detachInterrupt();
    TEST_ASSERT_TRUE(result);

    // TESTS TESTS TESTS CHANGE
    DPIN4.digitalWrite(AVRIO::write_t::High);
    ISRcallbackResult = false;

    result = DPIN3.attachInterrupt(AVRIO::edge_t::Change, ISRcallback);
    TEST_ASSERT_TRUE(result);

    DPIN4.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    DPIN4.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    DPIN4.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    DPIN4.digitalWrite(AVRIO::write_t::High);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    DPIN4.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_TRUE(ISRcallbackResult);
    ISRcallbackResult = false;

    DPIN4.digitalWrite(AVRIO::write_t::Low);
    delay(10);
    TEST_ASSERT_FALSE(ISRcallbackResult);

    result = DPIN3.detachInterrupt();
    TEST_ASSERT_TRUE(result);
}

void test_pin_getPin(void) {
    TEST_ASSERT_EQUAL(DPIN3.getPin(), 3);
}

void test_pin_analog(void) {
    struct testResult {
        bool result;
        String msg;
    } r;
    auto testMin = [](uint16_t (*test)(), uint16_t min = 0) -> testResult {
        DPIN3.analogWrite(min);
        delay(20);
        uint16_t result = test();
        String msg = String("Expected 0 <= x <= 200 was ") + String(result);
        return {result >= 0 && result <= 200, msg};
    };
    auto testHalf = [](uint16_t (*test)(), uint16_t half = 127) -> testResult {
        DPIN3.analogWrite(half);
        delay(20);
        uint16_t result = test();
        String msg = String("Expected 300 <= x <= 700 was ") + String(result);
        return {result >= 300 && result <= 700, msg};
    };
    auto testMax = [](uint16_t (*test)(), uint16_t max = 255) -> testResult {
        DPIN3.analogWrite(max);
        delay(20);
        uint16_t result = test();
        String msg = String("Expected 800 <= x <= 1023 was ") + String(result);
        return {result >= 800 && result <= 1023, msg};
    };

    DPIN3.pinMode(AVRIO::pin_m::Pwm);
    DPIN4.pinMode(AVRIO::pin_m::Input);
    APIN7.pinMode(AVRIO::pin_m::Input);

    /* Analog Write and Synchronous Analog Read */
    r = testMin([]() -> uint16_t { return APIN7.analogRead(); });
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    r = testHalf([]() -> uint16_t { return APIN7.analogRead(); });
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    r = testMax([]() -> uint16_t { return APIN7.analogRead(); });
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    /* Analog Write and Asynchronous Analog Read */
    // Waiting for conversion
    auto asyncAR = []() -> uint16_t {
        AVRIO::Pin::asyncADCReturnType ar = APIN7.asyncAnalogRead();
        while (!ar.ready()) {
        }
        return ar.read();
    };
    r = testMin(asyncAR);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    r = testHalf(asyncAR);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    r = testMax(asyncAR);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    // Simulating real use case
    auto asyncARAlt = []() -> uint16_t {
        while (true) {
            AVRIO::Pin::asyncADCReturnType ar = APIN7.asyncAnalogRead();
            if (ar.ready()) {
                return ar.read();
            }
            // Doing things here
        }
    };
    r = testMin(asyncARAlt);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    r = testHalf(asyncARAlt);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    r = testMax(asyncARAlt);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    testMax([]() -> uint16_t {
        AVRIO::Pin::asyncADCReturnType ar = APIN7.asyncAnalogRead();
        uint16_t result = ar.read();
        while (!ar.ready())
            ;
        return result;
    });

    /* Analog Write and Asynchronous Analog Read with Callback*/
    auto asyncARCB = []() -> uint16_t {
        uint16_t result;
        auto callbackFn = [&result](uint16_t reading) -> void { result = reading; };
        while (!APIN7.asyncAnalogRead(callbackFn)) {
        }
        return result;
    };
    r = testMin(asyncARCB);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    r = testHalf(asyncARCB);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    r = testMax(asyncARCB);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    /* Analog Write and Synchronous Analog Read with Internal Analog Reference */
    AVRIO::Pin::setAnalogReference(AVRIO::aref_t::Internal);
    uint16_t max = 1.1 / ((AVRIO::readVcc() / 1000) / 256.0);
    uint16_t half = max / 2;
    uint16_t min = 0;

    r = testMin([]() -> uint16_t { return APIN7.analogRead(); }, min);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    r = testHalf([]() -> uint16_t { return APIN7.analogRead(); }, half);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());

    r = testMax([]() -> uint16_t { return APIN7.analogRead(); }, max);
    TEST_ASSERT_TRUE_MESSAGE(r.result, r.msg.c_str());
}

void pin_test_tearDown(void) {
    DPIN2.pinMode(AVRIO::pin_m::Input);
    DPIN3.pinMode(AVRIO::pin_m::Input);
    DPIN4.pinMode(AVRIO::pin_m::Input);
    DPIN5.pinMode(AVRIO::pin_m::Input);
    APIN7.pinMode(AVRIO::pin_m::Input);
}