#include <Keyboard.h>

#define IDLE_DELAY              120000
#define BOUNCE_DELAY            30

#define PIN_RASPI_POWER_ON      10
#define PIN_SEND_IDLE           11
#define PIN_DISABLE_KEYBOARD    12

char keyChar[16];
uint8_t keyPin[16];
bool keyPressed[16];
unsigned long bounceTime[16] = { 0 };
bool boot = true;
unsigned long lastIdleTime = 0;

void setup()
{
    keyPin[0] = A5;     // joystick up
    keyPin[1] = A4;     // joystick down
    keyPin[2] = A3;     // joystick left
    keyPin[3] = A2;     // joystick right
    keyPin[4] = A1;     // L
    keyPin[5] = A0;     // X
    keyPin[6] = 0;      // R
    keyPin[7] = 1;      // Y
    keyPin[8] = 2;      // B
    keyPin[9] = 3;      // A
    keyPin[10] = 4;     // flipper
    keyPin[11] = 5;     // coin
    keyPin[12] = 6;     // player
    keyPin[13] = 7;     // start
    keyPin[14] = 8;     // select
    keyPin[15] = 9;     // coin acceptor

    keyChar[0] = KEY_UP_ARROW;
    keyChar[1] = KEY_DOWN_ARROW;
    keyChar[2] = KEY_LEFT_ARROW;
    keyChar[3] = KEY_RIGHT_ARROW;
    keyChar[4] = '3';
    keyChar[5] = ',';
    keyChar[6] = '4';
    keyChar[7] = '.';
    keyChar[8] = KEY_ESC;
    keyChar[9] = KEY_RETURN;
    keyChar[10] = KEY_LEFT_SHIFT;
    keyChar[11] = KEY_F1;
    keyChar[12] = KEY_F2;
    keyChar[13] = KEY_F9;
    keyChar[14] = KEY_F10;
    keyChar[15] = KEY_F1;

    Keyboard.begin();

    for (uint8_t i = 0; i < 15; i++) pinMode(keyPin[i], INPUT_PULLUP);
    pinMode(keyPin[15], INPUT);
    pinMode(PIN_DISABLE_KEYBOARD, INPUT);
    pinMode(PIN_SEND_IDLE, OUTPUT);
    pinMode(PIN_RASPI_POWER_ON, OUTPUT);
    digitalWrite(PIN_RASPI_POWER_ON, HIGH);
}

void loop()
{
    readKeys();
    if (boot || lastIdleTime + IDLE_DELAY < millis()) startIdle();
}

void readKeys()
{
    bool disableKeyboard = false;
    if (digitalRead(PIN_DISABLE_KEYBOARD))
    {
        disableKeyboard = true;
        keyPress();
    }
    
    for (uint8_t i = 0; i < 15; i++)
    {
        if (!digitalRead(keyPin[i]) && !keyPressed[i] && bounceTime[i] + BOUNCE_DELAY < millis())
        {
            if (!disableKeyboard) Keyboard.press(keyChar[i]);
            keyPressed[i] = true;
            bounceTime[i] = millis();
            keyPress();
        }
        
        if (digitalRead(keyPin[i]) && keyPressed[i] && bounceTime[i] + BOUNCE_DELAY < millis())
        {
            if (!disableKeyboard) Keyboard.release(keyChar[i]);
            keyPressed[i] = false;
            bounceTime[i] = millis();
            keyRelease();
        }
    }

    uint8_t i = 15; // coin acceptor
    if (digitalRead(keyPin[i]) && !keyPressed[i] && bounceTime[i] + BOUNCE_DELAY < millis())
    {
        if (!disableKeyboard) Keyboard.press(keyChar[i]);
        keyPressed[i] = true;
        bounceTime[i] = millis();
        keyPress();
    }
    
    if (!digitalRead(keyPin[i]) && keyPressed[i] && bounceTime[i] + BOUNCE_DELAY < millis())
    {
        if (!disableKeyboard) Keyboard.release(keyChar[i]);
        keyPressed[i] = false;
        bounceTime[i] = millis();
        keyRelease();
    }
}

void keyPress() 
{
    lastIdleTime = millis();
    stopIdle();
}

void keyRelease() 
{
  
}

void stopIdle()
{
    digitalWrite(PIN_SEND_IDLE, LOW);
}

void startIdle()
{
    boot = false;
    digitalWrite(PIN_SEND_IDLE, HIGH);
}
