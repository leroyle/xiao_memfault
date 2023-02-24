#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

#define ENABLE_APP_CLI
#define appVersion "funbiscuit_Cli V0.5"
#include "cli.h"

void appSetup() {

    uint8_t cliRc = cliSetup(appVersion);
    if (cliRc == 0)
    {
        Serial.println(F("Cli has started. Enter your commands."));
    } else {
        Serial.println(F("<====== ERROR: Cli startup failed. ======"));
    }

}
 
void appLoop() {

// CLI input is handled in a separate task within the cli support library
}
