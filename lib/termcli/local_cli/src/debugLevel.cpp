#ifdef ENABLE_APP_CLI

#include <Arduino.h>
#include "debugLevel.h"
int8_t _appDebugLevel = 4;
extern int8_t *_libDbgLevelPtr;

void init_debugLevel(EmbeddedCli *cli)
{
  embeddedCliAddBinding(cli, {
    "setDebugLevel",
    "Set new debug level",
    true,
    nullptr,
    onSetDebugLevel
  });

  embeddedCliAddBinding(cli, {
    "getDebugLevel",
    "Get current debug level",
    true,
    nullptr,
    onGetDebugLevel
  });
}

void onSetDebugLevel (EmbeddedCli *cli, char *args, void *context) {
    // Serial.println(F("Received setDebugLevel:"));
    Serial.println(F("onSetDebugLevel"));
    const char * dbgLevel = NULL;
    embeddedCliTokenizeArgs(args);
    for (int i = 1; i <= embeddedCliGetTokenCount(args); ++i) {
        Serial.print(F("arg"));
        Serial.print((char) ('0' + i));
        Serial.print(F(": "));
        dbgLevel = embeddedCliGetToken(args, i);
        Serial.println(dbgLevel); // embeddedCliGetToken(args, i));
    }
    if(dbgLevel != NULL)
    {
        setDebugLevel(dbgLevel);
    }
}

void onGetDebugLevel (EmbeddedCli *cli, char *args, void *context) {
    getDebugLevel();
}


/**
 * 
 * 
 */
int8_t getDebugLevel()
{
  // print level
  printf("Current Debug level: %d\r\n", _appDebugLevel);
  return _appDebugLevel;
}

void setDebugLevel(const char * _debugLevel)
{
  char *strPart;
  long ret;
  // print level
  printf("Set new debug level: %s\r\n", _debugLevel);
  
  ret = strtol(_debugLevel, &strPart, 10);
  if (strPart == _debugLevel)
  {
      printf("No number\r\n");
  }
  else
  {
      *_libDbgLevelPtr = ret;
    _appDebugLevel = ret;
  }

}

#endif // ENABLE_APP_CLI
