#define MENU_BLINK_DELAY    300
#define MENU_KEY_DELAY      200

long menuLastBlink;
int8_t menuPosition = 0;
int8_t menuPositionOld = -1;
bool menuBlank = false;

void karmes_MainMenu()
{
    gameEnded = true;
    
    while (!idleMode)
    {
        readKeys();

        if (gameEnded)
        {
            gameEnded = false;
            menuPosition = 0;
            menuPositionOld = -1;
            drawMainMenu();
        }

        if (menuLastBlink + MENU_BLINK_DELAY < millis())
        {
            if (menuBlank) menuBlank = false;
            else menuBlank = true;
            drawMenuPosition(menuPosition, menuBlank);
            menuLastBlink = millis();
        }

        if (keyButton1)
        {
            for (uint8_t i = 0; i < 9; i++)
            {
                movePixelsLeftRow(matrix, 0, 7, false);
                movePixelsLeftRow(matrix, 1, 7, false);
                movePixelsLeftRow(matrix, 2, 7, false);
                updateDisplay(matrix);
            }

            if (menuPosition == 0) // play
            {
                resetKeys();
                gameStarted();
                resetKeys();
                gameEnded = true;
            }

            else if (menuPosition == 1) // settings (ei käytössä)
            {
                resetKeys();
                resetKeys();
                gameEnded = true;
            }
            
            else if (menuPosition == 2) // hi-scores (ei käytössä)
            {
                resetKeys();
                resetKeys();
                gameEnded = true;
            }
            
            else if (menuPosition == 3) // exit
            {
                resetKeys();
                resetKeys();
                gameEnded = true;
                idleMode = true;
                digitalWrite(PIN_DISABLE_KEYS, LOW);
            }
        }

        else if (keyUp) 
        {
            resetKeys();
            delay(MENU_KEY_DELAY);
            menuPosition--;
            if (menuPosition < 0) menuPosition = 3;
            else if (menuPosition > 3) menuPosition = 0;
            drawMenuPosition(menuPosition, false);
            menuLastBlink = millis();
        }

        else if (keyDown) 
        {
            resetKeys();
            delay(MENU_KEY_DELAY);
            menuPosition++;
            if (menuPosition < 0) menuPosition = 3;
            else if (menuPosition > 3) menuPosition = 0;
            drawMenuPosition(menuPosition, false);
            menuLastBlink = millis();
        }
    }
}

void drawMainMenu()
{
    clearPixels(matrix, true);

    setText(matrix, fontBig, "kärmes 2", 9, 8, 50, true);

    for (uint8_t i = 0; i < 7; i++)
    {
        movePixelsUp(matrix, 1, true);
    }

    setPixel(matrix, 31, 10, false);       // kolmiot menuvalinnan ylä- ja alapuolella
    setPixel(matrix, 30, 11, false);
    setPixel(matrix, 31, 11, false);
    setPixel(matrix, 32, 11, false);
    
    setPixel(matrix, 30, 21, false);
    setPixel(matrix, 31, 21, false);
    setPixel(matrix, 32, 21, false);
    setPixel(matrix, 31, 22, false);
    
    drawMenuPosition(0, false);
}

void drawMenuPosition(int8_t pos, bool blank)
{
    clearRect(matrix, 0, 12, 63, 20, false);
    
    if (blank) updateDisplay(matrix);
    else 
    {
        if (pos == 0)      setText(matrix, fontSmall, "play", 22, 12, 0, true);
        else if (pos == 1) setText(matrix, fontSmall, "settings", 15, 12, 0, true);
        else if (pos == 2) setText(matrix, fontSmall, "hi-scores", 12, 12, 0, true);
        else if (pos == 3) setText(matrix, fontSmall, "exit", 25, 12, 0, true);
    }
}
