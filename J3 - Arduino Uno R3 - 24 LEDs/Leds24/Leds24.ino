#define SPEAKER_DELAY       3000    // viive ennenkuin kauittimet kytketään vahvistimiin
#define BLINK_DELAY         300     // viive valojen vilkutuksen välissä
#define INIT_DELAY          100     // viive valojen alustuksessa
#define RELAY_DELAY         2000     // viive releen reagoinnissa

#define PIN_DATA            6       // OUT  74HC595 pin 14 (SER)
#define PIN_LATCH           5       // OUT  74HC595 pin 12 (RCLK)
#define PIN_CLOCK           4       // OUT  74HC595 pin 11 (SRCLK)

#define PIN_IDLE1           2       // IN   HIGH = pelaajan 1 ohjainta ei käytetty vähään aikaan (Arduino Leonardo 1)
#define PIN_IDLE2           3       // IN   HIGH = pelaajan 2 ohjainta ei käytetty vähään aikaan (Arduino Leonardo 2)
#define PIN_SEND_IDLE       7       // OUT  HIGH = näytönsäästäjä päälle (Arduino Mega)
#define PIN_RELAY_ON        8       // OUT  LOW  = kaiuttimien kytkentärele päällä
#define PIN_OUT_ENABLE      A0      // OUT  LOW  = nappien valot päälle (74HC595 Output Enable pinni maihin)
#define PIN_RASPI_POWER_ON  A5      // IN   HIGH = Raspberry Pi on päällä (Arduino Leonardo 1)

#define PIN_LED1            9       // OUT  HIGH = ledinauha 1 päällä
#define PIN_LED2            10      // OUT  HIGH = ledinauha 2 päällä
#define PIN_LED3            11      // OUT  HIGH = ledinauha 3 päällä

#define SIDE1               0
#define L1                  1
#define X1                  2
#define R1                  3
#define Y1                  4
#define B1                  5
#define A1                  6

#define L2                  7
#define X2                  8
#define R2                  9
#define Y2                  10
#define B2                  11
#define A2                  12
#define SIDE2               13

#define TOP1                14
#define COIN1               15
#define PLAYER1             16
#define START1              17
#define SELECT1             18

#define TOP2                19
#define COIN2               20
#define PLAYER2             21
#define START2              22
#define SELECT2             23

unsigned long lastBlinkTime = 0;
unsigned long lastRaspiPowerOff = 0;
unsigned long lastRaspiPowerOn = 0;
uint8_t ledStripCount = 0;
bool ledStripDir = 0;
uint8_t ledCounter = 0;
uint8_t ledMode = 0;
uint32_t matrix = 0x0;

uint16_t lTop = 0;
uint16_t lMiddle = 0;
uint16_t lBottom = 0;
uint16_t lSide = 0;

bool ledsOn = false;

void setup()
{
    pinMode(PIN_OUT_ENABLE, OUTPUT);
    pinMode(PIN_RELAY_ON, OUTPUT);
    
    digitalWrite(PIN_RELAY_ON, HIGH);
    digitalWrite(PIN_OUT_ENABLE, HIGH);

    pinMode(PIN_RASPI_POWER_ON, INPUT);
    
    pinMode(PIN_DATA, OUTPUT);
    pinMode(PIN_LATCH, OUTPUT);
    pinMode(PIN_CLOCK, OUTPUT);
    
    pinMode(PIN_IDLE1, INPUT);
    pinMode(PIN_IDLE2, INPUT);
    pinMode(PIN_SEND_IDLE, OUTPUT);
    
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LED3, OUTPUT);

    clearLeds();
    digitalWrite(PIN_OUT_ENABLE, LOW);
    initialize();
    
    //delay(SPEAKER_DELAY);
    //digitalWrite(PIN_RELAY_ON, LOW);
}

void loop()
{
    if (digitalRead(PIN_RASPI_POWER_ON))
    {
        if (lastRaspiPowerOn == 0) lastRaspiPowerOn = millis();
        lastRaspiPowerOff = 0;
    }
    else
    {
        if (lastRaspiPowerOff == 0) lastRaspiPowerOff = millis();
        lastRaspiPowerOn = 0;
    }

    if (lastRaspiPowerOff > 0 && lastRaspiPowerOff + RELAY_DELAY < millis()) digitalWrite(PIN_RELAY_ON, HIGH);
    else if (lastRaspiPowerOn > 0 && lastRaspiPowerOn + RELAY_DELAY < millis()) digitalWrite(PIN_RELAY_ON, LOW);

    //if (!digitalRead(PIN_RASPI_POWER_ON) && lastRaspiPowerOn) digitalWrite(PIN_RELAY_ON, HIGH);
    //else digitalWrite(PIN_RELAY_ON, LOW);

    if (digitalRead(PIN_IDLE1) && digitalRead(PIN_IDLE2))
    {
        if (lastBlinkTime + BLINK_DELAY < millis()) blinkLeds();
        digitalWrite(PIN_SEND_IDLE, HIGH);
    }
    
    else 
    {
        setLeds();
        digitalWrite(PIN_SEND_IDLE, LOW);
    }
}

