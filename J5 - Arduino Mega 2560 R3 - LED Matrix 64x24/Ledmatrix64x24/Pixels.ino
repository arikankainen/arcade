// ************** PIXEL **************

void setPixel(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t x, uint8_t y, bool refresh)
{
    if (quit) return;

    if (x < PIXELS_X && y < PIXELS_Y)
    {
        uint8_t column = x / 8;
        uint8_t pixel = x - (column * 8);
        
        var[column][y] ^= (-1 ^ var[column][y]) & (1 << (7 - pixel));
    }

    if (refresh) updateDisplay(var);
}

void clearPixel(uint8_t var[MATRIX_X][MATRIX_Y], int x, int y, bool refresh)
{
    if (quit) return;

    if (x < PIXELS_X && y < PIXELS_Y)
    {
        uint8_t column = x / 8;
        uint8_t pixel = x - (column * 8);
        
        var[column][y] ^= (-0 ^ var[column][y]) & (1 << (7 - pixel));
    }

    if (refresh) updateDisplay(var);
}

void invertPixel(uint8_t var[MATRIX_X][MATRIX_Y], int x, int y, bool refresh)
{
    if (quit) return;

    if (x < PIXELS_X && y < PIXELS_Y)
    {
        uint8_t column = x / 8;
        uint8_t pixel = x - (column * 8);
        
        var[column][y] ^= 1 << (7 - pixel);
    }

    if (refresh) updateDisplay(var);
}

uint8_t getPixel(uint8_t var[MATRIX_X][MATRIX_Y], int x, int y)
{
    if (quit) return;

    if (x < PIXELS_X && y < PIXELS_Y)
    {
        uint8_t column = x / 8;
        uint8_t pixel = x - (column * 8);
        
        return (var[column][y] >> (7 - pixel)) & 1;
    }
    
    else return false;
}

// ************** RECTANGLE **************

void setRect(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t x, uint8_t y, uint8_t x2, uint8_t y2, bool refresh)
{
    if (quit) return;

    for (uint8_t yy = y; yy <= y2; yy++)
    {
        for (uint8_t xx = x; xx <= x2; xx++)
        {
            setPixel(var, xx, yy, false);
        }
    }

    if (refresh) updateDisplay(var);
}

void clearRect(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t x, uint8_t y, uint8_t x2, uint8_t y2, bool refresh)
{
    if (quit) return;

    for (uint8_t yy = y; yy <= y2; yy++)
    {
        for (uint8_t xx = x; xx <= x2; xx++)
        {
            clearPixel(var, xx, yy, false);
        }
    }

    if (refresh) updateDisplay(var);
}

void invertRect(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t x, uint8_t y, uint8_t x2, uint8_t y2, bool refresh)
{
    if (quit) return;

    for (uint8_t yy = y; yy <= y2; yy++)
    {
        for (uint8_t xx = x; xx <= x2; xx++)
        {
            invertPixel(var, xx, yy, false);
        }
    }

    if (refresh) updateDisplay(var);
}

// ************** CIRCLE **************

void setCircle(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t x, uint8_t y, uint8_t radius, bool refresh)
{
    if (quit) return;

    for (double i = 0; i <= (PI * 2); i += .2)
    {
        int x2 = cos(i) * radius;
        int y2 = sin(i) * radius;
        setPixel(var, x + x2, y + y2, false);
    }

    if (refresh) updateDisplay(var);
}

// ************** FULL SCREEN **************

void setPixels(uint8_t var[MATRIX_X][MATRIX_Y], bool refresh)
{
    if (quit) return;

    for (uint8_t x = 0; x < MATRIX_X; x++)
    {
        for (uint8_t y = 0; y < MATRIX_Y; y++)
        {
            var[x][y] = B11111111;
        }
    }

    if (refresh) updateDisplay(var);
}

void clearPixels(uint8_t var[MATRIX_X][MATRIX_Y], bool refresh)
{
    if (quit) return;

    for (uint8_t x = 0; x < MATRIX_X; x++)
    {
        for (uint8_t y = 0; y < MATRIX_Y; y++)
        {
            var[x][y] = B00000000;
        }
    }

    if (refresh) updateDisplay(var);
}

void invertPixels(uint8_t var[MATRIX_X][MATRIX_Y], bool refresh)
{
    if (quit) return;

    for (uint8_t y = 0; y < PIXELS_Y; y++)
    {
        for (uint8_t x = 0; x < PIXELS_X; x++)
        {
            invertPixel(var, x, y, false);
        }
    }

    if (refresh) updateDisplay(var);
}

// ************** ONE ROW **************

void setRow(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t row, bool refresh)
{
    if (quit) return;

    for (uint8_t y = row * 8; y < row * 8 + 8; y++)
    {
        for (uint8_t x = 0; x < PIXELS_X; x++)
        {
            setPixel(var, x, y, false);
        }
    }

    if (refresh) updateDisplay(var);
}

