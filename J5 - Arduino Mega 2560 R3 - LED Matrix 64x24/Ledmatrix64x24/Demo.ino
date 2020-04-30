void initialize()
{
    clearPixels(matrix, true);
    delay(2000);
    setText(matrix, fontBig, "welcome!", (64 - getTextWidth(fontBig, "welcome!")) / 2, 8, 100, true);
    delay(2000);
    demo_explosion_center();
    clearPixels(matrix, true);
    delay(2000);
    startup = false;
}

void arcade()
{
    bool textSet = false;

    while (idleMode)
    {
        if (digitalRead(PIN_DEMO_ANIMATION))
        {
            textSet = gameNamePrinted = false;
            if (digitalRead(PIN_DEMO_ANIMATION)) demo_arcade();
            if (digitalRead(PIN_DEMO_ANIMATION)) demo_textWave(7);
            if (digitalRead(PIN_DEMO_ANIMATION)) demo_insertCoinScroll();
            if (digitalRead(PIN_DEMO_ANIMATION)) demo_insertCoinBlink();
            if (digitalRead(PIN_DEMO_ANIMATION)) demo_or();
            if (digitalRead(PIN_DEMO_ANIMATION)) demo_insertManyCoins();
        }

        else
        {
            quit = false;
            serialEvent();
            
            if (gameMode)
            {
                if (!gameNamePrinted)
                {
                    char charGame[stringGame.length() + 1];
                    stringGame.toCharArray(charGame, stringGame.length() + 1);
                    int gameLen = getTextWidth(fontBig, charGame);
                    size_t strLen = strlen(charGame);
                
                    char rowText[3][30] = {'\0'};
                    int row = 0;
                    int col = 0;
                    int i = 0;
                    int lastSpaceCol = 0;
                    int lastSpaceI = 0;
                
                    while (row < 3 && col < 29)
                    {
                        if (charGame[i] == 32) 
                        {
                            lastSpaceCol = col;
                            lastSpaceI = i;
                        }
                        
                        rowText[row][col] = charGame[i];
                        
                        if (getTextWidth(fontBig, rowText[row]) > 64)
                        {
                            if (lastSpaceCol > 0)
                            {
                                i = lastSpaceI + 1;
                                rowText[row][lastSpaceCol] = '\0';
                                row++;
                                col = 0;
                                lastSpaceCol = lastSpaceI = 0;
                            }
                            else
                            {
                                rowText[row][col] = '\0';
                                row++;
                                col = 0;
                                lastSpaceCol = lastSpaceI = 0;
                            }
                        }
                        else
                        {
                            col++;
                            i++;
                        }
                    }
                    
                    int numRows = 3;
                    if (rowText[1][0] == '\0') numRows = 1;
                    else if (rowText[2][0] == '\0') numRows = 2;

                    for (int i = 0; i < 10; i++) movePixelsUp(matrix, 4, true);
                    clearPixels(matrix, false);
                    
                    if (numRows == 1)
                    {
                        setText(matrix, fontBig, rowText[0], (64 - getTextWidth(fontBig, rowText[0])) / 2, 8, 50, true);
                    }
                    else if (numRows == 2)
                    {
                        setText(matrix, fontBig, rowText[0], (64 - getTextWidth(fontBig, rowText[0])) / 2, 4, 50, true);
                        setText(matrix, fontBig, rowText[1], (64 - getTextWidth(fontBig, rowText[1])) / 2, 12, 50, true);
                    }
                    else
                    {
                        setText(matrix, fontBig, rowText[0], (64 - getTextWidth(fontBig, rowText[0])) / 2, 0, 50, true);
                        setText(matrix, fontBig, rowText[1], (64 - getTextWidth(fontBig, rowText[1])) / 2, 8, 50, true);
                        setText(matrix, fontBig, rowText[2], (64 - getTextWidth(fontBig, rowText[2])) / 2, 16, 50, true);
                    }
                    
                    gameNamePrinted = true;
                    textSet = false;
                }
            }

            else
            {
                if (!textSet)
                {
                    if (gameNamePrinted)
                    {
                        for (int i = 0; i < 10; i++) movePixelsDown(matrix, 4, true);
                    }
                    clearPixels(matrix, true);
                    scrollText(matrix, fontBig, "arcade", 1, (64 - getTextWidth(fontBig, "arcade")) / 2, 0);
    
                    gameNamePrinted = false;
                    textSet = true;
                }
            }
        }
    }
}

