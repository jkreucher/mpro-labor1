#include "DigitalOut.h"
#include "ThisThread.h"
#include "mbed.h"
#include "stm32f4xx_ll_tim.h"
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

#define DEBOUNCE_DELAY  20ms

// led bus define
// BusOut will not work with ticker isr
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

// timer for interrupt
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
//   Using BusOut crashes mbed-os when changing inside an isr
uint8_t leds_value;
void set_leds(uint8_t data) {
    ledRed1    = data & 0x01;
    ledOrange1 = (data >> 1) & 0x01;
    ledGreen1  = (data >> 2) & 0x01;
    ledGreen2  = (data >> 3) & 0x01;
    ledOrange2 = (data >> 4) & 0x01;
    ledRed2    = (data >> 5) & 0x01;
    // set leds_value to current setting
    leds_value = data;
}


// this function advances to the next index in a pattern and displays it
void pattern_next() {
    // check which pattern to show
    if(patternNum == 0) {
        // pattern: blink all
        patternStep = 0;
        // invert leds
        set_leds(~leds_value);
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
    leds_value = 0;
    patternDir = 0;
    patternNum = 0;
    patternStep = 0;
    patternSpeed = 300;

    // start timer
    patternTicker.attach(&pattern_next, chrono::milliseconds(patternSpeed));

    // main loop
    while (true) {
        // set direction by slide switch
        patternDir = buttonDir.read();

        // check dot pattern button
        if(buttonDot.read() == 1) {
            // debounce delay
            ThisThread::sleep_for(DEBOUNCE_DELAY);
            // check second button
            if(buttonBar.read() == 0) {
                // bar button is not pressed
                // change pattern to 1st pattern
                patternNum = 1;
            }
        }
        // check bar pattern button
        else if(buttonBar.read() == 1) {
            // debounce delay
            ThisThread::sleep_for(DEBOUNCE_DELAY);
            // check dot button
            if(buttonDot.read() == 0) {
                // dot button not pressed
                patternNum = 2;
            }
        }
        // check fast speed button and if fast speed already set
        // if so dont change speed to fast again
        else if((buttonFast.read() == 1) && (patternSpeed != 50)) {
            // debounce delay
            ThisThread::sleep_for(DEBOUNCE_DELAY);
            // check slow button
            if(buttonSlow.read() == 0) {
                // slow button not pressed
                patternSpeed = 50;
                // update timer speed
                patternTicker.attach(&pattern_next, chrono::milliseconds(patternSpeed));
            }
        }
        // check slow speed button and if slow speed already set
        // if so dont change speed to slow again
        else if((buttonSlow.read() == 1) && (patternSpeed != 2000)) {
            // debounce delay
            ThisThread::sleep_for(DEBOUNCE_DELAY);
            // check fast button
            if(buttonFast.read() == 0) {
                // fast button not pressed
                patternSpeed = 2000;
                // update timer speed
                patternTicker.attach(&pattern_next, chrono::milliseconds(patternSpeed));
            }
        }

        // debounce delay
        ThisThread::sleep_for(DEBOUNCE_DELAY);
    }
}
