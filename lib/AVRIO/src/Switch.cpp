// #include <AVRIO.h>
// namespace AVRIO {

// Switch::Switch(Pin pin, input_m i_mode, edge_t r_mode)
//     : _pin(pin), i_mode(i_mode), r_mode(r_mode) {
//     pin.pinMode((pin_m)i_mode);
// }

// void Switch::setInputMode(input_m mode) const {
//     this->i_mode = mode;
//     this->_pin.pinMode((pin_m)mode);
// }

// void Switch::setReadMode(edge_t mode) const {
//     this->r_mode = mode;
// }

// uint8_t Switch::read() const {
//     return this->_pin.digitalRead(this->r_mode);
// }

// }  // namespace AVRIO