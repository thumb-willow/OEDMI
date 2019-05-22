#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
const int XP = 8, XM = A2, YP = A3, YM = 9; //240x320 ID=0x7575
const int TS_LEFT = 949, TS_RT = 190, TS_TOP = 523, TS_BOT = 398;

#include <MIDIUSB.h>
#include <MIDI.h>
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define MINPRESSURE 200
#define MAXPRESSURE 1000

// Assign human-readable names to some common 16-bit color values:
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GRAY 0x8410

uint16_t version = MCUFRIEND_KBV_H_;

void setup()
{
    Serial.begin(9600);
    if (!Serial)
        delay(5000);            //allow some time for Leonardo
    uint16_t ID = tft.readID(); //
    Serial.println(F("Initializing OEDMI"));

    tft.begin(ID);
    Serial.println(F("Display Initialized"));
    tft.setRotation(0);
    tft.fillScreen(BLACK);
    tft.setTextSize(1);
    tft.setTextColor(WHITE);
    tft.setCursor(0, 0);
    tft.println(F("Booting up the saucy drums..."));
}

void loop()
{
    //PORTRAIT CALIBRATION     240 x 320
    uint16_t xpos, ypos, pressure; //screen coordinates
    tp = ts.getPoint();            //tp.x, tp.y are ADC values
    xpos = map(tp.x, TS_LEFT, TS_RT, 0, 240);
    ypos = map(tp.y, TS_TOP, TS_BOT, 0, 320);
    pressure = tp.z;

    // if sharing pins, you'll need to fix the directions of the touchscreen pins
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    tft.println("tp.x:" + String(tp.x) + " tp.y:" + String(tp.y) + " tp.z:" + String(tp.z));
}