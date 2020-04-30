void explosion(uint8_t x, uint8_t y)
{
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

void rain(uint8_t times)
{
    clearPixels(matrix, false);
    uint8_t count = 0;
    uint8_t dir = 0;
    
    for (uint8_t i = times; i > 0; i--)
    {
        movePixelsDown(matrix, 1, false);

        if (count > (times - 20) / 2) dir = 1;

        if (dir == 0) count++;
        else if (count > 0) count--;
        
        for (uint8_t ii = 0; ii < count / 6; ii++)
        {
            uint8_t x = random(64);
            setPixel(matrix, x, 0, false);
        }

        updateDisplay(matrix);
    }
}
