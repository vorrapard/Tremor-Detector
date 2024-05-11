#include "mbed.h"
#include "drivers/LCD_DISCO_F429ZI.h"

#define SAMPLE_FREQ 100.0f // Sampling frequency in Hz
#define SAMPLE_PERIOD_MS (1000.0f / SAMPLE_FREQ)

SPI spi(PF_9, PF_8, PF_7); // MOSI, MISO, SCK
DigitalOut cs(PC_1); // Chip select
LCD_DISCO_F429ZI lcd;

void init_gyro() {
    cs = 0;
    spi.write(0x20 | 0x00); // CTRL_REG1
    spi.write(0x0F); // Turn on the sensor, enable x, y, and z axis
    cs = 1;

    cs = 0;
    spi.write(0x23 | 0x00); // CTRL_REG4
    spi.write(0x00); // Set scale (250 dps full scale)
    cs = 1;
}

void read_gyro(int16_t &x, int16_t &y, int16_t &z) {
    char out[6];
    cs = 0;
    spi.write(0x28 | 0x80); // Read from OUT_X_L, MSB at high to read multiple bytes
    for (int i = 0; i < 6; ++i) {
        out[i] = spi.write(0x00); // Dummy write to read data
    }
    cs = 1;

    x = (out[1] << 8) | out[0];
    y = (out[3] << 8) | out[2];
    z = (out[5] << 8) | out[4];
}

// this function is use to detect when the gyro data crosses zero (change in the direction of motion)
int count_zero_crossings(int16_t* data, int size) {
    int count = 0;
    for (int i = 1; i < size; i++) {
        if ((data[i - 1] > 0 && data[i] < 0) || (data[i - 1] < 0 && data[i] > 0)) {
            count++;
        }
    }
    return count;
}

int main() {
    init_gyro();
    spi.format(8, 3);
    spi.frequency(1000000);

    const int windowSize = 100;
    int16_t x_data[windowSize], y_data[windowSize], z_data[windowSize];
    int index = 0;

    while (true) {
        int16_t raw_x, raw_y, raw_z;
        read_gyro(raw_x, raw_y, raw_z);

        x_data[index] = raw_x;
        y_data[index] = raw_y;
        z_data[index] = raw_z;
        index++;

        if (index >= windowSize) {
            int x_crossings = count_zero_crossings(x_data, windowSize);
            int y_crossings = count_zero_crossings(y_data, windowSize);
            int z_crossings = count_zero_crossings(z_data, windowSize);

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