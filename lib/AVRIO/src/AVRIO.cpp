#include "AVRIO.h"

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

namespace AVRIO {
uint32_t readVcc() {
    if (bit_is_set(ADCSRA, ADSC)) {
        return 0;
    }

    // Read 1.1V reference against AVcc
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(2);  // Wait for Vref to settle

    // Start the conversion
    sbi(ADCSRA, ADSC);

    // Wait until the conversion is finished
    while (bit_is_set(ADCSRA, ADSC)) {
    }

    // Back-calculate AVcc in mV
    long result = 1126400L / ADC;

    // Reset the analog reference to previous value
    ADMUX |= Pin::getAnalogReference();

    return result;
}
}  // namespace AVRIO