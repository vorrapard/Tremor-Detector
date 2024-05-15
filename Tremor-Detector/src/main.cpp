///////////////////////////////////////////////////////////////////////////////////////////
// TREMOR DETECTOR
// Team Details --------------------------------------------------------------------------
// 1) Soumili Nandi - sn3699
// 2) Vorrapard Kumthongdee - vk2584
// 3) Yi Guo - yg3273
///////////////////////////////////////////////////////////////////////////////////////////

#include <header.h>

EventFlags flags;
LCD_DISCO_F429ZI lcd;
SPI spi(PF_9, PF_8, PF_7, PC_1, use_gpio_ssel); // SPI pins for gyroscope

int main() {
    
    initGyro();         //  Initialize the gyroscope.

    spi.format(8, 3);   // Configure SPI format and frequency.
    spi.frequency(1'000'000);

    uint8_t write_buf[32], read_buf[32];
    const int windowSize = 100;     // Number of samples to collect.
    float x_data[windowSize], y_data[windowSize], z_data[windowSize];       // Data arrays used in frequency calculation.
    int index = 0;

    while (1) {
        uint16_t raw_gx, raw_gy, raw_gz;        // Raw data from the gyro
        int16_t original_gx, original_gy, original_gz;  // Converted raw data
        float gx, gy, gz;       // Scaled data

        write_buf[0] = OUT_X_L | 0x80 | 0x40;   // Prepare to read the gyroscope values starting from OUT_X_L  
        spi.transfer(write_buf, 7, read_buf, 7, spi_cb);    // Perform the SPI transfer to read 6 bytes of data (for x, y, and z axes)
        flags.wait_all(SPI_FLAG);

        /* Convert the received data into 16-bit integers for each axis. */ 
        raw_gx = (((uint16_t)read_buf[2]) << 8) | ((uint16_t)read_buf[1]);
        raw_gy = (((uint16_t)read_buf[4]) << 8) | ((uint16_t)read_buf[3]);
        raw_gz = (((uint16_t)read_buf[6]) << 8) | ((uint16_t)read_buf[5]);

        /* The value is expressed as 2’s complement so it needs to be translated */
        original_gx = convertComplement(raw_gx);
        original_gy = convertComplement(raw_gy);
        original_gz = convertComplement(raw_gz);

        /* Scale data by scaling factor. */
        gx = ((float)original_gx) * SCALING_FACTOR;
        gy = ((float)original_gy) * SCALING_FACTOR;
        gz = ((float)original_gz) * SCALING_FACTOR;

        /* Add data to the array until the window is full. */
        x_data[index] = gx;
        y_data[index] = gy;
        z_data[index] = gz;
        index++;

        /* If the window is full, process the data. */
        if (index >= windowSize) {
            /* Count how many times the dataset has cross zero (change of direction). */
            int x_crossings = countCrossings(x_data, windowSize, 0.05);
            int y_crossings = countCrossings(y_data, windowSize, 0.1);
            int z_crossings = countCrossings(z_data, windowSize, 0.2);

            /* Calculate the frequency based on zero crossings. */
            float x_freq = (x_crossings / 2.0) / (windowSize * SAMPLE_PERIOD_MS / 1000.0);
            float y_freq = (y_crossings / 2.0) / (windowSize * SAMPLE_PERIOD_MS / 1000.0);
            float z_freq = (z_crossings / 2.0) / (windowSize * SAMPLE_PERIOD_MS / 1000.0);
            float avg_freq = (x_freq + y_freq + z_freq) / 3.0f;     // Calculate average frequency

            /* Displaying frequency on the LED Screen. */
            displayFrequency(x_freq, y_freq, z_freq);
            if(avg_freq >= 3.0f && avg_freq <= 6.0f) {              // Tremor detected
                /* Decibel calculation for intensity. */            
                float x_rms = calculateRMS(x_data, windowSize);         // Calculate RMS values
                float y_rms = calculateRMS(y_data, windowSize);
                float z_rms = calculateRMS(z_data, windowSize);
            
                float avg_rms = (x_rms + y_rms + z_rms) / 3.0f;         // Average RMS      
                float avg_db = calculate_dB(avg_rms);                   // dB values      

                displayAlert(avg_freq, avg_db);         // Display tremor alert
            }

            index = 0; // Reset index to start collecting new window of data
        }

        thread_sleep_for(SAMPLE_PERIOD_MS);
    }
}