void clearRow(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t row, bool refresh)
{
    if (quit) return;

    for (uint8_t y = row * 8; y < row * 8 + 8; y++)
    {
        for (uint8_t x = 0; x < PIXELS_X; x++)
        {
            clearPixel(var, x, y, false);
        }
    }

    if (refresh) updateDisplay(var);
}

void invertRow(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t row, bool refresh)
{
    if (quit) return;

    for (uint8_t y = row * 8; y < row * 8 + 8; y++)
    {
        for (uint8_t x = 0; x < PIXELS_X; x++)
        {
            invertPixel(var, x, y, false);
        }
    }

    if (refresh) updateDisplay(var);
}

// ************** MATRIX **************

void copyMatrix(uint8_t from[MATRIX_X][MATRIX_Y], uint8_t to[MATRIX_X][MATRIX_Y])
{
    if (quit) return;

    for (uint8_t x = 0; x < MATRIX_X; x++)
    {
        for (uint8_t y = 0; y < MATRIX_Y; y++)
        {
            to[x][y] = from[x][y];
        }
    }
}

void appendMatrix(uint8_t from[MATRIX_X][MATRIX_Y], uint8_t to[MATRIX_X][MATRIX_Y], bool refresh)
{
    if (quit) return;

    for (uint8_t x = 0; x < MATRIX_X; x++)
    {
        for (uint8_t y = 0; y < MATRIX_Y; y++)
        {
            if (!getPixel(to, x, y)) to[x][y] = from[x][y];
        }
    }

    if (refresh) updateDisplay(to);
}

// ************** EFFECTS **************

void blinkDisplay(uint8_t var[MATRIX_X][MATRIX_Y])
{
    if (quit) return;

    invertPixels(var, true);
    invertPixels(var, true);
}

void flipVertically(uint8_t var[MATRIX_X][MATRIX_Y], bool refresh)
{
    if (quit) return;

    uint8_t varTemp[MATRIX_X][MATRIX_Y];
    
    for (uint8_t y = 0; y < PIXELS_Y; y++)
    {
        for (uint8_t x = 0; x < PIXELS_X; x++)
        {
            if (getPixel(var, x, y)) setPixel(varTemp, x, PIXELS_Y - 1 - y, false);
            else clearPixel(varTemp, x, PIXELS_Y - 1 - y, false);
        }
    }

    copyMatrix(varTemp, var);
    if (refresh) updateDisplay(var);
}

void flipHorizontally(uint8_t var[MATRIX_X][MATRIX_Y], bool refresh)
{
    if (quit) return;

    uint8_t varTemp[MATRIX_X][MATRIX_Y];
    
    for (uint8_t y = 0; y < PIXELS_Y; y++)
    {
        for (uint8_t x = 0; x < PIXELS_X; x++)
        {
            if (getPixel(var, x, y)) setPixel(varTemp, PIXELS_X - 1 - x, y, false);
            else clearPixel(varTemp, PIXELS_X - 1 - x, y, false);
        }
    }

    copyMatrix(varTemp, var);
    if (refresh) updateDisplay(var);
}

void curtainClose(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t pixels)
{
    if (quit) return;
    
    uint8_t varTemp[MATRIX_X][MATRIX_Y];
    copyMatrix(var, varTemp);
    
    uint8_t count = 0;

    for (uint8_t x = 0; x < PIXELS_X / 2; x++)
    {
        for (uint8_t y = 0; y < PIXELS_Y; y++)
        {
            clearPixel(varTemp, PIXELS_X - 1 - x, y, false);
            clearPixel(varTemp, x, y, false);
            setPixel(varTemp, x + 1, y, false);
            setPixel(varTemp, PIXELS_X - 2 - x, y, false);
        }
        
        count++;
        if (count == pixels)
        {
            updateDisplay(varTemp);
            count = 0;
        }

        if (quit) break;
    }
}

void curtainOpen(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t pixels)
{
    if (quit) return;

    uint8_t varTemp[MATRIX_X][MATRIX_Y];
    clearPixels(varTemp, false);

    uint8_t count = 0;

    for (uint8_t x = 0; x < PIXELS_X / 2 + 1; x++)
    {
        for (uint8_t y = 0; y < PIXELS_Y; y++)
        {
            setPixel(varTemp, PIXELS_X / 2 - x, y, false);
            setPixel(varTemp, x + PIXELS_X / 2, y, false);
            if (!getPixel(var, PIXELS_X / 2 + 1 - x, y)) clearPixel(varTemp, PIXELS_X / 2 + 1 - x, y, false);
            if (!getPixel(var, x + PIXELS_X / 2 - 1, y)) clearPixel(varTemp, x + PIXELS_X / 2 - 1, y, false);
        }

        count++;
        
        if (count == pixels)
        {
            updateDisplay(varTemp);
            count = 0;
        }

        if (quit) break;
    }
    
    updateDisplay(var);
}