void initialize()
{
    for (int i = 0; i < 6; i++)
    {
        s(TOP1); s(TOP2); s(SIDE1); s(SIDE2); out595(matrix); delay(BLINK_DELAY);
        c(TOP1); c(TOP2); c(SIDE1); c(SIDE2); out595(matrix); delay(BLINK_DELAY);
    }
    
    s(TOP1); out595(matrix); delay(INIT_DELAY);
    s(COIN1); out595(matrix); delay(INIT_DELAY);
    s(PLAYER1); out595(matrix); delay(INIT_DELAY);
    s(START1); out595(matrix); delay(INIT_DELAY);
    s(SELECT1); out595(matrix); delay(INIT_DELAY);
    s(SIDE1); out595(matrix); delay(INIT_DELAY);
    s(L1); out595(matrix); delay(INIT_DELAY);
    s(Y1); out595(matrix); delay(INIT_DELAY);
    s(B1); out595(matrix); delay(INIT_DELAY);
    s(X1); out595(matrix); delay(INIT_DELAY);
    s(R1); out595(matrix); delay(INIT_DELAY);
    s(A1); out595(matrix); delay(INIT_DELAY);
    s(Y2); out595(matrix); delay(INIT_DELAY);
    s(L2); out595(matrix); delay(INIT_DELAY);
    s(X2); out595(matrix); delay(INIT_DELAY);
    s(B2); out595(matrix); delay(INIT_DELAY);
    s(A2); out595(matrix); delay(INIT_DELAY);
    s(R2); out595(matrix); delay(INIT_DELAY);
    s(SIDE2); out595(matrix); delay(INIT_DELAY);
    s(SELECT2); out595(matrix); delay(INIT_DELAY);
    s(START2); out595(matrix); delay(INIT_DELAY);
    s(PLAYER2); out595(matrix); delay(INIT_DELAY);
    s(COIN2); out595(matrix); delay(INIT_DELAY);
    s(TOP2); out595(matrix); delay(INIT_DELAY);
}

void blinkLeds()
{
    ledTop();
    ledMiddle();
    ledBottom();
    ledSide();
    ledStrip();

    out595(matrix);
    lastBlinkTime = millis();
}

void setLeds()
{
    out595(0xFFFFFF);
    digitalWrite(PIN_LED1, LOW);
    digitalWrite(PIN_LED2, LOW);
    digitalWrite(PIN_LED3, LOW);
    ledsOn = true;
}

void clearLeds()
{
    out595(0x0);
    digitalWrite(PIN_LED1, HIGH);
    digitalWrite(PIN_LED2, HIGH);
    digitalWrite(PIN_LED3, HIGH);
    ledsOn = false;
}

void out595(uint32_t data) {
    digitalWrite(PIN_CLOCK, LOW);
    digitalWrite(PIN_LATCH, LOW);
    
    for (int i = 0; i < 24; i++) {
        digitalWrite(PIN_DATA, (data >> (23 - i)) & 1);
        digitalWrite(PIN_CLOCK, HIGH);
        digitalWrite(PIN_CLOCK, LOW);
    }
    
    digitalWrite(PIN_LATCH, HIGH);
}

void s(uint8_t i)
{
    if (i < 24) matrix ^= (-1 ^ matrix) & (1L << (23 - i));
}

void c(uint8_t i)
{
    if (i < 24) matrix ^= (-0 ^ matrix) & (1L << (23 - i));
}

// **********************************************

void ledTop()
{
    c(TOP1);
    c(TOP2);
}

