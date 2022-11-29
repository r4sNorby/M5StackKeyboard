#include <M5Stack.h>
#include "keyboard.h"

Keyboard keyboard;

void setup(void)
{
    M5.begin();
    Wire.begin();
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setBrightness(100);

    M5.Speaker.setVolume(1);
    M5.Speaker.update();
}

void loop()
{
    if (M5.BtnA.pressedFor(500))
    {
        keyboard.showkeyboard = true;
        M5.Speaker.tone(661, 120);
        M5.Lcd.setTextSize(1);
        const char *s = keyboard.keyboard();
        M5.Speaker.tone(661, 120);
        M5.Lcd.setTextSize(2);
        M5.lcd.print(s);
    }
    delay(1);
    M5.update();
}