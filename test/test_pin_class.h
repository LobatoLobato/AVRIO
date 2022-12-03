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
#pragma once
#include <AVRIO.h>
#include <Arduino.h>
#include <unity.h>

#define RUN_PIN_TESTS()               \
    RUN_TEST(test_pin_pin_mode);      \
    RUN_TEST(test_pin_digital_write); \
    RUN_TEST(test_pin_digital_read);  \
    RUN_TEST(test_pin_interrupt);     \
    RUN_TEST(test_pin_getPin);        \
    RUN_TEST(test_analog)

void test_pin_pin_mode(void);

void test_pin_digital_write(void);

void test_pin_digital_read(void);

void test_pin_interrupt(void);

void test_pin_getPin(void);

void test_analog(void);