#include "mbed.h"
#include "drivers/LCD_DISCO_F429ZI.h"

// Constants for gyroscope setup
#define GYRO_CONFIG_REG 0x20
#define GYRO_ENABLE 0x0F
#define OUT_X_L 0x28  // Register address for X-axis low byte
#define OUT_Y_L 0x2A  // Register address for Y-axis low byte
#define OUT_Z_L 0x2C  // Register address for Z-axis low byte
#define SCALE_FACTOR 0.07 // Degrees per second per LSB

#define SAMPLE_FREQ 100.0f // Sampling frequency in Hz
#define SAMPLE_PERIOD_MS (1000.0f / SAMPLE_FREQ)

// SPI pins and CS pin for the gyroscope
SPI spi(PF_9, PF_8, PF_7); // MOSI, MISO, SCK
DigitalOut cs(PC_1);       // Chip select for the gyroscope
LCD_DISCO_F429ZI lcd;

// Buffer for SPI communication
uint8_t tx_buffer[2];
uint8_t rx_buffer[6];  // Increased buffer size for multiple axes

// Initialize the gyroscope
void initGyro() {
    cs = 0;
    tx_buffer[0] = GYRO_CONFIG_REG;
    tx_buffer[1] = GYRO_ENABLE;
    spi.write((const char*)tx_buffer, 2, (char*)rx_buffer, 2);  // Send configuration to the gyroscope, with correct type casting
    cs = 1;
}

// Read angular rate from all three axes
void readAngularRate(float& x, float& y, float& z) {
    cs = 0;
    tx_buffer[0] = OUT_X_L | 0x80 | 0x40;  // Read multiple bytes starting from X low
    spi.write((const char*)tx_buffer, 1, (char*)rx_buffer, 6); // Read 6 bytes for XYZ, with correct type casting
    cs = 1;

    // Combine high and low bytes for each axis
    int16_t raw_x = (rx_buffer[1] << 8) | rx_buffer[0];
    int16_t raw_y = (rx_buffer[3] << 8) | rx_buffer[2];
    int16_t raw_z = (rx_buffer[5] << 8) | rx_buffer[4];

    // Convert to degrees per second
    x = raw_x * SCALE_FACTOR;
    y = raw_y * SCALE_FACTOR;
    z = raw_z * SCALE_FACTOR;
}

// this function is use to detect when the gyro data crosses zero (change in the direction of motion)
int count_zero_crossings(int16_t* data, int size, int16_t threshold) {
    int count = 0;
    for (int i = 1; i < size; i++) {
        if ((data[i - 1] > threshold && data[i] < -threshold) || (data[i - 1] < -threshold && data[i] > threshold)) {
            count++;
        }
    }
    return count;
}

int main() {
    spi.format(8, 3); // 8 bits per frame, Mode 3
    spi.frequency(1000000); // 1 MHz

    const int windowSize = 10;
    int16_t x_data[windowSize], y_data[windowSize], z_data[windowSize];
    int index = 0;
    int treshhold = 100;     //treshhold used to count cross-zero

    initGyro();

    while (true) {
        float x_dps, y_dps, z_dps;
        readAngularRate(x_dps, y_dps, z_dps);
        printf("Rates -> X: %.2f dps, Y: %.2f dps, Z: %.2f dps\n", x_dps, y_dps, z_dps);

        x_data[index] = x_dps;
        y_data[index] = y_dps;
        z_data[index] = z_dps;
        index++;

        if (index >= windowSize) {
            int x_crossings = count_zero_crossings(x_data, windowSize, treshhold);
            int y_crossings = count_zero_crossings(y_data, windowSize, treshhold);
            int z_crossings = count_zero_crossings(z_data, windowSize, treshhold);

            float x_freq = (x_crossings / 2.0) / (windowSize * SAMPLE_PERIOD_MS / 1000.0);
            float y_freq = (y_crossings / 2.0) / (windowSize * SAMPLE_PERIOD_MS / 1000.0);
            float z_freq = (z_crossings / 2.0) / (windowSize * SAMPLE_PERIOD_MS / 1000.0);

            lcd.Clear(LCD_COLOR_BLUE);
            lcd.SetBackColor(LCD_COLOR_BLUE);
            lcd.SetTextColor(LCD_COLOR_WHITE);

            char buffer[32];
            sprintf(buffer, "X Freq: %.2f Hz", x_freq);
            lcd.DisplayStringAt(0, LINE(5), (uint8_t *)buffer, CENTER_MODE);
            sprintf(buffer, "Y Freq: %.2f Hz", y_freq);
            lcd.DisplayStringAt(0, LINE(6), (uint8_t *)buffer, CENTER_MODE);
            sprintf(buffer, "Z Freq: %.2f Hz", z_freq);
            lcd.DisplayStringAt(0, LINE(7), (uint8_t *)buffer, CENTER_MODE);

            index = 0; // Reset index to start collecting new window of data
        }

        ThisThread::sleep_for(10ms);
    }
}