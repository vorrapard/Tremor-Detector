#include <header.h>

// Calculate RMS value for the given data
float calculateRMS(float *data, int size) {
    float sum = 0.0f;
    for (int i = 0; i < size; i++) {
        sum += data[i] * data[i];
    }
    return sqrt(sum / size);
}