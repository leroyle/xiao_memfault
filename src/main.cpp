
#ifdef INCLUDE_NO_LORA
#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

#include "memfaultMain.h"

#define pdMSTOTICKS( xTimeInMs ) (( TickType_t ) xTimeInMs * configTICK_RATE_HZ / 1000 )

void setup() {


    Serial.begin(115200);
    delay(500);
    while (!Serial) taskYIELD();

#ifdef TERM_CLI
    Serial.println("=====================================");
    Serial.println("Starting TERM_CLI");
#endif
#ifdef MEMFAULT_RTT
    Serial.println("=====================================");
    Serial.println("Starting MEMFAULT_RTT");
#endif
#ifdef MEMFAULT_TERM
    Serial.println("=====================================");
    Serial.println("Starting MEMFAULT_TERM");
#endif

    delay(500);
    memfaultSetup();
}



uint32_t lastTime=0;
uint32_t delayTime = pdMS_TO_TICKS(10 * 1000* 10);
uint32_t heartBeatCount;
uint32_t initCount=0;
void loop() {

//#ifdef ADD_HEARTBEAT
    uint32_t currTime= millis();
    if(currTime > lastTime + delayTime)
    {
        Serial.print("Hello World!: ");
        Serial.println(heartBeatCount++);
        Serial.flush();
        lastTime = currTime;
    }
//#endif

    memfaultLoop();
}

#else  // INCLUDE_NO_LORA

/**
  *  The INCLUDE_NO_LORA , if not defined includes LoRa implementation files
  *  
  *
*/
#define pdMSTOTICKS( xTimeInMs ) (( TickType_t ) xTimeInMs * configTICK_RATE_HZ / 1000 )

/**
 * @file LoRaWAN_OTAA_ABP.ino
 * @author rakwireless.com
 * @brief LoRaWan node example with OTAA/ABP registration
 * @version 0.1
 * @date 2020-08-21
 * 
 * @copyright Copyright (c) 2020
 * 
 * @note RAK4631 GPIO mapping to nRF52840 GPIO ports
   RAK4631    <->  nRF52840
   WB_IO1     <->  P0.17 (GPIO 17)
   WB_IO2     <->  P1.02 (GPIO 34)
   WB_IO3     <->  P0.21 (GPIO 21)
   WB_IO4     <->  P0.04 (GPIO 4)
   WB_IO5     <->  P0.09 (GPIO 9)
   WB_IO6     <->  P0.10 (GPIO 10)
   WB_SW1     <->  P0.01 (GPIO 1)
   WB_A0      <->  P0.04/AIN2 (AnalogIn A2)
   WB_A1      <->  P0.31/AIN7 (AnalogIn A7)
 */
#include <Arduino.h>
#include "Adafruit_TinyUSB.h"

// LoRaWan credentials
#include "loraCreds.h"
#ifdef MEMFAULT
#include "memfaultMain.h"
#endif
/** 
  The INCLUDE_LORA_API_CALLS adds in actual calls to the LoRa subsystem
*/

#ifdef INCLUDE_LORA_API_CALLS
#include <LoRaWan-RAK4630.h> //http://librarymanager/All#SX126x
#endif

// RAK4630 supply two LED
#ifndef LED_BUILTIN
#define LED_BUILTIN 35
#endif

#include <SPI.h>


#ifndef LED_BUILTIN2
#define LED_BUILTIN2 36
#endif

#ifdef INCLUDE_LORA_API_CALLS

bool doOTAA = true;   // OTAA is used by default.
#define SCHED_MAX_EVENT_DATA_SIZE APP_TIMER_SCHED_EVENT_DATA_SIZE /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE 60										  /**< Maximum number of events in the scheduler queue. */
#define LORAWAN_DATERATE DR_3									  /*LoRaMac datarates definition, from DR_0 to DR_5*/
// LAL#define LORAWAN_TX_POWER TX_POWER_5							/*LoRaMac tx power definition, from TX_POWER_0 to TX_POWER_15*/
#define LORAWAN_TX_POWER TX_POWER_14							/*LoRaMac tx power definition, from TX_POWER_0 to TX_POWER_15*/
#define JOINREQ_NBTRIALS 10										  /**< Number of trials for the join request. */
DeviceClass_t g_CurrentClass = CLASS_A;					/* class definition*/
LoRaMacRegion_t g_CurrentRegion = LORAMAC_REGION_US915;    /* Region:EU868*/
    
lmh_confirm g_CurrentConfirm = LMH_UNCONFIRMED_MSG;				  /* confirm/unconfirm packet definition*/
uint8_t gAppPort = LORAWAN_APP_PORT;							        /* data port*/

