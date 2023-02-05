#ifndef __HEAPCHECK_COMMAND_H__
#define __HEAPCHECK_COMMAND_H__

#ifdef ENABLE_APP_CLI
#include "embedded_cli.h"

uint32_t checkHeapSpace(const char *repeat, const char * repeatTime);
const char * getTaskName();
TaskStatus_t * getTaskDetails(TaskStatus_t *taskDetails);
void init_heapCheck(EmbeddedCli *cli);
void onHeapCheck(EmbeddedCli *cli, char *args, void *context);

// We need to delcare sbrk
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

#endif  // ENABLE_APP_CLI
#endif
