#include <header.h>

// Display tremor intensity on the LCD screen
void displayAlert(float frequency, float avg_db) {
    lcd.SetTextColor(LCD_COLOR_WHITE);
    lcd.SetBackColor(LCD_COLOR_BLACK);

    if(frequency >= 3.0f && frequency <= 6.0f) {
        if (frequency >= 3.0f && frequency < 4.0f) {
            lcd.Clear(LCD_COLOR_GREEN);
            lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"Tremor Intensity: LOW", CENTER_MODE);
        } else if (frequency >= 4.0f && frequency < 5.0f) {
            lcd.Clear(LCD_COLOR_YELLOW);
            lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"Tremor Intensity: MEDIUM", CENTER_MODE);
        } else if (frequency >= 5.0f && frequency <= 6.0f) {
            lcd.Clear(LCD_COLOR_RED);
            lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"Tremor Intensity: HIGH", CENTER_MODE);
        }
        char buffer[32];
        sprintf(buffer, "Intensity %.2f dB", avg_db);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)buffer, CENTER_MODE);
    }
}