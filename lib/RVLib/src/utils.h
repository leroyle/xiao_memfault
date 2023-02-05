#ifndef _UTILS_H_
#define _UTILS_H_

// #include <LoRaWan-RAK4630.h>
#include "Arduino.h"
#include "FreeRTOS.h"
#include "task.h"

// void doError();
// void overflow();

TaskStatus_t * getTaskDetails(TaskStatus_t *taskDetails);
const char * getTaskName();

#endif
