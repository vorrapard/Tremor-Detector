#include <header.h>

// Convert complement value to its original value
int16_t convertComplement(uint16_t data) {
    if (data & (1 << 15)) { // if the value is negative
        data = data ^ 0xFFFF;
        return -(data + 1);
    } else {
        return data;
    }
}