/**@brief Structure containing LoRaWan parameters, needed for lmh_init()
*/
static lmh_param_t g_lora_param_init = {LORAWAN_ADR_OFF, LORAWAN_DATERATE, LORAWAN_PUBLIC_NETWORK, JOINREQ_NBTRIALS, LORAWAN_TX_POWER, LORAWAN_DUTYCYCLE_OFF};

// Foward declaration
static void lorawan_has_joined_handler(void);
static void lorawan_join_failed_handler(void);
static void lorawan_rx_handler(lmh_app_data_t *app_data);
static void lorawan_confirm_class_handler(DeviceClass_t Class);
static void send_lora_frame(void);

/**@brief Structure containing LoRaWan callback functions, needed for lmh_init()
*/
static lmh_callback_t g_lora_callbacks = {BoardGetBatteryLevel, BoardGetUniqueId, BoardGetRandomSeed,
                                        lorawan_rx_handler, lorawan_has_joined_handler, lorawan_confirm_class_handler, lorawan_join_failed_handler
                                       };
// Private defination
#define LORAWAN_APP_DATA_BUFF_SIZE 64                     /**< buffer size of the data to be transmitted. */
#define LORAWAN_APP_INTERVAL 20000                        /**< Defines for user timer, the application data transmission interval. 20s, value in [ms]. */
static uint8_t m_lora_app_data_buffer[LORAWAN_APP_DATA_BUFF_SIZE];            //< Lora user application data buffer.
static lmh_app_data_t m_lora_app_data = {m_lora_app_data_buffer, 0, 0, 0, 0}; //< Lora user application data structure.

TimerEvent_t appTimer;
static uint32_t timers_init(void);
static uint32_t count = 0;
static uint32_t count_fail = 0;

#endif //INCLUDE_LORA_API_CALLS

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Initialize Serial for debug output
#ifdef LAL_WAIT
  time_t timeout = millis();
#endif
  Serial.begin(115200);

  while (!Serial) taskYIELD();
#ifdef LAL_WAIT
  while (!Serial)
  {
    if ((millis() - timeout) < 5000)
    {
      delay(100);
    }
    else
    {
      break;
    }
  }
#endif //LAL_WAIT

    delay(500);
  Serial.println("=====================================");
  Serial.println("Welcome to MEMFAULT LoRaWan, intializing!!!");
#ifdef MEMFAULT
    memfaultSetup();
#endif
  Serial.println("=====================================");
  Serial.println(" MEMFAULT LoRaWan ready to go!!!");
  Serial.flush();

#ifdef INCLUDE_LORA_API_CALLS
  // Initialize LoRa chip.
  lora_rak4630_init();

  TickType_t xDelay = pdMSTOTICKS(2000);
  vTaskDelay(xDelay);

  Serial.println("=====================================");
  Serial.println("Welcome to RAK4630 LoRaWan!!!");
  if (doOTAA)
  {
    Serial.println("Type: OTAA");
  }
  else
  {
    Serial.println("Type: ABP");
  }

  switch (g_CurrentRegion)
  {
    case LORAMAC_REGION_AS923:
      Serial.println("Region: AS923");
      break;
    case LORAMAC_REGION_AU915:
      Serial.println("Region: AU915");
      break;
    case LORAMAC_REGION_CN470:
      Serial.println("Region: CN470");
      break;
  case LORAMAC_REGION_CN779:
    Serial.println("Region: CN779");
    break;
    case LORAMAC_REGION_EU433:
      Serial.println("Region: EU433");
      break;
    case LORAMAC_REGION_IN865:
      Serial.println("Region: IN865");
      break;
    case LORAMAC_REGION_EU868:
      Serial.println("Region: EU868");
      break;
    case LORAMAC_REGION_KR920:
      Serial.println("Region: KR920");
      break;
    case LORAMAC_REGION_US915:
      Serial.println("Region: US915");
    break;
  case LORAMAC_REGION_RU864:
    Serial.println("Region: RU864");
    break;
  case LORAMAC_REGION_AS923_2:
    Serial.println("Region: AS923-2");
    break;
  case LORAMAC_REGION_AS923_3:
    Serial.println("Region: AS923-3");
    break;
  case LORAMAC_REGION_AS923_4:
    Serial.println("Region: AS923-4");
      break;
  }
  Serial.println("=====================================");
  
  //creat a user timer to send data to server period
  uint32_t err_code;
  err_code = timers_init();
  if (err_code != 0)
  {
    Serial.printf("timers_init failed - %d\n", err_code);
    return;
  }

  // Setup the EUIs and Keys
  //if (doOTAA)
  //{
    lmh_setDevEui(nodeDeviceEUI);
    lmh_setAppEui(nodeAppEUI);
    lmh_setAppKey(nodeAppKey);
  //}
  //else
  //{
 //   lmh_setNwkSKey(nodeNwsKey);
 //   lmh_setAppSKey(nodeAppsKey);
 //   lmh_setDevAddr(nodeDevAddr);
 // }

  // Initialize LoRaWan
  err_code = lmh_init(&g_lora_callbacks, g_lora_param_init, doOTAA, g_CurrentClass, g_CurrentRegion);
  if (err_code != 0)
  {
    Serial.printf("lmh_init failed - %d\n", err_code);
    return;
  }

  // Start Join procedure
  lmh_join();