void arcade_test()
{
    quit = false;

    demo_arcade();
    demo_textWave(9);
    demo_insertCoinScroll();
    demo_insertCoinBlink();
    demo_or();
    demo_insertManyCoins();
    
    karmes_MainMenu();
}

// ************** DEMO **************

void demo_arcade()
{
    if (quit) return;

    clearPixels(matrix, true);

    uint8_t del = 50;
    
    for (uint8_t i = 0; i < 2; i++)
    {
        setText(matrix, fontBig, "a", 15, 8, del, true);
        clearText(matrix, fontBig, "a", 15, 8, del, false);

        setText(matrix, fontBig, "r", 21, 8, del, true);
        clearText(matrix, fontBig, "r", 21, 8, del, false);

        setText(matrix, fontBig, "c", 27, 8, del, true);
        clearText(matrix, fontBig, "c", 27, 8, del, false);

        setText(matrix, fontBig, "a", 33, 8, del, true);
        clearText(matrix, fontBig, "a", 33, 8, del, false);

        setText(matrix, fontBig, "d", 39, 8, del, true);
        clearText(matrix, fontBig, "d", 39, 8, del, false);

        setText(matrix, fontBig, "e", 45, 8, del, true);
        clearText(matrix, fontBig, "e", 45, 8, del, true);
        
        d(50);

        setText(matrix, fontBig, "e", 45, 8, del, true);
        clearText(matrix, fontBig, "e", 45, 8, del, false);

        setText(matrix, fontBig, "d", 39, 8, del, true);
        clearText(matrix, fontBig, "d", 39, 8, del, false);

        setText(matrix, fontBig, "a", 33, 8, del, true);
        clearText(matrix, fontBig, "a", 33, 8, del, false);
        
        setText(matrix, fontBig, "c", 27, 8, del, true);
        clearText(matrix, fontBig, "c", 27, 8, del, false);

        setText(matrix, fontBig, "r", 21, 8, del, true);
        clearText(matrix, fontBig, "r", 21, 8, del, false);

        setText(matrix, fontBig, "a", 15, 8, del, true);
        clearText(matrix, fontBig, "a", 15, 8, del, true);

        d(700);
    }

        setText(matrix, fontBig, "a", 15, 8, del, true);
        setText(matrix, fontBig, "r", 21, 8, del, true);
        setText(matrix, fontBig, "c", 27, 8, del, true);
        setText(matrix, fontBig, "a", 33, 8, del, true);
        setText(matrix, fontBig, "d", 39, 8, del, true);
        setText(matrix, fontBig, "e", 45, 8, del, true);

        uint8_t x1 = 12;
        uint8_t x2 = 52;
        uint8_t y1 = 5;
        uint8_t y2 = 17;

        for (uint8_t x = x1; x <= x2; x += 2) setPixel(matrix, x, y2, true);
        for (uint8_t y = y2 - 2; y >= y1; y -= 2) setPixel(matrix, x2, y, true);
        for (uint8_t x = x2; x >= x1; x -= 2) setPixel(matrix, x, y1, true);
        for (uint8_t y = y1; y <= y2; y += 2) setPixel(matrix, x1, y, true);
        
        x1 = 10;
        x2 = 54;
        y1 = 3;
        y2 = 19;

        for (uint8_t y = y2 - 2; y >= y1; y -= 2) setPixel(matrix, x1, y, true);
        for (uint8_t x = x1; x <= x2; x += 2) setPixel(matrix, x, y1, true);
        for (uint8_t y = y1; y <= y2; y += 2) setPixel(matrix, x2, y, true);
        for (uint8_t x = x2; x >= x1; x -= 2) setPixel(matrix, x, y2, true);

        for (uint8_t i = 0; i < 12; i++)
        {
            clearText(matrix, fontBig, "arcade", 15, 8, 0, true);
            d(50);
            setText(matrix, fontBig, "arcade", 15, 8, 0, true);
            d(50);
        }

        d(2000);

        for (uint8_t y = 0; y < 24; y++) 
        {
            setPixel(matrix, 0, y, false);
            setPixel(matrix, 63, 23 - y, true);
        }
        
        curtainClose(matrix, 1);
        clearPixels(matrix, true);

        d(1000);
}

