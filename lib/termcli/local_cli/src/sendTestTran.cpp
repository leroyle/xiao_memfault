#ifdef ENABLE_APP_CLI

#include <Arduino.h>
#ifdef WISBLOCK_4631
#include <LoRaWan-RAK4630.h>
#endif
#include "sendTestTran.h"
#include "repeatCommand.h"

static RepeatTest repeatFlag = STOPREPEAT;
void sendSensorTestMsgs();
static uint32_t lastRunTime = 0;

void init_sendTestTran(EmbeddedCli *cli)
{
    embeddedCliAddBinding(cli, {
    "sendTestTran",
    "Send test trans to the network [T/F, # seconds]",
    true,
    nullptr,
    onSendTestTran
    });

    embeddedCliAddBinding(cli, {"getTestTranList",
    "print list of possible test transactions",
    false,
    nullptr,
    onGetTestTranList
    });
}

void onSendTestTran(EmbeddedCli *cli, char *args, void *context)
{
#ifdef WISBLOCK_4631
  const char * _repeat = NULL;
  const char *  _repeatTime = NULL;
  embeddedCliTokenizeArgs(args);
  for (int i = 1; i <= embeddedCliGetTokenCount(args); ++i)
  {
      
    Serial.print(F("arg"));
    Serial.print((char)('0' + i));
    Serial.print(F(": "));
    // Serial.println(embeddedCliGetToken(args, i));
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
    sendTestTran(_repeat, _repeatTime);
#endif // WISBLOCK_4631
}

void onGetTestTranList(EmbeddedCli *cli, char *args, void *context)
{
    getTestTranList();
}

/**
 * 
 */
int8_t getTestTranList()
{
  // print level
  Serial.println("Supported Test Transactions");
  Serial.println("Test 1");
  Serial.println("Test 2");
  Serial.println("Test 3");

  return 3;
}

static void __attribute__ ((unused)) isRepeat (uint32_t cycleTime)
{
  if(repeatFlag == REPEAT)
  {
    uint32_t currentTime = millis();

    if(currentTime > (lastRunTime + cycleTime))
    {
      lastRunTime = currentTime;
      sendTestTran(NULL, NULL);
    }
  }
}

int8_t sendTestTran(const char *repeat, const char * repeatTime)
{
#ifdef WISBLOCK_4631
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
  sendSensorTestMsgs();
#endif // WISBLOCK_4631
  return 0;
}

#endif // ENABLE_APP_CLI
