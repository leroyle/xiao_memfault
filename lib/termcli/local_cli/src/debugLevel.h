#ifndef __DEBUGLEVEL_COMMAND_H__
#define __DEBUGLEVEL_COMMAND_H__

#ifdef ENABLE_APP_CLI
#include "embedded_cli.h"

int8_t getDebugLevel();
void setDebugLevel(const char * level);
void onGetDebugLevel(EmbeddedCli *cli, char *args, void *context);
void onSetDebugLevel(EmbeddedCli *cli, char *args, void *context);
void init_debugLevel(EmbeddedCli *cli);

#endif  // ENABLE_APP_CLI
#endif