#endif // INCLUDE_LORA_API_CALLS

}

uint32_t lastTime=0;
uint32_t delayTime = pdMS_TO_TICKS(10 * 1000* 10);
uint32_t heartBeatCount;
uint32_t initCount=0;
void loop()
{
//#ifdef ADD_HEARTBEAT
    uint32_t currTime= millis();
    if(currTime > lastTime + delayTime)
    {
        Serial.print("Hello World!: ");
        Serial.println(heartBeatCount++);
        Serial.flush();
        lastTime = currTime;
    }
//#endif

  // Put your application tasks here, like reading of sensors,
  // Controlling actuators and/or other functions. 


#ifdef MEMFAULT
    memfaultLoop();
#endif
}

#ifdef INCLUDE_LORA_API_CALLS
/**@brief LoRa function for handling HasJoined event.
 */
void lorawan_has_joined_handler(void)
{
  if(doOTAA == true)
  {
  Serial.println("OTAA Mode, Network Joined!");
  }
  else
  {
    Serial.println("ABP Mode");
  }

  lmh_error_status ret = lmh_class_request(g_CurrentClass);
  if (ret == LMH_SUCCESS)
  {
    delay(1000);
    TimerSetValue(&appTimer, LORAWAN_APP_INTERVAL);
    TimerStart(&appTimer);
  }
}
/**@brief LoRa function for handling OTAA join failed
*/
static void lorawan_join_failed_handler(void)
{
  Serial.println("OTAA join failed!");
  Serial.println("Check your EUI's and Keys's!");
  Serial.println("Check if a Gateway is in range!");
}
/**@brief Function for handling LoRaWan received data from Gateway
 *
 * @param[in] app_data  Pointer to rx data
 */
void lorawan_rx_handler(lmh_app_data_t *app_data)
{
  Serial.printf("LoRa Packet received on port %d, size:%d, rssi:%d, snr:%d, data:%s\n",
          app_data->port, app_data->buffsize, app_data->rssi, app_data->snr, app_data->buffer);
}

void lorawan_confirm_class_handler(DeviceClass_t Class)
{
  Serial.printf("switch to class %c done\n", "ABC"[Class]);
  // Informs the server that switch has occurred ASAP
  m_lora_app_data.buffsize = 0;
  m_lora_app_data.port = gAppPort;
  lmh_send(&m_lora_app_data, g_CurrentConfirm);
}

void send_lora_frame(void)
{
  if (lmh_join_status_get() != LMH_SET)
  {
    //Not joined, try again later
    return;
  }

  uint32_t i = 0;
  memset(m_lora_app_data.buffer, 0, LORAWAN_APP_DATA_BUFF_SIZE);
  m_lora_app_data.port = gAppPort;
  m_lora_app_data.buffer[i++] = 'H';
  m_lora_app_data.buffer[i++] = 'e';
  m_lora_app_data.buffer[i++] = 'l';
  m_lora_app_data.buffer[i++] = 'l';
  m_lora_app_data.buffer[i++] = 'o';
  m_lora_app_data.buffer[i++] = '!';
  m_lora_app_data.buffsize = i;

  lmh_error_status error = lmh_send(&m_lora_app_data, g_CurrentConfirm);
  if (error == LMH_SUCCESS)
  {
    count++;
    Serial.printf("lmh_send ok count %d\n", count);
  }
  else
  {
    count_fail++;
    Serial.printf("lmh_send fail count %d\n", count_fail);
  }
}

/**@brief Function for handling user timerout event.
 */
void tx_lora_periodic_handler(void)
{
  TimerSetValue(&appTimer, LORAWAN_APP_INTERVAL);
  TimerStart(&appTimer);
  Serial.println("Sending frame now...");
  send_lora_frame();
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
uint32_t timers_init(void)
{
  TimerInit(&appTimer, tx_lora_periodic_handler);
  return 0;
}

#endif // INCLUDE_LORA_API_CALLS
#endif  // INCLUDE_NO_LORA
