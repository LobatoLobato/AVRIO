#include "AVRIO.h"
namespace AVRIO {
uint32_t readVcc() {
    if (bit_is_set(ADCSRA, ADSC))
        return 0;
    // Read 1.1V reference against AVcc
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(2);             // Wait for Vref to settle
    ADCSRA |= _BV(ADSC);  // Convert
    while (bit_is_set(ADCSRA, ADSC))
        ;
    long result = 1126400L / ADC;  // Back-calculate AVcc in mV
    analogRead(A0);
    return result;
}
}  // namespace AVRIO