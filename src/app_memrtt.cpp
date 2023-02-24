#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

#include "app_error.h"
#include "app_timer.h"

 #define ADD_WATCHDOG

extern "C" {
#include "nrf_pwr_mgmt.h"
}

#include "nrfx_wdt.h"
#include "mflt_cli.h"
#include "memfault/components.h"

#ifdef ADD_WATCHDOG
#include "memfault/ports/watchdog.h"
#endif

#include "memfault/ports/freertos.h"

/**
 *  MEMFAULT_RTT support uses Memfault RTT mode with a Segger debug module and 
 * JLinkRTTClient
 *
*/

#define appVersion "Memfault RTT CLI V0.5"

#define NRF_LOG_DEFAULT_BACKENDS_INIT() nrf_log_default_backends_init()

 extern "C" {
void mflt_cli_init(void);
ret_code_t nrf_drv_clock_init(void);
// void nrf_drv_clock_lfclk_request(void *);
void nrf_log_default_backends_init(void);
void mflt_cli_try_process(void);
}


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

void appSetup() {

   // these try to restart uart, results in a UART POWER CLOCK interrup
   // nrf_drv_clock_init();
   // nrf_drv_clock_lfclk_request(NULL);

   log_init();
   NRF_LOG_WARNING("TEST");

   timers_init();

   mflt_cli_init();
   memfault_freertos_port_boot();
   memfault_platform_boot();

#ifdef ADD_WATCHDOG
   prv_hardware_watchdog_enable();
   memfault_software_watchdog_enable();
#endif

}
 

void appLoop() {

#ifdef ADD_WATCHDOG
    prv_hardware_watchdog_feed();
    memfault_software_watchdog_feed();
#endif

    mflt_cli_try_process();
 if (NRF_LOG_PROCESS() == false) {
      nrf_pwr_mgmt_run();
 }
}