void movePixelsUp(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t pixels, bool refresh)
{
    if (quit) return;

    for (uint8_t y = 0; y < PIXELS_Y - pixels; y++)
    {
        for (uint8_t x = 0; x < PIXELS_X; x++)
        {
            if (getPixel(var, x, y + pixels)) setPixel(var, x, y, false);
            else clearPixel(var, x, y, false);

            if (y == PIXELS_Y - 1 - pixels)
            {
                for (uint8_t yy = 0; yy < pixels; yy++)
                {
                     clearPixel(var, x, PIXELS_Y - 1 - yy, false);
                }
            }
        }
    }

    if (refresh) updateDisplay(var);
}

void movePixelsDown(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t pixels, bool refresh)
{
    if (quit) return;

    for (uint8_t y = PIXELS_Y - 1; y > pixels - 1; y--)
    {
        for (uint8_t x = 0; x < PIXELS_X; x++)
        {
            if (getPixel(var, x, y - pixels)) setPixel(var, x, y, false);
            else clearPixel(var, x, y, false);

            if (y == pixels)
            {
                for (uint8_t yy = 0; yy < pixels; yy++)
                {
                     clearPixel(var, x, yy, false);
                }
            }
        }
    }

    if (refresh) updateDisplay(var);
}

void movePixelsLeft(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t pixels, bool refresh)
{
    if (quit) return;

    for (uint8_t y = 0; y < PIXELS_Y; y++)
    {
        for (uint8_t x = 0; x < PIXELS_X - pixels; x++)
        {
            if (getPixel(var, x + pixels, y)) setPixel(var, x, y, false);
            else clearPixel(var, x, y, false);

            if (x == PIXELS_X - 1 - pixels)
            {
                for (uint8_t xx = 0; xx < pixels; xx++)
                {
                     clearPixel(var, PIXELS_X - 1 - xx, y, false);
                }
            }
        }
    }

    if (refresh) updateDisplay(var);
}

void movePixelsRight(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t pixels, bool refresh)
{
    if (quit) return;

    for (uint8_t y = 0; y < PIXELS_Y; y++)
    {
        for (uint8_t x = PIXELS_X - 1; x > pixels - 1; x--)
        {
            if (getPixel(var, x - pixels, y)) setPixel(var, x, y, false);
            else clearPixel(var, x, y, false);

            if (x == pixels)
            {
                for (uint8_t xx = 0; xx < pixels; xx++)
                {
                     clearPixel(var, xx, y, false);
                }
            }
        }
    }

    if (refresh) updateDisplay(var);
}

void movePixelsLeftRow(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t row, uint8_t pixels, bool refresh)
{
    if (quit) return;

    for (uint8_t y = 0; y < PIXELS_Y; y++)
    {
        for (uint8_t x = 0; x < PIXELS_X - pixels; x++)
        {
            if (y / 8 == row)
            {
                if (getPixel(var, x + pixels, y)) setPixel(var, x, y, false);
                else clearPixel(var, x, y, false);
    
                if (x == PIXELS_X - 1 - pixels)
                {
                    for (uint8_t xx = 0; xx < pixels; xx++)
                    {
                         clearPixel(var, PIXELS_X - 1 - xx, y, false);
                    }
                }
            }
        }
    }

    if (refresh) updateDisplayRow(var, row);
}

void movePixelsRightRow(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t row, uint8_t pixels, bool refresh)
{
    if (quit) return;

    for (uint8_t y = 0; y < PIXELS_Y; y++)
    {
        for (uint8_t x = PIXELS_X - 1; x > pixels - 1; x--)
        {
            if (y / 8 == row)
            {
                if (getPixel(var, x - pixels, y)) setPixel(var, x, y, false);
                else clearPixel(var, x, y, false);
    
                if (x == pixels)
                {
                    for (uint8_t xx = 0; xx < pixels; xx++)
                    {
                         clearPixel(var, xx, y, false);
                    }
                }
            }
        }
    }

    if (refresh) updateDisplayRow(var, row);
}

// ************** UPDATE DISPLAY **************

void updateDisplay(uint8_t var[MATRIX_X][MATRIX_Y])
{
    if (quit) return;

    for (uint8_t y = 0; y < MATRIX_Y; y++)
    {
        uint8_t row = y / 8;

        max7219_BeginUpdate(row);
        for (uint8_t x = 0; x < MATRIX_X; x++)
        {
            max7219_Data(y + 1 - (row * 8), var[x][y], row);
        }
        max7219_EndUpdate(row);
    }
}

void updateDisplayRow(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t row)
{
    if (quit) return;

    for (uint8_t y = 0; y < MATRIX_Y; y++)
    {
        if (y / 8 == row)
        {
            max7219_BeginUpdate(row);
            for (uint8_t x = 0; x < MATRIX_X; x++)
            {
                max7219_Data(y + 1 - (row * 8), var[x][y], row);
            }
            max7219_EndUpdate(row);
        }
    }
}
