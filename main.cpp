#include "DigitalOut.h"
#include "mbed.h"


/*** PIN DEFINITIONS ***/
// Nucleo On-Board
#define LED_NUCLEO  PA_5
#define SW_NUCLEO   PC_13
// Velleman STEM Shield I/O
#define LED_RED1    PB_0
#define LED_ORANGE1 PC_1
#define LED_GREEN1  PA_10
#define LED_RED2    PB_3
#define LED_ORANGE2 PB_5
#define LED_GREEN2  PB_4
#define SW_1    PA_9
#define SW_2    PC_7
#define SW_3    PB_6
#define SW_4    PA_7
#define SW_5    PA_6
// wait function for mbed-os-6
#define WAIT(t) ThisThread::sleep_for(chrono::milliseconds(t))


// led bus define
BusOut busLeds(LED_RED1, LED_ORANGE1, LED_GREEN1, LED_GREEN2, LED_ORANGE2, LED_RED2);
DigitalOut led(LED_RED1);
// button inputs
DigitalIn buttonDir(SW_1);
DigitalIn buttonDot(SW_2);
DigitalIn buttonBar(SW_3);
DigitalIn buttonFast(SW_4);
DigitalIn buttonSlow(SW_5);

// timer
Ticker patternTicker;

// led patterns
const uint8_t patternDot[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20};
const uint8_t patternBar[] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3E, 0x3C, 0x38, 0x30, 0x20};
// pattern info
//   gets changed by button input
uint8_t patternNum;     // which pattern to show
uint8_t patternDir;     // pattern direction
int8_t patternStep;     // pattern index variable
uint16_t patternSpeed;  // delay for each step in ms


void pattern_next() {
    // check which pattern to show
    if(patternNum == 0) {
        // pattern: blink all
        patternStep = 0;
        busLeds = ~busLeds;
    } else if(patternNum == 1) {
        // pattern: dot
        // check if step at the end of pattern
        if(patternStep < 0) patternStep = sizeof(patternDot) - 1;
        if(patternStep >= sizeof(patternDot)) patternStep = 0;
        // set leds to pattern index
        busLeds = patternDot[patternStep];
    } else if(patternNum == 2) {
        // pattern: bar
        // check if step at the end of pattern
        if(patternStep < 0) patternStep = sizeof(patternBar) - 1;
        if(patternStep >= sizeof(patternBar)) patternStep = 0;
        // set leds to pattern index
        busLeds = patternBar[patternStep];
    }
    // next step
    if(patternDir)
        patternStep--;
    else
        patternStep++;
}


void toggle() {
    led = !led;
}

int main() {    
    // Initialise global vars
    patternDir = 0;
    patternNum = 0;
    patternStep = 0;

    // set timer interrupt
    patternTicker.attach(&toggle, 300ms);

    // main loop
    while (true) {
        // set direction by slide switch
        patternDir = buttonDir.read();
        // check pattern button
        if(buttonDot.read()) patternNum = 1;
        else if(buttonBar.read()) patternNum = 2;
        else if(buttonFast.read()) {}
        else if(buttonSlow.read()) {}
        // debounce delay
        WAIT(20);
    }
}