void demo_textWave(uint8_t count)
{
    if (quit) return;

    bool show[10] = { false, false, false, false, false, false, false, false, false, false };
    
    for (uint8_t c = 0; c < count; c++)
    {
        int h[10];
    
        for (int i = -90; i < 90; i += 4)
        {
            for (uint8_t ii = 0; ii < 10; ii++)
            {
                h[ii] =  9 + (sin((i + (ii * 10)) * (3.14 / 90)) * 9);
            }

            if (c == 0)
            {
                if (i > -10) show[0] = true;
                if (i > 0)   show[1] = true;
                if (i > 10)  show[2] = true;
                if (i > 20)  show[3] = true;
                if (i > 30)  show[4] = true;
                if (i > 40)  show[5] = true;
                if (i > 50)  show[6] = true;
                if (i > 60)  show[7] = true;
                if (i > 70)  show[8] = true;
                if (i > 80)  show[9] = true;
            }

            if (c == count - 1)
            {
                if (i > -90) show[0] = false;
                if (i > -80) show[1] = false;
                if (i > -70) show[2] = false;
                if (i > -60) show[3] = false;
                if (i > -50) show[4] = false;
                if (i > -40) show[5] = false;
                if (i > -30) show[6] = false;
                if (i > -20) show[7] = false;
                if (i > -10) show[8] = false;
                if (i > 0)   show[9] = false;
            }
            
            clearPixels(matrix, false);
            if (show[0]) setText(matrix, fontBig, "i", 3, h[0], 0, false);
            if (show[1]) setText(matrix, fontBig, "n", 6, h[1], 0, false);
            if (show[2]) setText(matrix, fontBig, "s", 12, h[2], 0, false);
            if (show[3]) setText(matrix, fontBig, "e", 18, h[3], 0, false);
            if (show[4]) setText(matrix, fontBig, "r", 23, h[4], 0, false);
            if (show[5]) setText(matrix, fontBig, "t", 29, h[5], 0, false);
            if (show[6]) setText(matrix, fontBig, "c", 40, h[6], 0, false);
            if (show[7]) setText(matrix, fontBig, "o", 46, h[7], 0, false);
            if (show[8]) setText(matrix, fontBig, "i", 52, h[8], 0, false);
            if (show[9]) setText(matrix, fontBig, "n", 55, h[9], 0, false);
            updateDisplay(matrix);
        }
    }

    d(1000);
}

void demo_insertCoinScroll()
{
    if (quit) return;
    
    clearPixels(matrix, true);
    scrollText(matrix, fontBig, "insert coin", 1, 3, 0);
    d(200);
}

void demo_insertCoinBlink()
{
    if (quit) return;

    int blinkDelay = 500;

    clearPixels(matrix, true);
    clearPixels(matrixBuffer1, false);
    setText(matrix, fontBig, "insert coin", 3, 8, 0, false);

    for (uint8_t i = 0; i < 8; i++)
    {
        if (quit) break;
        d(blinkDelay);
        updateDisplay(matrix);
        d(blinkDelay);
        updateDisplay(matrixBuffer1);
    }

    d(1000);
}

void demo_or()
{
    if (quit) return;

    clearPixels(matrix, true);
    setText(matrix, fontBig, "or...", 25, 8, 500, true);
    d(2000);

    for (uint8_t i = 0; i < 5; i++)
    {
        if (quit) break;
        movePixelsDown(matrix, 4, true);
    }
}

