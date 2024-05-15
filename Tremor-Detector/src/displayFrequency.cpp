#include <header.h>

// Display the frequency data on the LCD screen
void displayFrequency(float x_freq, float y_freq, float z_freq) {
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
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"No tremors!", CENTER_MODE);
    
}