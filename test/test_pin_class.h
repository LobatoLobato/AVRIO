/* Provides unit testing to
Target test board = Arduino Nano
AVRIO{                           |TEST_IMPLEMENTED|
  static Pin::initializePins()   |        ✓       |
  Pin::digitalWrite()            |        ✓       |
  Pin::digitalRead()             |        ✓       |
  Pin::attachInterrupt()         |        ✓       |
  Pin::detachInterrupt()         |        ✓       |
  Pin::analogRead()              |        ✓       |
  Pin::analogWrite()             |        ✓       |
  Pin::pinMode()                 |        ✓       |
  Pin::getPin()                  |        ✓       |
  Pin::Pin()                     |        ✓       | // If all methods above work the constructor works
}
*/
#pragma once
#include <AVRIO.h>
#include <Arduino.h>
#include <unity.h>

#define RUN_PIN_TESTS()                        \
    RUN_TEST(test_pin_static_initialize_pins); \
    RUN_TEST(test_pin_pin_mode);               \
    RUN_TEST(test_pin_interrupt);              \
    RUN_TEST(test_pin_getPin);                 \
    RUN_TEST(test_pin_analog);                 \
    RUN_TEST(test_pin_digital_write);          \
    RUN_TEST(test_pin_digital_read);           \
    pin_test_tearDown();

// Static methods
void test_pin_static_initialize_pins(void);

// Instance methods
void test_pin_pin_mode(void);
void test_pin_digital_write(void);
void test_pin_digital_read(void);
void test_pin_interrupt(void);
void test_pin_getPin(void);
void test_pin_analog(void);

void pin_test_tearDown(void);