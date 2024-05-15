#include <header.h>

/*Calculate decibel value from RMS*/ 
float calculate_dB(float rms) {
    return 20.0f * log10(rms);
}