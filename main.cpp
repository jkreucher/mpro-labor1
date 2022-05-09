#include "DigitalOut.h"
#include "ThisThread.h"
#include "mbed.h"
#include <chrono>
#include <cstdint>


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


// led bus define
//BusOut busLeds(LED_RED1, LED_ORANGE1, LED_GREEN1, LED_GREEN2, LED_ORANGE2, LED_RED2);
DigitalOut ledRed1(LED_RED1);
DigitalOut ledOrange1(LED_ORANGE1);
DigitalOut ledGreen1(LED_GREEN1);
DigitalOut ledRed2(LED_RED2);
DigitalOut ledOrange2(LED_ORANGE2);
DigitalOut ledGreen2(LED_GREEN2);
// button inputs
DigitalIn buttonDir(SW_1);
DigitalIn buttonDot(SW_4);
DigitalIn buttonBar(SW_5);
DigitalIn buttonFast(SW_2);
DigitalIn buttonSlow(SW_3);

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
uint16_t patternSpeed;


// this function creates a bus of leds
//   Using BusOut crashes mbed-os when using inside in isr
void set_leds(uint8_t data) {
    ledRed1    = data & 0x01;
    ledOrange1 = (data >> 1) & 0x01;
    ledGreen1  = (data >> 2) & 0x01;
    ledGreen2  = (data >> 3) & 0x01;
    ledOrange2 = (data >> 4) & 0x01;
    ledRed2    = (data >> 5) & 0x01;
}


// this function advances to the next index in a pattern and displays it
void pattern_next() {
    // check which pattern to show
    if(patternNum == 0) {
        // pattern: blink all
        patternStep = 0;
        set_leds(0xFF);
    } else if(patternNum == 1) {
        // pattern: dot
        // check if step at the end of pattern
        if(patternStep < 0) patternStep = sizeof(patternDot) - 1;
        if(patternStep >= sizeof(patternDot)) patternStep = 0;
        // set leds to pattern index
        set_leds(patternDot[patternStep]);
    } else if(patternNum == 2) {
        // pattern: bar
        // check if step at the end of pattern
        if(patternStep < 0) patternStep = sizeof(patternBar) - 1;
        if(patternStep >= sizeof(patternBar)) patternStep = 0;
        // set leds to pattern index
        set_leds(patternBar[patternStep]);
    }
    // next step
    if(patternDir)
        patternStep--;
    else
        patternStep++;
}


int main() {    
    // Initialise global vars
    patternDir = 0;
    patternNum = 1;
    patternStep = 0;
    patternSpeed = 300;

    // start timer
    patternTicker.attach(&pattern_next, chrono::milliseconds(patternSpeed));

    // main loop
    while (true) {
        // set direction by slide switch
        patternDir = buttonDir.read();
        // check pattern button
        if(buttonDot.read() == 1) {
            patternNum = 1;
        }
        if(buttonBar.read() == 1) {
            patternNum = 2;
        }
        if((buttonFast.read() == 1) && (patternSpeed != 50)) {
            patternSpeed = 50;
            // update timer speed
            patternTicker.attach(&pattern_next, chrono::milliseconds(patternSpeed));
        }
        if((buttonSlow.read() == 1) && (patternSpeed != 2000)) {
            patternSpeed = 2000;
            // update timer speed
            patternTicker.attach(&pattern_next, chrono::milliseconds(patternSpeed));
        }
        // debounce delay
        ThisThread::sleep_for(10ms);
    }
}
