/**
  * main entry for non LoRaWan apps.
  * for targets: xiao_cli, xiao_term, xiao_rtt, wisblock_cli,
  *              wisblock_term, wisblock_rtt
**/

#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

#include "appmain.h"

#define pdMSTOTICKS( xTimeInMs ) (( TickType_t ) xTimeInMs * configTICK_RATE_HZ / 1000 )

void loop_two(void *);
static TaskHandle_t  _loopHandle_two;

void setup() {


    Serial.begin(115200);
    delay(500);
    while (!Serial) taskYIELD();

    delay(500);
    appSetup();
    // Create a second task so we can easily verfy task switching is not broken
    xTaskCreate(loop_two, "looptwo", 0x400, NULL, TASK_PRIO_LOW, &_loopHandle_two);

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
    taskYIELD();
}


// second task to verify task switching

uint32_t lastTime2=0;
uint32_t delayTime2 = pdMS_TO_TICKS(5 * 1000);
uint32_t heartBeatCount2;
uint32_t initCount2=0;
void loop_two(void *) {
    while(1)
    {
        uint32_t currTime= millis();
        if(currTime > lastTime2 + delayTime2)
        {
            Serial.print("loop2 Hello ");
            Serial.println(heartBeatCount2++);
            Serial.flush();
            lastTime2 = currTime;
        }
        taskYIELD();
    }
}
