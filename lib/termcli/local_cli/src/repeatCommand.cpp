#ifdef ENABLE_APP_CLI

#include <Arduino.h>
#include "repeatCommand.h"
#include "assert.h"

static const uint32_t DEFAULT_CYCLE_TIME = 5000;
static const int8_t MAX_FUNC_PTRS = 5;

static int8_t funcPtrCount = 0;
IntFunc fparr[MAX_FUNC_PTRS] = {};
static RepeatArgsStruct repeatArgsArr[10]; 

 void init_repeatCommand(EmbeddedCli *cli)
 { 
  embeddedCliAddBinding(cli, {
    "repeat",
    "repeat last command (NOT SUPPORTED)",
    true,
    nullptr,
    onRepeat
  });
}

void onRepeat(EmbeddedCli *cli, char *args, void *context) {
    Serial.println(F("onRepeat: "));
    const char *_repeat = NULL;
    embeddedCliTokenizeArgs(args);
    for (int i = 1; i <= embeddedCliGetTokenCount(args); ++i) {
        Serial.print(F("arg"));
        Serial.print((char) ('0' + i));
        Serial.print(F(": "));
        _repeat = embeddedCliGetToken(args, i);
        Serial.printf("Repeat Arg: %s\r\n",_repeat); // embeddedCliGetToken(args, i));
    }
}

void  removeRepeatFunction ( void (*func_ptr)(uint32_t) )
{
  if (func_ptr == NULL || func_ptr < 0)
  {
    return;
  }
  
  for (uint8_t i = 0; i< funcPtrCount; i++)
    {
      if (repeatArgsArr[funcPtrCount - 1].func == func_ptr)
      {
        repeatArgsArr[funcPtrCount - 1].func = NULL;
        repeatArgsArr[funcPtrCount - 1].cycleTime = 0;
        funcPtrCount --;
      }
    }

  assert(funcPtrCount >= 0);
}

int8_t getNumRepeatFunctions()
{
  return funcPtrCount;
}

void runRepeatList()
{
  if (funcPtrCount > 0)
  {
    for (uint8_t i = 0; i< funcPtrCount; i++)
    {
      if (repeatArgsArr[i].func != NULL)
      {
        repeatArgsArr[i].func(repeatArgsArr[i].cycleTime);
      }
    }
  }
}

RepeatTest processRepeatArgs(const char *repeat, const char * repeatTime,  void (*func_ptr)(uint32_t))
{
  char *strPartRepeat = NULL;
  long strtolRet = 0;
  uint32_t cycleTime = DEFAULT_CYCLE_TIME;

  if (func_ptr == NULL)
  {
    return NOCHANGE;
  }

  if (repeatTime != NULL)
  {
    strtolRet = strtol(repeatTime, &strPartRepeat, 10);
    if (strPartRepeat == repeatTime)
    {
        printf("No number\r\n");
    }
    else
    {
      if (strtolRet != 0)
      {
        cycleTime = strtolRet * 1000;
      }
      printf("Reset repeat cycle time: %lu\r\n", cycleTime);
    }
  }

  if (repeat != NULL && repeat[0] == 'T')
  {
    // before we add this function, try to remove any existing
    // entry
    removeRepeatFunction(func_ptr);
    repeatArgsArr[funcPtrCount].func = func_ptr;
    repeatArgsArr[funcPtrCount].cycleTime = cycleTime;
    funcPtrCount++;
    return REPEAT;
  } else if (repeat != NULL && repeat[0] == 'F'){
      removeRepeatFunction(func_ptr);
      
    return STOPREPEAT;
  }
  return NOCHANGE;
}
#endif // ENABLE_APP_CLI
