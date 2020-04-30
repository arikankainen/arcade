#include <EEPROM.h>
#include <avr/pgmspace.h>

#define MATRIX_X                8
#define MATRIX_Y                24
#define PIXELS_X                64
#define PIXELS_Y                24

#define KEY_UP                  A5
#define KEY_DOWN                A4
#define KEY_LEFT                A3
#define KEY_RIGHT               A2
#define KEY_BUTTON1             A1
#define KEY_BUTTON2             A0

#define RANDOM_SEED             A15

#define INTERRUPT_KEY1          20
#define INTERRUPT_KEY2          21
#define INTERRUPT_BOUNCE_DELAY  200

#define PIN_DISABLE_KEYS        13
#define PIN_DEMO_ANIMATION      19

bool keyUp = false;
bool keyRight = false;
bool keyDown = false;
bool keyLeft = false;
bool keyButton1 = false;
bool keyButton2 = false;

volatile bool idleMode = true;
volatile int testCount = 0;
volatile unsigned long lastInterrupt = 0;
volatile bool quit = false;

uint8_t matrix[MATRIX_X][MATRIX_Y];
uint8_t matrixBuffer1[MATRIX_X][MATRIX_Y];

String stringConsole = "";
String stringGame = "";

int test = 0;

bool serialReceived = false;
bool gameMode = false;
bool gameNamePrinted = false;
bool startup = true;

void setup() 
{
    Serial.begin(9600);
    
    pinMode(KEY_UP, INPUT_PULLUP);
    pinMode(KEY_LEFT, INPUT_PULLUP);
    pinMode(KEY_DOWN, INPUT_PULLUP);
    pinMode(KEY_RIGHT, INPUT_PULLUP);
    pinMode(KEY_BUTTON1, INPUT_PULLUP);
    pinMode(KEY_BUTTON2, INPUT_PULLUP);

    pinMode(PIN_DISABLE_KEYS, OUTPUT);
    pinMode(PIN_DEMO_ANIMATION, INPUT);

    pinMode(INTERRUPT_KEY1, INPUT_PULLUP);
    pinMode(INTERRUPT_KEY2, INPUT_PULLUP);
    
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_KEY1), isr_key1, FALLING);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_KEY2), isr_key2, FALLING);
    attachInterrupt(digitalPinToInterrupt(PIN_DEMO_ANIMATION), isr_demo, CHANGE);
    
    randomSeed(analogRead(RANDOM_SEED));

    digitalWrite(PIN_DISABLE_KEYS, LOW);
    
    max7219_Init();
    clearPixels(matrixBuffer1, false);
    clearPixels(matrix, true);
    initialize();
}   

void loop()
{
    if (!idleMode) karmes_MainMenu();
    else arcade();
    //arcade_test();
}

void readKeys()
{
    if (!digitalRead(KEY_UP)) keyUp = true;
    if (!digitalRead(KEY_RIGHT)) keyRight = true;
    if (!digitalRead(KEY_DOWN)) keyDown = true;
    if (!digitalRead(KEY_LEFT)) keyLeft = true;
    if (!digitalRead(KEY_BUTTON1)) keyButton1 = true;
    if (!digitalRead(KEY_BUTTON2)) keyButton2 = true;
}

void resetKeys()
{
    keyUp = keyRight = keyDown = keyLeft = keyButton1 = keyButton2 = false;
}


void d(int del)
{
    if (quit) return;
    
    for (int i = 0; i < del; i++)
    {
        if (quit) break;
        delay(1);
    }
}

void isr_key1()
{
    if (!digitalRead(INTERRUPT_KEY1) && idleMode && !digitalRead(PIN_DEMO_ANIMATION))
    {
        idleMode = false;
        digitalWrite(PIN_DISABLE_KEYS, HIGH);
    }
}

void isr_key2()
{
    // nappi2 ei ole käytössä    
}

void isr_demo()
{
    if (!startup)
    {
        if (!digitalRead(PIN_DEMO_ANIMATION)) quit = true;
        else quit = false;
    }
}

void serialEvent() {
    while (Serial.available())
    {
        String ser = Serial.readString();
        String console, game = "";
        bool menu = false;
        
        int console_begin = ser.indexOf("<console>");
        int console_end = ser.indexOf("</console>");
        int game_begin = ser.indexOf("<game>");
        int game_end = ser.indexOf("</game>");
        int menu_begin = ser.indexOf("<menu>");

        if (console_begin > -1) console = ser.substring(console_begin + 9, console_end);
        if (game_begin > -1) game = ser.substring(game_begin + 6, game_end);
        if (menu_begin > -1) menu = true;

        if (menu)
        {
            stringConsole = stringGame = "";
            gameMode = false;
        }
        else
        {
            stringConsole = console;
            stringGame = game;
            stringGame.replace("&amp;", "&");
            gameMode = true;
            gameNamePrinted = false;
        }

        serialReceived = true;
    }
}