void demo_insertManyCoins()
{
    if (quit) return;

    int blinkDelay = 20;

    clearPixels(matrix, true);
    clearPixels(matrixBuffer1, false);
    scrollText(matrix, fontBig, "insert", 0, 15, 0);
    scrollText(matrix, fontBig, "many", 1, 19, 0);
    scrollText(matrix, fontBig, "coins!", 2, 17, 0);
    d(2500);
    
    updateDisplay(matrix);

    for (uint8_t i = 0; i < 15; i++) 
    {
        if (quit) break;
        movePixelsLeftRow(matrix, 0, 4, true);
    }
    
    for (uint8_t i = 0; i < 15; i++) 
    {
        if (quit) break;
        movePixelsRightRow(matrix, 1, 4, true);
    }
    
    for (uint8_t i = 0; i < 2; i++)
    {
        if (quit) break;
        movePixelsUp(matrix, 4, true);
    }
    
    for (uint8_t i = 0; i < 3; i++)
    {
        if (quit) break;
        movePixelsRightRow(matrix, 1, 3, true);
    }

    setText(matrix, fontBig, "all", 7, 8, 100, true);

    for (uint8_t i = 0; i < 40; i++)
    {
        if (quit) break;
        d(blinkDelay);
        updateDisplay(matrix);
        d(blinkDelay);
        updateDisplay(matrixBuffer1);
    }
    
    d(1000);
}

//void demo_randomClear(int times)
//{
//    for (int t = 0; t < times; t++)
//    {
//        uint8_t x = random(64);
//        uint8_t y = random(24);
//
//        if (getPixel(matrix, x, y))
//        {
//            clearPixel(matrix, x, y, false);
//            updateDisplay(matrix);
//        }
//    }
//}
//
//void demo_moveScreenDown()
//{
//    for (uint8_t i = 0; i < 24; i++)
//    {
//        movePixelsDown(matrix, 1, true);
//    }
//}
//
void demo_explosion(uint8_t times)
{
    for (uint8_t t = 0; t < times; t++)
    {
        uint8_t x = random(64);
        uint8_t y = random(24);
        
        for (int i = 1; i < 25; i++)
        {
            clearPixels(matrix, false);
            setCircle(matrix, x, y, i, false);

            if (i == 1 || i == 4)
            {
                blinkDisplay(matrix);
            }
            
            else updateDisplay(matrix);
        }
    }
}

