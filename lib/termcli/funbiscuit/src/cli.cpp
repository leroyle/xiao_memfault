#ifdef ENABLE_APP_CLI
#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

#include "embedded_cli.h"
// New Command
#include "stackCheck.h"
#include "heapCheck.h"
#include "funbiscuit_cli.h"
#include "debugLevel.h"
#include "sendTestTran.h"
#include "repeatCommand.h"
#include "connectCheck.h"

char cliAppVersion [VERSION_MAX];
extern uint32_t appStartMillis;

// 164 bytes is minimum size for this params on Arduino Nano
#define CLI_BUFFER_SIZE 1000 // 164
#define CLI_RX_BUFFER_SIZE 16
#define CLI_CMD_BUFFER_SIZE 32
#define CLI_HISTORY_SIZE 32 *2

// New Command, this needs to be incremented
// when adding a new command
#define CLI_BINDING_COUNT 14

EmbeddedCli *cli;

CLI_UINT cliBuffer[BYTES_TO_CLI_UINTS(CLI_BUFFER_SIZE)];

void onCommand(EmbeddedCli *embeddedCli, CliCommand *command);

void writeChar(EmbeddedCli *embeddedCli, char c);

void onHello(EmbeddedCli *cli, char *args, void *context);

void onLed(EmbeddedCli *cli, char *args, void *context);

void onAdc(EmbeddedCli *cli, char *args, void *context);

// New Command
void onGetAppVersion(EmbeddedCli *cli, char *args, void *context);

void onUpTime(EmbeddedCli *cli, char *args, void *context);

// void onSendTestTran(EmbeddedCli *cli, char *args, void *context);

// void onGetTestTranList(EmbeddedCli *cli, char *args, void *context);

// end New Command

// CLI Task
void cliTask(void *arg);
static TaskHandle_t  _cliTaskHandle;
#define CLI_STACK_SZ       (256*5)
//

uint8_t cliSetup(const char *appVersion) {
   
    // App version
    if (appVersion != NULL)
    {
        strncpy(cliAppVersion, appVersion, VERSION_MAX) ;
    }
    else
    {
         strncpy(cliAppVersion, "App Version not defined", VERSION_MAX) ;
    } 
    EmbeddedCliConfig *config = embeddedCliDefaultConfig();
    config->cliBuffer = cliBuffer;
    config->cliBufferSize = CLI_BUFFER_SIZE * sizeof(CLI_UINT);
    config->rxBufferSize = CLI_RX_BUFFER_SIZE;
    config->cmdBufferSize = CLI_CMD_BUFFER_SIZE;
    config->historyBufferSize = CLI_HISTORY_SIZE;
    config->maxBindingCount = CLI_BINDING_COUNT;
    cli = embeddedCliNew(config);

    if (cli == NULL) {
        Serial.println(F("Cli was not created. Check sizes!"));
        return -1;
    }
    Serial.println(F("Cli has started. Enter your commands."));

    embeddedCliAddBinding(cli, {
            "get-led",
            "Get led status",
            false,
            nullptr,
            onLed
    });
    embeddedCliAddBinding(cli, {
            "get-adc",
            "Read adc value",
            false,
            nullptr,
            onAdc
    });
    embeddedCliAddBinding(cli, {
            "hello",
            "Print hello message",
            true,
            (void *) "World",
            onHello
    });

    // New Command
   
   // add binding for external commands
    init_repeatCommand(cli);
    init_stackCheck(cli);
    init_heapCheck(cli);
    init_debugLevel(cli);
    init_sendTestTran(cli);
    init_connectCheck(cli);


    embeddedCliAddBinding(cli, {
            "appVersion",
            "Get App Version String",
            true,
            nullptr,
            onGetAppVersion
    });

    embeddedCliAddBinding(cli, {
            "getUpTime",
            "Time since app started (Might roll over)",
            true,
            nullptr,
            onUpTime
    });

    // end New Command

    cli->onCommand = onCommand;
    cli->writeChar = writeChar;
     // create the CLI RTOS handler
    xTaskCreate(cliTask, "CLI_Handler", CLI_STACK_SZ, NULL, TASK_PRIO_LOW, &_cliTaskHandle);

    return 0;
}

void onCommand(EmbeddedCli *embeddedCli, CliCommand *command) {
    Serial.println(F("Received command:"));
    Serial.println(command->name);
    embeddedCliTokenizeArgs(command->args);
    for (int i = 1; i <= embeddedCliGetTokenCount(command->args); ++i) {
        Serial.print(F("arg"));
        Serial.print((char) ('0' + i));
        Serial.print(F(": "));
        Serial.println(embeddedCliGetToken(command->args, i));
    }
}

void onHello(EmbeddedCli *cli, char *args, void *context) {
    Serial.print(F("Hello "));
    if (embeddedCliGetTokenCount(args) == 0)
        Serial.print((const char *) context);
    else
        Serial.print(embeddedCliGetToken(args, 1));
    Serial.print("\r\n");
}

void onLed(EmbeddedCli *cli, char *args, void *context) {
    Serial.print(F("LED: "));
    Serial.print(random(256));
    Serial.print("\r\n");
}

void onAdc(EmbeddedCli *cli, char *args, void *context) {
    Serial.print(F("ADC: "));
    Serial.print(random(1024));
    Serial.print("\r\n");
}

// New Command

void onGetAppVersion (EmbeddedCli *cli, char *args, void *context) {
    Serial.print(cliAppVersion);
    Serial.print("\r\n");
}

uint32_t appStartMillis;
void onUpTime (EmbeddedCli *cli, char *args, void *context) {
    printf("App UpTime (seconds): %lu\r\n", (millis() - appStartMillis) /1000);
}

// End New Command

void writeChar(EmbeddedCli *embeddedCli, char c) {
    Serial.write(c);
}

void cliTask(void *arg)
{
    // use some stack space
    char buff[2000];
    buff[0] = '\0';
    sprintf(buff, "test me\r\n");
    while(1)
    {
        // provide all chars to cli
        while (Serial.available() > 0) {
            embeddedCliReceiveChar(cli, Serial.read());
        }

        embeddedCliProcess(cli);
        runRepeatList();
        // let other tasks run
        yield();
    }
}
#endif // ENABLE_APP_CLI
