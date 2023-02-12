
#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

// Now defined in platformio.ini
//#define TERM_CLI
// #define MEMFAULT_RTT
// #define MEMFAULT_TERM

#include "app_error.h"
#include "app_timer.h"


extern "C" {
#include "nrf_pwr_mgmt.h"
}

#include "nrfx_wdt.h"
#include "mflt_cli.h"
#include "memfault/components.h"

#if defined (MEMFAULT_TERM) || defined (MEMFAULT_RTT)

#ifdef ADD_WATCHDOG
#include "memfault/ports/watchdog.h"
#endif

#endif

#include "memfault/ports/freertos.h"

/**
 *  define
 *  TERM_CLI to use custom funbiscuit cli implementation
 *  MEMFAULT_TERM to use Memfault shell mode with a serial terminal
 *  MEMFAULT_RTT to use Memfault RTT mode with a Segger debug module and 
 * JLinkRTTClient
 *
*/
#ifdef TERM_CLI
#define ENABLE_APP_CLI
#define appVersion "funbiscuit_Cli V0.5"
#include "cli.h"
void term_CLI_Loop(void);
#endif

#ifdef MEMFAULT_TERM
#define appVersion "Memfault Term CLI V0.5"
void memfault_TERM_Loop(void);
#endif

#ifdef MEMFAULT_RTT
#define appVersion "Memfault RTT CLI V0.5"
void memfault_RTT_Loop(void);
#endif


#define NRF_LOG_DEFAULT_BACKENDS_INIT() nrf_log_default_backends_init()

 extern "C" {
void mflt_cli_init(void);
ret_code_t nrf_drv_clock_init(void);
// void nrf_drv_clock_lfclk_request(void *);
void nrf_log_default_backends_init(void);
void mflt_cli_try_process(void);
}

#if defined (MEMFAULT_TERM) || defined (MEMFAULT_RTT)

static void log_init(void) {
  const ret_code_t err_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(err_code);

  NRF_LOG_DEFAULT_BACKENDS_INIT();
}

extern "C" int memfault_platform_boot(void);

static void timers_init(void) {
   ret_code_t err_code;

   err_code = app_timer_init();
   APP_ERROR_CHECK(err_code);
}
#endif

#ifdef ADD_WATCHDOG
static void prv_wdt_event_handler(void) {
  // Note: This handler should be entirely unreachable as the software watchdog should kick in
  // first but just in case let's reboot if it is invoked.
  memfault_reboot_tracking_mark_reset_imminent(kMfltRebootReason_HardwareWatchdog, NULL);
  memfault_platform_reboot();
  MEMFAULT_UNREACHABLE;
}

static nrfx_wdt_channel_id s_wdt_channel_id;

static void prv_hardware_watchdog_enable(void) {
  nrfx_wdt_config_t config = NRFX_WDT_DEAFULT_CONFIG;

  uint32_t err_code = nrfx_wdt_init(&config, prv_wdt_event_handler);
  APP_ERROR_CHECK(err_code);
  err_code = nrfx_wdt_channel_alloc(&s_wdt_channel_id);
  APP_ERROR_CHECK(err_code);
  nrfx_wdt_enable();
}

static void prv_hardware_watchdog_feed(void) {
  nrfx_wdt_channel_feed(s_wdt_channel_id);
}
#endif // ADD_WATCHDOG

#ifdef MEMFAULT_TERM
// Needed by Memfault for output to a serial terminal in shell mode
//! We will use the Memfault CLI shell as a very basic debug interface
  static int prv_send_char(char c);

  const sMemfaultShellImpl impl = {
    .send_char = prv_send_char,
 };

static int prv_send_char(char c) {
  fprintf(stderr,"%c", c);
  return 0;
}
#endif

void memfaultSetup() {

   // these try to restart uart, results in a UART POWER CLOCK interrup
   // nrf_drv_clock_init();
   // nrf_drv_clock_lfclk_request(NULL);

#if defined (MEMFAULT_TERM) || defined (MEMFAULT_RTT)
   log_init();
   NRF_LOG_WARNING("TEST");

   timers_init();

   mflt_cli_init();
   memfault_freertos_port_boot();
   memfault_platform_boot();

#if defined (MEMFAULT_TERM) 
   memfault_demo_shell_boot(&impl);
   prv_send_char('\n');
#endif
#endif

#ifdef ADD_WATCHDOG
   prv_hardware_watchdog_enable();
   memfault_software_watchdog_enable();
#endif


#ifdef TERM_CLI
    uint8_t cliRc = cliSetup(appVersion);
    if (cliRc == 0)
    {
        Serial.println(F("Cli has started. Enter your commands."));
    } else {
        Serial.println(F("<====== ERROR: Cli startup failed. ======"));
    }

#endif
}
 

void memfaultLoop() {

#ifdef ADD_WATCHDOG
    prv_hardware_watchdog_feed();
    memfault_software_watchdog_feed();
#endif


// finish loop depending on whats enabled
#ifdef MEMFAULT_TERM
	memfault_TERM_Loop();
#endif
#ifdef MEMFAULT_RTT
	memfault_RTT_Loop();
#endif
//#ifdef TERM_CLI
//	term_CLI_Loop();
//#endif


}

#ifdef MEMFAULT_TERM

void memfault_TERM_Loop(void)
{

    while (Serial.available() > 0)
    {
	char rx_byte = Serial.read();
	//prv_send_char( rx_byte);
	memfault_demo_shell_receive_char(rx_byte);
    }
 if (NRF_LOG_PROCESS() == false) {
      nrf_pwr_mgmt_run();
 }
}
#endif 

#ifdef MEMFAULT_RTT
void memfault_RTT_Loop(void)
{

    mflt_cli_try_process();
 if (NRF_LOG_PROCESS() == false) {
      nrf_pwr_mgmt_run();
 }
}
#endif 

#ifdef TERM_CLI
void term_CLI_Loop(void)
{
    while (Serial.available() > 0)
    {
        // if (appCli != NULL)
        // {
        //     embeddedCliReceiveChar(appCli, Serial.read());
       //  }
    }

    //if (appCli != NULL)
    //{
      //  embeddedCliProcess(appCli);
   // }

}
#endif

