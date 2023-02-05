#include "utils.h"

// cannot seem to get this to work yet. Might need to
// customize the version in the FreeRTOS runtime

void vApplicationStackOverflowHook( TaskHandle_t xTask, signed char *pcTaskName )
{
  printf("##########<<<<<<<< Main stack overflow\r\n");
  printf("Task %s handle 0x%x\r\n", pcTaskName, (unsigned int) xTask);
  while(1)
  {}
}

// overwrite array
void doError()
{
 const char *arr[] = { "0", "1"};
 uint8_t foo = 2;

 printf("foo = %s\r\n",  arr[foo]);
}

/**
 * return task name
 */

const char * getTaskName()
{
  TaskStatus_t xTaskDetails;
  getTaskDetails(&xTaskDetails);

	return xTaskDetails.pcTaskName;
}

/**
 * get current task details structure
 */

TaskStatus_t * getTaskDetails(TaskStatus_t *taskDetails)
{
   vTaskGetInfo(NULL,
        taskDetails,
        pdTRUE,
        eInvalid);

   return taskDetails;
}

