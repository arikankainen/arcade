#define EEPROM_ADDR_HISCORE1    0
#define EEPROM_ADDR_HISCORE2    2
#define EEPROM_ADDR_HISCORE3    4
#define EEPROM_ADDR_HINAME1     6
#define EEPROM_ADDR_HINAME2     16
#define EEPROM_ADDR_HINAME3     26

#define BLINK_DELAY             100
#define GAME_DELAY              80
#define RECT_DELAY              2
#define TAIL_GROW_SPEED         5

uint16_t points;

uint8_t tailBufferX[1000];
uint8_t tailBufferY[1000];

uint8_t tailDir;
uint16_t tailLen;
uint8_t tailX;
uint8_t tailY;
uint8_t randomX1;
uint8_t randomY1;
uint8_t randomX2;
uint8_t randomY2;
uint8_t sinceLastRect;

long lastMove;
long lastBlink;

bool blinkOn;
bool gameEnded = true;

void readEEPROM()
{
//    hiScore = (int)EEPROM.read(eepromAddrHiscore);                  // luetaan hiscore EEPROMista
//    settingsSound = (bool)EEPROM.read(eepromAddrSound);             // luetaan ääniasetukset EEPROMista
//    settingsBrightness = (int)EEPROM.read(eepromAddrBrightness);    // luetaan kirkkausasetukset EEPROMista

//    EEPROM.write(eepromAddrSound, (byte)settingsSound);     // kirjoitetaan ääniasetus EEPROMiin
}

void gameStarted()
{
    sinceLastRect = RECT_DELAY;
    points = 0;
    tailLen = 50;
    tailDir = 1;                                // 0 = Up, 1 = Right, 2 = Down, 3 = Left
    tailX = 12;
    tailY = 12;
    gameEnded = false;
    lastMove = millis();
    
    for (uint16_t i = 0; i < 1000; i++) tailBufferX[i] = tailBufferY[i] = 0;

    clearPixels(matrix, true);
    addRandom(1);
    addRandom(2);
    addSquare();
    addSquare();
    addSquare();
    addSquare();
    addSquare();
    
    while (!gameEnded && !idleMode)
    {
        readKeys();
    
        if (lastMove + GAME_DELAY < millis()) moveTail();
        if (lastBlink + BLINK_DELAY < millis()) blinkRandom();
    }
}

void addSquare()
{
    bool randomOk;
    uint8_t count = 0;
    
    do
    {
        count++;
        randomOk = true;
        uint8_t rectX = random(63);
        uint8_t rectY = random(23);
        
        randomOk = setRectIfEmpty(rectX, rectY, rectX + 1, rectY + 1);

        if (tailDir == 1 && (rectY == tailY || rectY + 1 == tailY) && rectX < tailX + 11)            randomOk = false;   // Ei laiteta neliötä liian lähelle kärmeksen menosuuntaa
        else if (tailDir == 3 && (rectY == tailY || rectY + 1 == tailY) && rectX + 1 > tailX - 11)   randomOk = false;
        else if (tailDir == 2 && (rectX == tailX || rectX + 1 == tailX) && rectY < tailY + 11)       randomOk = false;
        else if (tailDir == 0 && (rectX == tailX || rectX + 1 == tailX) && rectY + 1 > tailY - 11)   randomOk = false;
        
    } while (!randomOk && count < 100);    // Yritetään löytää sopiva paikka 100 kertaa, jos ei löydy niin luovutetaan
}

bool setRectIfEmpty(uint8_t x, uint8_t y, uint8_t x2, uint8_t y2)
{
    bool set = true;

    uint8_t border = 3;
    int8_t checkX = x - border;
    int8_t checkY = y - border;
    int8_t checkX2 = x2 + border;
    int8_t checkY2 = y2 + border;

    if (checkX < 0) checkX = 0;
    if (checkY < 0) checkY = 0;
    if (checkX2 > PIXELS_X - 1) checkX2 = PIXELS_X - 1;
    if (checkY2 > PIXELS_Y - 1) checkY2 = PIXELS_Y - 1;
    
    for (uint8_t yy = checkY; yy <= checkY2; yy++)
    {
        for (uint8_t xx = checkX; xx <= checkX2; xx++)
        {
            if (getPixel(matrix, xx, yy)) set = false;
            else if (xx == randomX1 && yy == randomY1) set = false;
            else if (xx == randomX2 && yy == randomY2) set = false;
        }
    }

    if (set) setRect(matrix, x, y, x2, y2, false);
    return set;
}

void collected()
{
    if (points > 0 && points % 10 == 0) flipTailHoriz();
    if (points % 2 == 0) addSquare();
    
    if (tailX == randomX1 && tailY == randomY1) addRandom(1);
    else if (tailX == randomX2 && tailY == randomY2) addRandom(2);
}

