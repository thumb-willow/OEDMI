#include <MCUFRIEND_kbv.h>
#include <MIDI.h>
#include <TouchScreen.h>
#include <math.h>

MCUFRIEND_kbv tft;
// const int XP = 8, XM = A2, YP = A3, YM = 9; //240x320 ID=0x7575
const int TS_LEFT = 647, TS_RT = 516, TS_TOP = 642, TS_BOT = 503;
TouchScreen ts = TouchScreen(8, A3, A2, 9, 300);
TSPoint tsPoint;
bool isTouched = false;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

#define MINPRESSURE 300
#define MAXPRESSURE 1000
#define PIEZO_THRESHHOLD 100

int muxSelectPins[] = {9, 10, 11, 12};
int piezoPins[]{A14, A15};

/* Standard MIDI drum tones
35  Acoustic Bass Drum        59  Ride Cymbal 2    
36  Bass Drum 1               60  Hi Bongo    
37  Side Stick                61  Low Bongo    
38  Acoustic Snare            62  Mute Hi Conga    
39  Hand Clap                 63  Open Hi Conga    
40  Electric Snare            64  Low Conga    
41  Low Floor Tom             65  High Timbale    
42  Closed Hi Hat             66  Low Timbale    
43  High Floor Tom            67  High Agogo    
44  Pedal Hi-Hat              68  Low Agogo    
45  Low Tom                   69  Cabasa    
46  Open Hi-Hat               70  Maracas    
47  Low-Mid Tom               71  Short Whistle    
48  Hi-Mid Tom                72  Long Whistle    
49  Crash Cymbal 1            73  Short Guiro    
50  High Tom                  74  Long Guiro    
51  Ride Cymbal 1             75  Claves    
52  Chinese Cymbal            76  Hi Wood Block    
53  Ride Bell                 77  Low Wood Block    
54  Tambourine                78  Mute Cuica    
55  Splash Cymbal             79  Open Cuica    
56  CowBell                   80  Mute Triangle    
57  Crash Cymbal 2            81  Open Triangle
58  Vibraslap                 
*/

// Physical channels on the controller
int trigger2MidiToneMap[] = {
    //Channel 1.1 Snare
    38,
    //Channel 1.2 Snare Rim
    40,
    //Channel 2.1 Tom 1
    48,
    //Channel 2.2 Tom 1 Rim
    50,
    //Channel 3.1 Tom 2
    45,
    //Channel 3.2 Tom 2 RIM
    47,
    //Channel 4.1 Tom 3 HEAD
    43,
    //Channel 4.2 Tom 3 RIM
    58,
    //Channel 5.1 High Hat Bow
    46,
    //Channel 5.2 High Hat Edge
    26,
    //Channel 6.1 Crash 1 Bow
    49,
    //Channel 6.2 Crash 1 Edge
    55,
    //Channel 7.1 Crash 2 Bow
    57,
    //Channel 7.2 Crash 2 Edge
    52,
    //Channel 8.1 Ride Edge
    59,
    //Channel 9 Ride Bell
    53,
    //Channel 10 Kick 1
    0,
    //Channel 11 Kick 2
    0,
    //Channel 12 Floor Tom 1
    0,
    //Channel 13 Floor Tom 2
    0,
    // Channel 14 Aux 1
    0,
    // Channel 15 Aux 2
    0,
    // Channel 16 Aux 3
    0,
    // Channel 17 Aux 4
    0,
    // Channel 18 Aux 5
    0,
    // Channel 19 Aux 6
    0,
    // Channel 20 Aux 7
    0,
    // Channel 21 Aux 8
    0,
    // Channel 22 Aux 9
    0,
    // Channel 23 Aux 10
    0,
    // Channel 24 Aux 11
    0,
    // Channel 25 Aux 12
    0,
    // Channel 26 Aux 13
    0,
    // Channel 27 Aux 14
    0,
    // Channel 28 Aux 15
    0,

};

void noteOn(byte channel, byte pitch, byte velocity)
{
    MIDI.sendNoteOn(pitch, velocity, channel);
}

void noteOff(byte channel, byte pitch, byte velocity)
{
    MIDI.sendNoteOff(pitch, velocity, channel);
}

void setup()
{
    Serial.begin(9600);
    if (!Serial)
        delay(5000); //allow some time for Leonardo

    Serial.println(F("Initializing OEDMI"));
    tft.begin(tft.readID());
    Serial.println(F("Display Initialized"));

    tft.setRotation(0);
    tft.fillScreen(0x0000);
    tft.setTextSize(2);
    tft.setTextColor(0xFFFF);
    tft.setCursor(0, 0);
    tft.println(F("Booting up the saucy drums..."));
    Serial.println(F("Setting up shift register"));

    pinMode(muxSelectPins[0], OUTPUT);
    pinMode(muxSelectPins[1], OUTPUT);
    pinMode(muxSelectPins[2], OUTPUT);
    pinMode(muxSelectPins[3], OUTPUT);
    digitalWrite(muxSelectPins[0], LOW);
    digitalWrite(muxSelectPins[1], LOW);
    digitalWrite(muxSelectPins[2], LOW);
    digitalWrite(muxSelectPins[3], LOW);

    Serial.println(F("Starting MIDI"));
    MIDI.begin();

    Serial.println(F("Ready"));
}

void loop()
{

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            digitalWrite(muxSelectPins[j], bitRead(i, j));
        } //read the value at the SIG pin

        int piezo1 = analogRead(piezoPins[0]);
        int piezo2 = analogRead(piezoPins[1]);

        // Channel 0 - 15
        if (piezo1 >= PIEZO_THRESHHOLD)
        {
            noteOn(0, trigger2MidiToneMap[i], map(piezo1, 100, 1034, 64, 127));
            Serial.println("P1: " + String(piezo1));
        }
        else
        {
            noteOff(0, trigger2MidiToneMap[i + 16], 64);
        }

        // Channel 16 - 27
        if (piezo2 >= PIEZO_THRESHHOLD)
        {
            // Kick Drum
            noteOn(0, trigger2MidiToneMap[i + 16], map(piezo2, 100, 1034, 64, 127));
            Serial.println("P2: " + String(piezo2));
        }
        else
        {
            noteOff(0, trigger2MidiToneMap[i + 16], 64);
        }
        MIDI.read();
    }
}