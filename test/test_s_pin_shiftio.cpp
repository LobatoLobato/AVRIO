#include "test_s_pin_shiftio.h"
const AVRIO::Pin CLKOUT(5, AVRIO::pin_m::Output);   // Nano's D5 |Clk out pin
const AVRIO::Pin CLKIN(2, AVRIO::pin_m::Input);     // Nano's D2|Clk in pin
const AVRIO::Pin DATAOUT(3, AVRIO::pin_m::Output);  // Nano's D3|Data out pin
const AVRIO::Pin DATAIN(4, AVRIO::pin_m::Input);    // Nano's D4|Data in pin

volatile uint8_t shift_counter = 0;

// Reads data into shiftInput (LSBFirst)
volatile uint64_t shiftInput = 0x00;
void shiftoutcb() {
    uint64_t reading = DATAIN.digitalRead();
    shiftInput |= reading << shift_counter++;
}
void test_static_pin_shift_out(void) {
    CLKIN.attachInterrupt(AVRIO::edge_t::Rising, shiftoutcb);

    // Shift out with delay
    // Milliseconds
    uint64_t mil = millis();
    shift_counter = 0;
    shiftInput = 0;
    AVRIO::Pin::shiftOut(DATAOUT, CLKOUT, (uint8_t)0xAA, AVRIO::bit_order::LSBFirst, 10, 50, true);
    TEST_ASSERT_GREATER_OR_EQUAL(480, (millis() - mil));
    TEST_ASSERT_EQUAL_UINT8(0xAA, shiftInput);

    // Microseconds
    mil = millis();
    shift_counter = 0;
    shiftInput = 0;
    AVRIO::Pin::shiftOut(DATAOUT, CLKOUT, (uint8_t)0xAA, AVRIO::bit_order::LSBFirst, 10, 50);
    TEST_ASSERT_LESS_OR_EQUAL(480, (millis() - mil));
    TEST_ASSERT_EQUAL_UINT8(0xAA, shiftInput);

    /* LSBFirst */
    // 8 bit shift out
    shift_counter = 0;
    shiftInput = 0;
    AVRIO::Pin::shiftOut(DATAOUT, CLKOUT, (uint8_t)0xAA);
    TEST_ASSERT_EQUAL_UINT8(0xAA, shiftInput);

    // 16 bit shift out
    shift_counter = 0;
    shiftInput = 0;
    AVRIO::Pin::shiftOut(DATAOUT, CLKOUT, (uint16_t)0xAAAA);
    TEST_ASSERT_EQUAL_UINT16(0xAAAA, shiftInput);

    // 32 bit shift out
    shift_counter = 0;
    shiftInput = 0;
    AVRIO::Pin::shiftOut(DATAOUT, CLKOUT, (uint32_t)0xAAAAAAAA);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA, shiftInput);

    // 64 bit shift out
    shift_counter = 0;
    shiftInput = 0;
    AVRIO::Pin::shiftOut(DATAOUT, CLKOUT, (uint64_t)0xAAAAAAAAAAAAAAAA);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA, shiftInput >> 32);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA, shiftInput);

    /* MSBFirst */
    // 8 bit shift out
    shift_counter = 0;
    shiftInput = 0;
    AVRIO::Pin::shiftOut(DATAOUT, CLKOUT, (uint8_t)0xAA, AVRIO::bit_order::MSBFirst);
    TEST_ASSERT_EQUAL_UINT8(0xAA ^ 0xFF, shiftInput);

    // 16 bit shift out
    shift_counter = 0;
    shiftInput = 0;
    AVRIO::Pin::shiftOut(DATAOUT, CLKOUT, (uint16_t)0xAAAA, AVRIO::bit_order::MSBFirst);
    TEST_ASSERT_EQUAL_UINT16(0xAAAA ^ 0xFFFF, shiftInput);

    // 32 bit shift out
    shift_counter = 0;
    shiftInput = 0;
    AVRIO::Pin::shiftOut(DATAOUT, CLKOUT, (uint32_t)0xAAAAAAAA, AVRIO::bit_order::MSBFirst);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA ^ 0xFFFFFFFF, shiftInput);

    // 64 bit shift out
    shift_counter = 0;
    shiftInput = 0;
    AVRIO::Pin::shiftOut(DATAOUT, CLKOUT, (uint64_t)0xAAAAAAAAAAAAAAAA, AVRIO::bit_order::MSBFirst);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA ^ 0xFFFFFFFF, shiftInput >> 32);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA ^ 0xFFFFFFFF, shiftInput);

    CLKIN.detachInterrupt();
}