void addRandom(uint8_t randomNo)
{
    bool randomOk;
    uint8_t randomX;
    uint8_t randomY;
    
    do 
    {
        randomOk = true;
        randomX = random(64);
        randomY = random(24);
        
        if (tailDir == 1 || tailDir == 3)                                       // madon suunta oikea tai vasen
        {
            if (randomY == tailY) randomOk = false;                             // ei samalle riville madon kanssa uutta pistettä
        }
        
        if (tailDir == 0 || tailDir == 2)                                       // madon suunta ylös tai alas
        {
            if (randomX == tailX) randomOk = false;                             // ei samalle riville madon kanssa uutta pistettä
        }
        
        if (getPixel(matrix, randomX, randomY)) randomOk = false;                // ei lisätä pistettä entisen pikselin päälle
    
    } while (!randomOk);

    if (randomNo == 1)
    {
        randomX1 = randomX;
        randomY1 = randomY;
        setPixel(matrix, randomX1, randomY1, true);
    }
    else if (randomNo == 2)
    {
        randomX2 = randomX;
        randomY2 = randomY;
        setPixel(matrix, randomX2, randomY2, true);
    }
}

void blinkRandom()
{
    if (blinkOn)
    {
        setPixel(matrix, randomX1, randomY1, false);
        setPixel(matrix, randomX2, randomY2, true);
        blinkOn = false;
    }
    else
    {
        clearPixel(matrix, randomX1, randomY1, false);
        clearPixel(matrix, randomX2, randomY2, true);
        blinkOn = true;
    }

    lastBlink = millis();
}

void moveTail()
{
    if (keyUp && tailDir != 2)      tailDir = 0;
    if (keyRight && tailDir != 3)   tailDir = 1;
    if (keyDown && tailDir != 0)    tailDir = 2;
    if (keyLeft && tailDir != 1)    tailDir = 3;
    resetKeys();
    
    if (tailDir == 0)       tailY--;
    else if (tailDir == 1)  tailX++;
    else if (tailDir == 2)  tailY++;
    else if (tailDir == 3)  tailX--;

    clearPixel(matrix, tailBufferX[tailLen - 1], tailBufferY[tailLen - 1], false);
    checkCollision();
    setPixel(matrix, tailX, tailY, false);
    pushTail(tailX, tailY);
    updateDisplay(matrix);

    lastMove = millis();
}

void flipTailHoriz()
{
    clearPixel(matrix, randomX1, randomY1, false);
    clearPixel(matrix, randomX2, randomY2, false);

    for (uint16_t i = 0; i < 1000; i++)
    {
        tailBufferX[i] = 63 - tailBufferX[i];
    }

    if (tailDir == 1) tailDir = 3;
    else if (tailDir == 3) tailDir = 1;

    tailX = 63 - tailX;
    randomX1 = 63 - randomX1;
    randomX2 = 63 - randomX2;

    copyMatrix(matrix, matrixBuffer1);
    clearRect(matrixBuffer1, 14, 7, 49, 15, false);
    setText(matrixBuffer1, fontBig, "flip!", 22, 8, 80, false);
    invertRect(matrixBuffer1, 14, 7, 49, 15, false);

    for (uint8_t i = 0; i < 6; i++)
    {
        invertRect(matrixBuffer1, 14, 7, 49, 15, true);
        delay(150);
    }

    curtainClose(matrix, 2);
    flipHorizontally(matrix, false);
    curtainOpen(matrix, 2);
    delay(100);
}

void checkCollision()
{
    bool col = false;
    
    if ((tailX == randomX1 && tailY == randomY1) || (tailX == randomX2 && tailY == randomY2))
    {
        points++;
        tailLen += TAIL_GROW_SPEED;
        collected();
        blinkDisplay(matrix);
    }
    else 
    {
        if (tailX > 63 || tailY > 23) col = true;;
    
        for (uint16_t i = 1; i < tailLen; i++) {
            if (tailX == tailBufferX[i] && tailY == tailBufferY[i]) col = true;
        }

        if (getPixel(matrix, tailX, tailY)) col = true;
    }

    if (col) collision();
}

void collision()
{
    gameEnded = true;
    explosion(tailX, tailY);
    rain(50);
    
    clearPixels(matrix, true);
    scrollText(matrix, fontBig, "score", 1, 25, 0);

    char charPoints[5];
    itoa(points, charPoints, 10);

    for (uint8_t i = 0; i < 5; i++)
    {
        if (points < 10) setText(matrix, fontBig, charPoints, 49, 8, 0, true);
        else setText(matrix, fontBig, charPoints, 43, 8, 0, true);
        
        delay(500);
        clearRect(matrix, 43, 8, 63, 15, true);
        delay(100);
    }
    
    setPixels(matrix, true);
}

void pushTail(uint8_t x, uint8_t y)
{
    for (uint16_t i = 999; i > 0; i--)
    {
         tailBufferX[i] = tailBufferX[i - 1];
         tailBufferY[i] = tailBufferY[i - 1];
    }

    tailBufferX[0] = x;
    tailBufferY[0] = y;
}
