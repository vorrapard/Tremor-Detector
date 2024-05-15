///////////////////////////////////////////////////////////////////////////////////////////
// TREMOR DETECTOR
// Team Details --------------------------------------------------------------------------
// 1) Soumili Nandi - sn3699
// 2) Vorrapard Kumthongdee - vk2584
// 3) Yi Guo - yg3273
///////////////////////////////////////////////////////////////////////////////////////////


#include <mbed.h>
#include <stdio.h>
#include "drivers/LCD_DISCO_F429ZI.h"

// Gyro control registers and configurations
#define CTRL_REG1 0x20
#define CTRL_REG1_CONFIG 0b01'10'1'1'1'1
#define CTRL_REG4 0x23
#define CTRL_REG4_CONFIG 0b0'0'01'0'00'0
#define SPI_FLAG 1
#define OUT_X_L 0x28

// Constants for calculation
#define SCALING_FACTOR (17.5f * 0.0174532925199432957692236907684886f / 1000.0f)
#define SAMPLE_FREQ 100.0f // Sampling frequency in Hz
#define SAMPLE_PERIOD_MS (1000.0f / SAMPLE_FREQ)

// Functions 
void initGyro();
void spi_cb(int event);
int countCrossings(float *data, int size, float threshold);
int16_t convertComplement(uint16_t data);
void displayAlert(float frequency, float avg_db);
void displayFrequency(float x_freq, float y_freq, float z_freq);
float calculateRMS(float *data, int size);
float calculate_dB(float rms);

extern EventFlags flags;
extern LCD_DISCO_F429ZI lcd;
extern SPI spi;