// Sends data from the shiftOutArray (MSBFirst)
volatile uint64_t shiftOutput = 0;
volatile uint8_t bitOrder = LSBFIRST;
volatile uint64_t mod = 128;
void shiftincb() {
    if (bitOrder == LSBFIRST) {
        DATAOUT.digitalWrite((AVRIO::write_t)(shiftOutput & 1));
        shiftOutput >>= (uint64_t)1;
    } else {
        DATAOUT.digitalWrite((AVRIO::write_t)((shiftOutput & mod) != 0));
        shiftOutput <<= (uint64_t)1;
    }
}
void test_static_pin_shift_in(void) {
    CLKIN.attachInterrupt(AVRIO::edge_t::Rising, shiftincb);
    uint64_t reading;

    // Shift in with delay
    // Milliseconds
    uint64_t mil = millis();
    mil = millis();
    shiftOutput = 0xAA;
    reading = AVRIO::Pin::shiftIn<uint8_t>(DATAIN, CLKOUT, AVRIO::bit_order::LSBFirst, 10, true);
    TEST_ASSERT_GREATER_OR_EQUAL(80, (millis() - mil));
    TEST_ASSERT_EQUAL_UINT8(0xAA, reading);

    // Microseconds
    mil = millis();
    shiftOutput = 0xAA;
    reading = AVRIO::Pin::shiftIn<uint8_t>(DATAIN, CLKOUT, AVRIO::bit_order::LSBFirst, 10);
    TEST_ASSERT_LESS_OR_EQUAL(80, (millis() - mil));
    TEST_ASSERT_EQUAL_UINT8(0xAA, reading);

    /* =========================LSBFirst========================= */
    bitOrder = LSBFIRST;
    // 8 bit shift in
    shiftOutput = 0xAA;
    reading = AVRIO::Pin::shiftIn<uint8_t>(DATAIN, CLKOUT);
    TEST_ASSERT_EQUAL_UINT8(0xAA, reading);

    // 16 bit shift in
    shiftOutput = 0xAAAA;
    reading = AVRIO::Pin::shiftIn<uint16_t>(DATAIN, CLKOUT);
    TEST_ASSERT_EQUAL_UINT16(0xAAAA, reading);

    // 32 bit shift in
    shiftOutput = 0xAAAAAAAA;
    reading = AVRIO::Pin::shiftIn<uint32_t>(DATAIN, CLKOUT);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA, reading);

    // 64 bit shift in
    shiftOutput = 0xAAAAAAAAAAAAAAAA;
    reading = AVRIO::Pin::shiftIn<uint64_t>(DATAIN, CLKOUT);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA, reading >> 32);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA, reading);

    /* =========================MSBFirst========================= */
    bitOrder = MSBFIRST;
    // 8 bit shift in
    shiftOutput = 0xAA;
    mod = (((uint8_t)-1) / 2) + 1;
    reading = AVRIO::Pin::shiftIn<uint8_t>(DATAIN, CLKOUT, AVRIO::bit_order::MSBFirst);
    TEST_ASSERT_EQUAL_UINT8(0xAA, reading);

    // 16 bit shift in
    shiftOutput = 0xAAAA;
    mod = ((uint16_t)-1) / 2 + 1;
    reading = AVRIO::Pin::shiftIn<uint16_t>(DATAIN, CLKOUT, AVRIO::bit_order::MSBFirst);
    TEST_ASSERT_EQUAL_UINT16(0xAAAA, reading);

    // 32 bit shift in
    shiftOutput = 0xAAAAAAAA;
    mod = ((uint32_t)-1) / 2 + 1;
    reading = AVRIO::Pin::shiftIn<uint32_t>(DATAIN, CLKOUT, AVRIO::bit_order::MSBFirst);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA, reading);

    // 64 bit shift in
    shiftOutput = 0xAAAAAAAAAAAAAAAA;
    mod = ((uint64_t)-1) / 2 + 1;
    reading = AVRIO::Pin::shiftIn<uint64_t>(DATAIN, CLKOUT, AVRIO::bit_order::MSBFirst);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA, reading >> 32);
    TEST_ASSERT_EQUAL_UINT32(0xAAAAAAAA, reading);

    CLKIN.detachInterrupt();
}

void shiftio_test_setUp(void) {
    AVRIO::Pin::initializePins(DATAOUT, CLKOUT, CLKIN, DATAIN);
}
void shiftio_test_tearDown(void) {
    CLKOUT.pinMode(AVRIO::pin_m::Input);
    DATAOUT.pinMode(AVRIO::pin_m::Input);
    CLKIN.pinMode(AVRIO::pin_m::Input);
    DATAIN.pinMode(AVRIO::pin_m::Input);
}