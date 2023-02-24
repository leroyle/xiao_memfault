/**
  * main entry for non LoRaWan apps.
  * for targets: xiao_cli, xiao_term, xiao_rtt, wisblock_cli,
  *              wisblock_term, wisblock_rtt
**/

#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

#include "appmain.h"

#define pdMSTOTICKS( xTimeInMs ) (( TickType_t ) xTimeInMs * configTICK_RATE_HZ / 1000 )

void setup() {


    Serial.begin(115200);
    delay(500);
    while (!Serial) taskYIELD();

    delay(500);
    appSetup();
}

uint32_t lastTime=0;
uint32_t delayTime = pdMS_TO_TICKS(10 * 1000);
uint32_t heartBeatCount;
uint32_t initCount=0;
void loop() {

    uint32_t currTime= millis();
    if(currTime > lastTime + delayTime)
    {
        Serial.print("Hello World!: ");
        Serial.println(heartBeatCount++);
        Serial.flush();
        lastTime = currTime;
    }

    appLoop();
}

