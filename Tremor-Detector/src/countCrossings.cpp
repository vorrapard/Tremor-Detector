#include <header.h>

// Function to detect when the gyro data crosses zero (change in the direction of motion)
int countCrossings(float *data, int size, float threshold) {
    int count = 0;
    for (int i = 1; i < size; i++) {
        if ((data[i - 1] > threshold && data[i] < -threshold) || (data[i - 1] < -threshold && data[i] > threshold)) {
            count++;
        }
    }
    return count;
}