#define DECODE_MODE             0x9
#define INTENSITY               0xA
#define SCAN_LIMIT              0xB
#define SHUTDOWN                0xC
#define DISPLAY_TEST            0xF

uint8_t PIN_DATA[3];            // DATA -pinnit
uint8_t PIN_LOAD[3];            // LOAD -pinnit
uint8_t PIN_CLOCK[3];           // CLOCK -pinnit

volatile uint8_t *PORT[3];      // Port Register
volatile uint8_t *DDR[3];       // Data Direction Register

void max7219_Init()
{
    PORT[0] = &PORTE;           // Ensimmäisen näyttörivin Port
    PORT[1] = &PORTH;           // Toisen
    PORT[2] = &PORTB;           // Kolmannen

    DDR[0] = &DDRE;             // Ensimmäisen näyttörivin Data Direction Register
    DDR[1] = &DDRH;             // Toisen
    DDR[2] = &DDRB;             // Kolmannen

    PIN_DATA[0] = 4;            // Ensimmäisen näyttörivin DATA, LOAD ja CLOCK -pinnit (pinnin numero vastaa valitun portin pinniä (0-7), ei Arduinon pinniä)
    PIN_LOAD[0] = 5;
    PIN_CLOCK[0] = 3;
    
    PIN_DATA[1] = 3;            // Toisen
    PIN_LOAD[1] = 4;
    PIN_CLOCK[1] = 5;
    
    PIN_DATA[2] = 4;            // Kolmannen
    PIN_LOAD[2] = 5;
    PIN_CLOCK[2] = 6;

    // 1. näyttö: DIN = D2,  LOAD = D3,  CLOCK = D5
    // 2. näyttö: DIN = D6,  LOAD = D7,  CLOCK = D8
    // 3. näyttö: DIN = D10, LOAD = D11, CLOCK = D12

    for (uint8_t row = 0; row < 3; row++)
    {
        *(DDR[row]) |= (1 << PIN_DATA[row]) | (1 << PIN_LOAD[row]) | (1 << PIN_CLOCK[row]);     // DATA, LOAD ja CLOCK -pinnit inputeiksi
        *(PORT[row]) &= ~(1 << PIN_DATA[row]);                                                  // DATA -pinni alas
        *(PORT[row]) |= (1 << PIN_LOAD[row]) | (1 << PIN_CLOCK[row]);                           // LOAD ja CLOCK -pinnit ylös
    }

    max7219_All(DECODE_MODE, 0x0);                      // Numerojen koodaus pois päältä; voidaan sytyttää halutut pikselit eikä vain 7-segment näytön numeroita
    max7219_All(INTENSITY, 0x0);                        // Näytön kirkkaus 0 (0-15)
    max7219_All(SCAN_LIMIT, 0x7);                       // Otetaan käyttöön koko 64 pikselin ohjaus (sama kuin 8kpl 7-segment näyttöjä)
    max7219_All(SHUTDOWN, 0x1);                         // Näyttö päälle (0 = pois päältä)
    max7219_All(DISPLAY_TEST, 0x0);                     // Näytön testi (kaikki pikselit palaa) pois päältä
}

void max7219_All(uint8_t address, uint8_t data)
{
    for (int row = 0; row < 3; row++)
    {
        max7219_BeginUpdate(row);
        for (int col = 0; col < 8; col++) max7219_Data(address, data, row);
        max7219_EndUpdate(row);
    }
}

void max7219_BeginUpdate(uint8_t row)
{
    *(PORT[row]) &= ~(1 << PIN_LOAD[row]);                                         // LOAD -pinni alas; otetaan vastaan dataa
}

void max7219_EndUpdate(uint8_t row)
{
    *(PORT[row]) |= (1 << PIN_LOAD[row]);                                          // LOAD -pinni ylös; otetaan data käyttöön
}

void max7219_Data(uint8_t address, uint8_t data, uint8_t row)
{
    for (int i = 0; i < 8; i++)
    {
        *(PORT[row]) &= ~(1 << PIN_CLOCK[row]);                                    // CLOCK -pinni alas; otetaan vastaan bitti
        if ((address >> (7 - i)) & 1) *(PORT[row]) |= (1 << PIN_DATA[row]);        // Jos bitti on 1
        else *(PORT[row]) &= ~(1 << PIN_DATA[row]);                                // Jos bitti on 0
        *(PORT[row]) |= (1 << PIN_CLOCK[row]);                                     // CLOCK -pinni ylös; bitti otettu vastaan
    }

    for (int i = 0; i < 8; i++)
    {
        *(PORT[row]) &= ~(1 << PIN_CLOCK[row]);
        if ((data >> (7 - i)) & 1) *(PORT[row]) |= (1 << PIN_DATA[row]);
        else *(PORT[row]) &= ~(1 << PIN_DATA[row]);
        *(PORT[row]) |= (1 << PIN_CLOCK[row]);
    }
}