void ledMiddle()
{
    if (lMiddle == 0)
    {
        c(COIN1);
        c(PLAYER1);
        c(START1);
        c(SELECT1);

        c(COIN2);
        c(PLAYER2);
        c(START2);
        c(SELECT2);
    }

    else if (lMiddle == 2)
    {
        s(COIN1);
        s(COIN2);
    }

    else if (lMiddle == 4)
    {
        s(PLAYER1);
        s(PLAYER2);
    }

    else if (lMiddle == 6)
    {
        s(START1);
        s(START2);
    }

    else if (lMiddle == 8)
    {
        s(SELECT1);
        s(SELECT2);
    }

    if (lMiddle == 10)
    {
        c(COIN1);
        c(PLAYER1);
        c(START1);
        c(SELECT1);

        c(COIN2);
        c(PLAYER2);
        c(START2);
        c(SELECT2);
    }

    else if (lMiddle == 12)
    {
        s(COIN1);
        s(PLAYER1);
        s(START1);
        s(SELECT1);

        s(COIN2);
        s(PLAYER2);
        s(START2);
        s(SELECT2);
    }


    else if (lMiddle == 14)
    {
        c(COIN1);
        c(PLAYER1);
        c(START1);
        c(SELECT1);

        c(COIN2);
        c(PLAYER2);
        c(START2);
        c(SELECT2);
    }

    else if (lMiddle == 16)
    {
        s(SELECT1);
        s(SELECT2);
    }

    else if (lMiddle == 18)
    {
        s(START1);
        s(START2);
    }

    else if (lMiddle == 20)
    {
        s(PLAYER1);
        s(PLAYER2);
    }

    else if (lMiddle == 22)
    {
        s(COIN1);
        s(COIN2);
    }

    else if (lMiddle == 24)
    {
        c(COIN1);
        c(PLAYER1);
        c(START1);
        c(SELECT1);

        c(COIN2);
        c(PLAYER2);
        c(START2);
        c(SELECT2);
    }

    else if (lMiddle == 26)
    {
        s(COIN1);
        s(PLAYER1);
        s(START1);
        s(SELECT1);

        s(COIN2);
        s(PLAYER2);
        s(START2);
        s(SELECT2);
    }

    lMiddle++;
    if (lMiddle == 28) lMiddle = 0;
}

void ledBottom()
{
    if (lBottom == 0)
    {
        s(L1); c(X1); c(R1);
        c(Y1); c(B1); c(A1);

        c(L2); c(X2); s(R2);
        c(Y2); c(B2); c(A2);

        lBottom++;
    }

    else if (lBottom == 1)
    {
        c(L1); s(X1); c(R1);
        c(Y1); c(B1); c(A1);
        
        c(L2); s(X2); c(R2);
        c(Y2); c(B2); c(A2);

        lBottom++;
    }

    else if (lBottom == 2)
    {
        c(L1); c(X1); s(R1);
        c(Y1); c(B1); c(A1);
        
        s(L2); c(X2); c(R2);
        c(Y2); c(B2); c(A2);

        lBottom++;
    }

    else if (lBottom == 3)
    {
        c(L1); c(X1); c(R1);
        c(Y1); c(B1); s(A1);
        
        c(L2); c(X2); c(R2);
        s(Y2); c(B2); c(A2);

        lBottom++;
    }

    else if (lBottom == 4)
    {
        c(L1); c(X1); c(R1);
        c(Y1); s(B1); c(A1);
        
        c(L2); c(X2); c(R2);
        c(Y2); s(B2); c(A2);

        lBottom++;
    }

    else if (lBottom == 5)
    {
        c(L1); c(X1); c(R1);
        s(Y1); c(B1); c(A1);
        
        c(L2); c(X2); c(R2);
        c(Y2); c(B2); s(A2);

        lBottom = 0;
    }
}

void ledSide()
{
    if (lSide == 0)
    {
        s(SIDE1);
        c(SIDE2);
    }

    else if (lSide == 3)
    {
        c(SIDE1);
        s(SIDE2);
    }

    lSide++;
    if (lSide == 6) lSide = 0;
}

void ledStrip()
{
    ledStripCount++;

    if (ledStripCount == 6) ledStripCount = 0;

    if (ledStripCount == 0)
    {
        digitalWrite(PIN_LED1, LOW);
        digitalWrite(PIN_LED2, HIGH);
        digitalWrite(PIN_LED3, HIGH);
    }

    else if (ledStripCount == 2)
    {
        digitalWrite(PIN_LED1, HIGH);
        digitalWrite(PIN_LED2, LOW);
        digitalWrite(PIN_LED3, HIGH);
    }

    else if (ledStripCount == 4)
    {
        digitalWrite(PIN_LED1, HIGH);
        digitalWrite(PIN_LED2, HIGH);
        digitalWrite(PIN_LED3, LOW);
    }
}
