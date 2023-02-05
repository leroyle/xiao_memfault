
#ifndef __STACKCHECK_H__
#define __STACKCHECK_H__

#ifdef ENABLE_APP_CLI
#include "embedded_cli.h"

void stackCheck(const char *repeat, const char * repeatTime);
void onStackCheck(EmbeddedCli *cli, char *args, void *context);
void init_stackCheck(EmbeddedCli *cli);
#endif // ENABLE_APP_CLI
#endif
