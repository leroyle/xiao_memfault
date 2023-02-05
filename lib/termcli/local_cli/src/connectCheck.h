#ifndef __CONNECT_CHECK_H__
#define __CONNECT_CHECK_H__

#ifdef ENABLE_APP_CLI
#include "embedded_cli.h"

void init_connectCheck(EmbeddedCli *cli);
void onConnectCheck(EmbeddedCli *cli, char *args, void *context);

#endif  // ENABLE_APP_CLI
#endif
