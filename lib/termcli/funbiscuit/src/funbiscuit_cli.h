#ifndef __FUNBISCUIT_CLI_H__
#define __FUNBISCUIT_CLI_H__
#ifdef ENABLE_APP_CLI

#include "embedded_cli.h"

// App version
#define VERSION_MAX 80

uint8_t cliSetup(const char * appVersionString);

#endif // ENABLE_APP_CLI
#endif

