int getTextWidth(uint8_t font[], char text[])
{
    int textWidth = 0;
    size_t len = strlen(text);
    
    for (int i = 0; i < len; i++)
    {
        uint8_t character = text[i];

        if (character == 165)                   character = 123;    // å
        else if (character == 164)              character = 124;    // ä
        else if (character == 182)              character = 125;    // ö
        else if (character == 133)              character = 91;     // Å
        else if (character == 132)              character = 92;     // Ä
        else if (character == 150)              character = 93;     // Ö
        else if (character == 195)              continue;                                                 // Jätetään for-silmukka kesken jos on ääkkösen eka merkki (195+???)

        if (character > 96 && character < 126)  character -= 32;
        
        int location = (character - 32) * 8;                                                              // Merkin sijainti "characters"-arrayssa (ensimmäinen merkki on asciin 32:s merkki)
        uint8_t width = pgm_read_byte(&font[location + 7]);                                               // Merkin pituus "characters"-arraysta (jokaisen merkin kahdeksas rivi)
        
        textWidth += width;
    }

    return textWidth - 1;
}

void scrollText(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t font[], char text[], uint8_t row, uint8_t spaces, int scrollDelay)
{
    if (quit) return;
    
    size_t len = strlen(text);                                                                            // Scrollattavan tekstin pituus

    for (int i = 0; i < len + spaces; i++)                                                                // Käydään teksti läpi merkki merkiltä (lisänä "spaces" määrä tyhjiä pikseleitä)
    {
        if (quit) break;
        
        uint8_t character = text[i];                                                                      // Merkin ascii-numeroarvo
        
        if (character == 165)                   character = 123;    // å
        else if (character == 164)              character = 124;    // ä
        else if (character == 182)              character = 125;    // ö
        else if (character == 133)              character = 91;     // Å
        else if (character == 132)              character = 92;     // Ä
        else if (character == 150)              character = 93;     // Ö
        else if (character == 195)              continue;                                                 // Jätetään for-silmukka kesken jos on ääkkösen eka merkki (195+???)

        if (character > 96 && character < 126)  character -= 32;
        
        int location = (character - 32) * 8;                                                              // Merkin sijainti "characters"-arrayssa (ensimmäinen merkki on asciin 32:s merkki)
        uint8_t width = pgm_read_byte(&font[location + 7]);                                               // Merkin pituus "characters"-arraysta (jokaisen merkin kahdeksas rivi)
        if (i >= len) width = 1;                                                                          // Jos mennään yli tekstin pituuden, lisättävä tyhjä pituus 1 pikseli
        
        for (int col = 0; col < width; col++)
        {
            movePixelsLeftRow(var, row, 1, false);                                                        // Siirretään rivin jokainen pikseli vasemmalle
            for (int y = 0 + (row * 8); y < 7 + (row * 8); y++)
            {
                bool pixel = (pgm_read_byte(&font[y - (row * 8) + location]) >> (7 - col)) & 1;           // Luetaan "characters"-arraysta onko pikseli päällä vai ei
                if (i < len ) var[7][y] ^= (-pixel ^ var[7][y]) & (1 << (0));                             // Merkitään näytön puskurin oikeaan reunaan onko pikseli päällä vai ei
            }
            
            updateDisplayRow(var, row);                                                                   // Päivitetään näyton rivi
            if (scrollDelay > 0) delay(scrollDelay);                                                      // Hidastetaan
        }
    }
}

void setText(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t font[], char text[], uint8_t x, uint8_t y, uint16_t scrollDelay, bool refresh)
{
    if (quit) return;
    
    size_t len = strlen(text);
    
    for (int i = 0; i < len; i++)
    {                                                                                                     
        if (quit) break;

        uint8_t character = text[i];

        if (character == 165)                   character = 123;    // å
        else if (character == 164)              character = 124;    // ä
        else if (character == 182)              character = 125;    // ö
        else if (character == 133)              character = 91;     // Å
        else if (character == 132)              character = 92;     // Ä
        else if (character == 150)              character = 93;     // Ö
        else if (character == 195)              continue;

        if (character > 96 && character < 126)  character -= 32;

        int location = (character - 32) * 8;
        uint8_t width = pgm_read_byte(&font[location + 7]);
        
        for (int x2 = 0; x2 < width; x2++)
        {
            for (int y2 = 0; y2 < 7; y2++)
            {
                bool pixel = (pgm_read_byte(&font[y2 + location]) >> (7 - x2)) & 1;
                if (pixel) setPixel(var, x2 + x, y2 + y, false);
            }
        }
        
        if (scrollDelay > 0 && refresh)
        {
            d(scrollDelay);
            updateDisplay(var);
        }
        
        x += width;
    }

    if (refresh) updateDisplay(var);
}

void clearText(uint8_t var[MATRIX_X][MATRIX_Y], uint8_t font[], char text[], uint8_t x, uint8_t y, uint16_t scrollDelay, bool refresh)
{
    if (quit) return;
    
    size_t len = strlen(text);
    
    for (int i = 0; i < len; i++)
    {                                                                                                     
        if (quit) break;

        uint8_t character = text[i];

        if (character == 165)                   character = 123;    // å
        else if (character == 164)              character = 124;    // ä
        else if (character == 182)              character = 125;    // ö
        else if (character == 133)              character = 91;     // Å
        else if (character == 132)              character = 92;     // Ä
        else if (character == 150)              character = 93;     // Ö
        else if (character == 195)              continue;

        if (character > 96 && character < 126)  character -= 32;

        int location = (character - 32) * 8;
        uint8_t width = pgm_read_byte(&font[location + 7]);
        
        for (int x2 = 0; x2 < width; x2++)
        {
            for (int y2 = 0; y2 < 7; y2++)
            {
                bool pixel = (pgm_read_byte(&font[y2 + location]) >> (7 - x2)) & 1;
                if (pixel) clearPixel(var, x2 + x, y2 + y, false);
            }
        }
        
        if (scrollDelay > 0 && refresh)
        {
            d(scrollDelay);
            updateDisplay(var);
        }
        
        x += width;
    }

    if (refresh) updateDisplay(var);
}
