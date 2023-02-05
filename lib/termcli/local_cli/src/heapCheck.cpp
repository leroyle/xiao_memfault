#ifdef ENABLE_APP_CLI

#include <Arduino.h>
#include "heapCheck.h"
#include "repeatCommand.h"

static void isRepeat (uint32_t cycleTime);
static RepeatTest repeatFlag = STOPREPEAT;
static uint32_t lastRunTime = 0;

void init_heapCheck(EmbeddedCli *cli)
{
  embeddedCliAddBinding(cli, {
    "heapCheck",
    "Dump Heap Statistics [T/F, # seconds]",
    true,
    nullptr,
    onHeapCheck
  });
  }

  void onHeapCheck(EmbeddedCli *cli, char *args, void *context) {
    Serial.println(F("onHeapCheck: "));
    const char * _repeat = NULL;
    const char *  _repeatTime = NULL;
    embeddedCliTokenizeArgs(args);
    for (int i = 1; i <= embeddedCliGetTokenCount(args); ++i)
    {
        Serial.print(F("arg"));
        Serial.print((char)('0' + i));
        Serial.print(F(": "));
        const char *curArg = embeddedCliGetToken(args, i);
        Serial.println(curArg);

        if (i == 1)
        {
            _repeat = curArg;
        }
        if(i == 2)
        {
            _repeatTime = curArg;
        }
    }
    checkHeapSpace(_repeat, _repeatTime);
    Serial.print("\r\n");
}

/**
 * Print heap details
 * 
 */
uint32_t checkHeapSpace(const char *repeat, const char * repeatTime)
{

  TaskStatus_t xTaskDetails;

 if (repeat != NULL || repeatTime != NULL)
  {
    RepeatTest processRes = processRepeatArgs(repeat, repeatTime, &isRepeat);
    if(processRes == NOCHANGE)
    {
      // leave as is
      ;
    }
    else if(processRes == REPEAT)
    {
      repeatFlag = REPEAT;
    } else if(processRes == STOPREPEAT)
    {
      repeatFlag = STOPREPEAT;
    }
  }

  getTaskDetails(&xTaskDetails);

  // The called functions use info from the linker .map file
  // extern unsigned char __HeapBase[];
  // extern unsigned char __HeapLimit[];
  
  uint32_t sbrkVal = (uint32_t ) reinterpret_cast<char*>(sbrk(0));
  printf(PSTR("\r\n(%s): Heap Space Check()\r\n=========================> \r\n"), xTaskDetails.pcTaskName);
  printf(PSTR("Current sbrk() value: 0x%x\r\n"), (unsigned int)sbrkVal);
  int total = dbgHeapTotal(); 
  int used = dbgHeapUsed();
  int free =  dbgHeapFree();

  printf("Total:  %d (0x%x) \r\nUsed:   %d (0x%x) \r\nFree:   %d (0x%x)\r\n", total, total, used, used, free, free);
  printf(PSTR("=========================> \r\n\r\n"));
 
  return dbgHeapFree();
}

static void isRepeat (uint32_t cycleTime)
{
  if(repeatFlag == REPEAT)
  {
    uint32_t currentTime = millis();

    if(currentTime > (lastRunTime + cycleTime))
    {
      lastRunTime = currentTime;
      checkHeapSpace(NULL, NULL);
    }
  }
}
#endif // ENABLE_APP_CLI
