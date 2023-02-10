
#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

#include "memfaultMain.h"


void setup() {


    Serial.begin(115200);
    while (!Serial) taskYIELD();

#ifdef TERM_CLI
    Serial.println("Starting TERM_CLI");
#endif
#ifdef MEMFAULT_RTT
    Serial.println("Starting MEMFAULT_RTT");
#endif
#ifdef MEMFAULT_TERM
    Serial.println("Starting MEMFAULT_TERM");
#endif

    delay(500);
    memfaultSetup();
}



void loop() {

   memfaultLoop();
}