void demo_explosion_center()
{
    uint8_t x = 32;
    uint8_t y = 12;
    
    for (int i = 1; i < 25; i++)
    {
        clearPixels(matrix, false);
        setCircle(matrix, x, y, i, false);

        if (i == 1 || i == 4)
        {
            blinkDisplay(matrix);
        }
        
        else updateDisplay(matrix);
    }
}
//
//void demo_rain(uint8_t times)
//{
//    clearPixels(matrix, false);
//    uint8_t count = 0;
//    uint8_t dir = 0;
//    
//    for (uint8_t i = times; i > 0; i--)
//    {
//        movePixelsDown(matrix, 1, false);
//
//        if (count > (times - 20) / 2) dir = 1;
//
//        if (dir == 0) count++;
//        else if (count > 0) count--;
//        
//        for (uint8_t ii = 0; ii < count / 6; ii++)
//        {
//            uint8_t x = random(64);
//            setPixel(matrix, x, 0, false);
//        }
//
//        updateDisplay(matrix);
//    }
//}
//
//void demo_curves(int times)
//{
//    uint8_t dir = 0;
//    float w = 4;
//
//    float curve1 = 25;
//    float curve2 = 40;
//    float curve3 = 55;
//    
//    for (int t = 0; t < times; t++)
//    {
//        for (float ii = 0.0; ii < 6.2; ii += 0.4)
//        {
//            if (w > 8) dir = 1;
//            if (w < 2) dir = 0;
//    
//            if (dir == 0) w += 0.2;
//            else w -= 0.2;
//
//            curve1 -= .2;
//            curve2 -= .2;
//            curve3 -= .2;
//            
//            if (curve1 < -25) curve1 = 25;
//            if (curve2 < -25) curve2 = 25;
//            if (curve3 < -25) curve3 = 25;
//            
//            clearPixels(matrix, false);
//
//            for (float i = 0.0; i < 6.4; i += 0.1)
//            {
//                int c = cos(i + ii) * w;
//                int s = sin(i + ii) * w;
//                int cp = cos(i + ii + PI) * w;
//                
//                setPixel(matrix, i * 10, 12 + c + curve1, false);
//                setPixel(matrix, i * 10, 12 + s + curve2, false);
//                setPixel(matrix, i * 10, 12 + cp + curve3, false);
//            }
//            
//            updateDisplay(matrix);
//        }
//    }
//
//    setPixels(matrix, true);
//    clearPixels(matrix, true);
//}
//
//void demo_insertCoinLetter(uint8_t count)
//{
//    if (quit) return;
//
//    int del = 40;
//
//    for (uint8_t c = 0; c < count; c++)
//    {
//        setText(matrix, fontBig, "i", 3, 7, del, true);
//        setText(matrix, fontBig, "n", 6, 8, del, true);
//        setText(matrix, fontBig, "s", 12, 9, del, true);
//        setText(matrix, fontBig, "e", 18, 10, del, true);
//        setText(matrix, fontBig, "r", 23, 11, del, true);
//        setText(matrix, fontBig, "t", 29, 11, del, true);
//        setText(matrix, fontBig, "c", 40, 10, del, true);
//        setText(matrix, fontBig, "o", 46, 9, del, true);
//        setText(matrix, fontBig, "i", 52, 8, del, true);
//        setText(matrix, fontBig, "n", 55, 7, del, true);
//    
//        d(1000);
//        
//        clearText(matrix, fontBig, "i", 3, 7, del, true);
//        clearText(matrix, fontBig, "n", 6, 8, del, true);
//        clearText(matrix, fontBig, "s", 12, 9, del, true);
//        clearText(matrix, fontBig, "e", 18, 10, del, true);
//        clearText(matrix, fontBig, "r", 23, 11, del, true);
//        clearText(matrix, fontBig, "t", 29, 11, del, true);
//        clearText(matrix, fontBig, "c", 40, 10, del, true);
//        clearText(matrix, fontBig, "o", 46, 9, del, true);
//        clearText(matrix, fontBig, "i", 52, 8, del, true);
//        clearText(matrix, fontBig, "n", 55, 7, del, true);
//    
//        d(100);
//    
//        setText(matrix, fontBig, "i", 3, 11, del, true);
//        setText(matrix, fontBig, "n", 6, 10, del, true);
//        setText(matrix, fontBig, "s", 12, 9, del, true);
//        setText(matrix, fontBig, "e", 18, 8, del, true);
//        setText(matrix, fontBig, "r", 23, 7, del, true);
//        setText(matrix, fontBig, "t", 29, 7, del, true);
//        setText(matrix, fontBig, "c", 40, 8, del, true);
//        setText(matrix, fontBig, "o", 46, 9, del, true);
//        setText(matrix, fontBig, "i", 52, 10, del, true);
//        setText(matrix, fontBig, "n", 55, 11, del, true);
//    
//        d(1000);
//        
//        clearText(matrix, fontBig, "i", 3, 11, del, true);
//        clearText(matrix, fontBig, "n", 6, 10, del, true);
//        clearText(matrix, fontBig, "s", 12, 9, del, true);
//        clearText(matrix, fontBig, "e", 18, 8, del, true);
//        clearText(matrix, fontBig, "r", 23, 7, del, true);
//        clearText(matrix, fontBig, "t", 29, 7, del, true);
//        clearText(matrix, fontBig, "c", 40, 8, del, true);
//        clearText(matrix, fontBig, "o", 46, 9, del, true);
//        clearText(matrix, fontBig, "i", 52, 10, del, true);
//        clearText(matrix, fontBig, "n", 55, 11, del, true);
//    }
//}
//
//void demo_pushPixelsRight()
//{
//    if (quit) return;
//
//    for (uint8_t x = 0; x < 65; x++)
//    {
//        for (uint8_t y = 0; y < 24; y++)
//        {
//            bool push = getPixel(matrix, x, y);
//            for (uint8_t xx = x; xx < 64; xx++)
//            {
//                if (getPixel(matrix, xx, y) && push) 
//                {
//                    push = getPixel(matrix, xx + 1, y);
//                    setPixel(matrix, xx + 1, y, false);
//                }
//            }
//            
//            setPixel(matrix, x, y, false);
//            clearPixel(matrix, x - 1, y, false);
//        }
//        
//        updateDisplay(matrix);
//    }
//
//    d(1000);
//}

