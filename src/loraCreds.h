#ifndef __LORA_CREDS_H__
#define __LORA_CREDS_H__
/** 
  add your LoRaWan console creds
*/
#error:  "Error: add your LoRaWan Credentials here"

#define PROD_CONSOLE

#ifdef PROD_CONSOLE
uint8_t nodeDeviceEUI[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t nodeAppEUI[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t nodeAppKey[16] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};


#else // Staging Console

uint8_t nodeDeviceEUI[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t nodeAppEUI[8] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
uint8_t nodeAppKey[16] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

#endif

#endif  // __LORA_CREDS_h__
