#include <M5Stack.h>

class Keyboard
{
public:
    bool showkeyboard = false;
    char keymap[2][4][10] = {{{'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'}, {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '~'}, {'^', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ' ', '<'}, {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'}}, {{'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'}, {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', '~'}, {'^', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ' ', '<'}, {'<', '>', ',', '.', '-', '_', '+', '*', '/', '\\'}}};
    int shift = 1;
    int csel = 0;
    int rsel = 0;
    int oldcsel = -1;
    int oldrsel = -1;

#define KEYSTRING_BUFFER_SIZE (15)
    char keystring[KEYSTRING_BUFFER_SIZE];
    char oldkeystring[KEYSTRING_BUFFER_SIZE];

    void updateKeyboard()
    {
        // M5.Speaker.tone(561, 40); //frequency 3000, with a duration of 200ms
        int x, y;
        M5.Lcd.fillRect(0, 80, 320, 128, BLACK); // Default for second num was 112
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 10; c++)
            {
                x = (c * 32);
                y = (80 + (r * 32)); // Default for first num was 112
                if ((csel == c) && (rsel == r))
                {
                    M5.Lcd.setTextColor(TFT_BLACK, TFT_WHITE);
                    M5.Lcd.fillRect(x, y, 32, 32, WHITE);
                }
                else
                {
                    M5.Lcd.drawRect(x, y, 32, 32, BLUE);
                }

                if ((c == 9) && (r == 1))
                {
                    M5.Lcd.drawString("<-'", x + 10, y + 7, 2);
                }
                else if ((c == 8) && (r == 2))
                {
                    M5.Lcd.drawString("__", x + 7, y + 7, 2);
                }
                else if ((c == 9) && (r == 2))
                {
                    M5.Lcd.drawString("<-", x + 10, y + 7, 2);
                }
                else
                {
                    M5.Lcd.drawString(String(keymap[shift][r][c]), x + 12, y + 7, 2);
                }
                M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
            }
        }
        oldcsel = csel;
        oldrsel = rsel;
    }

    void updateText()
    {
        if (showkeyboard && keystring != oldkeystring)
        {
            M5.Lcd.fillRect(0, 48, 320, 30, BLACK); // Default second num 80

            char *text = keystring;
            M5.Lcd.drawString(text, 0, 48, 4); // Default second num 80
        }
    }

    char *keyboard()
    {
        // Setup
        M5.Lcd.drawRect(0, 208, 106, 32, BLUE);
        M5.Lcd.drawRect(106, 208, 107, 32, BLUE);
        M5.Lcd.drawRect(213, 208, 106, 32, BLUE);
        M5.Lcd.drawString("Kolonne", 30, 215, 2); // Default first num 80
        M5.Lcd.drawString("Vaelg", 143, 215, 2);  // Default first num 80
        M5.Lcd.drawString("Raekke", 245, 215, 2); // Default first num 80

        keystring[0] = '|';
        updateText();
        keystring[0] = 0;
        updateKeyboard();

        // Keyboard while loop
        while (showkeyboard)
        {
            if (M5.BtnA.wasPressed())
            {
                csel = csel + 1;
                if (csel > 9)
                {
                    csel = 0;
                }
                updateKeyboard();
            }
            if (M5.BtnC.wasPressed())
            {
                rsel = rsel + 1;
                if (rsel > 3)
                {
                    rsel = 0;
                }
                updateKeyboard();
            }

            if (M5.BtnB.wasPressed())
            {
                int len = strlen(keystring);
                int cursorPipe;

                if ((rsel == 2) && (csel == 0))
                {
                    shift++;
                    if (shift > 1)
                        shift = 0;
                    updateKeyboard();
                }
                else
                {
                    if ((rsel == 1) && (csel == 9))
                    {
                        M5.Lcd.clear();
                        showkeyboard = false;
                    }
                    else if ((rsel == 2) && (csel == 9))
                    {
                        if (len > 0)
                        {
                            cursorPipe = len - 1;
                        }
                        else
                        {
                            cursorPipe = len;
                        }
                    }
                    else
                    {
                        keystring[len] = keymap[shift][rsel][csel];
                        if (len == 0 && shift == 1)
                        {
                            shift = 0;
                            updateKeyboard();
                        }
                        cursorPipe = len + 1;
                    }
                    keystring[cursorPipe] = '|';
                    updateText();
                    keystring[cursorPipe] = 0;
                }
            }
            strcpy(oldkeystring, keystring);
            delay(1);
            M5.update();
        }
        return keystring;
    }
};