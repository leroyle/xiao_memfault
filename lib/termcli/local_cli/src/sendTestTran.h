#ifndef __SEND_TEST_TRAN_COMMAND_H__
#define __SEND_TEST_TRAN_COMMAND_H__

#ifdef ENABLE_APP_CLI
#include "embedded_cli.h"

int8_t sendTestTran(const char *repeat, const char * repeatTime);
int8_t getTestTranList();
void onSendTestTran(EmbeddedCli *cli, char *args, void *context);
void onGetTestTranList(EmbeddedCli *cli, char *args, void *context);
void init_sendTestTran(EmbeddedCli *cli);

#endif  // ENABLE_APP_CLI
#endif
