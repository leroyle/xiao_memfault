#ifndef __REPEAT_COMMAND__H__
#define __REPEAT_COMMAND__H__

#ifdef ENABLE_APP_CLI
#include "embedded_cli.h"

typedef void (*IntFunc)(uint32_t);
struct RepeatArgsStruct
{
    IntFunc func;
    uint32_t cycleTime;
};

typedef enum RepeatTest {REPEAT, STOPREPEAT, NOCHANGE, UNKNOWN} RepeatTest;

typedef struct RepeatArgsStruct RepeatArgsStruct;
RepeatTest processRepeatArgs(const char *repeat, const char * repeatTime, void (*func_ptr)(uint32_t));
void  removeRepeatFunction ( void (*func_ptr)(uint32_t) );

int8_t getNumRepeatFunctions();
void runRepeatList();

void init_repeatCommand(EmbeddedCli *cli);
void onRepeat(EmbeddedCli *cli, char *args, void *context);

#endif  // ENABLE_APP_CLI
#endif
