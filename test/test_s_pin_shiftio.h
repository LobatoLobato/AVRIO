/* Provides unit testing to
Target test board = Arduino Nano
AVRIO{                       |TEST_AVAILABLE|
  static Pin::shiftOut()     |       ✓      |
  static Pin::shiftIn()      |       ✓      |
}
*/
#pragma once
#include <AVRIO.h>
#include <Arduino.h>
#include <unity.h>

#define RUN_SHIFTIO_TESTS()              \
    shiftio_test_setUp();                \
    RUN_TEST(test_static_pin_shift_in);  \
    RUN_TEST(test_static_pin_shift_out); \
    shiftio_test_tearDown();

void test_static_pin_shift_out(void);
void test_static_pin_shift_in(void);
void shiftio_test_setUp(void);
void shiftio_test_tearDown(void);