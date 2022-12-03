#include <AVRIO.h>
#include <Arduino.h>
#include <unity.h>
#include "test_pin_class.h"

const AVRIO::Pin SIG(13, AVRIO::pin_m::Output);

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

int test_status;
void setup() {
    UNITY_BEGIN();              // Begin unit testing
    RUN_PIN_TESTS();            // Run pin class tests
    test_status = UNITY_END();  // Stop unit testing
}

void loop() {
    SIG.digitalWrite(AVRIO::write_t::Toggle);
    if (test_status == 0) {
        delay(1500);
    } else {
        delay(100);
    }
}