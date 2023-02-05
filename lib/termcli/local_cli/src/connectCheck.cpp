#ifdef ENABLE_APP_CLI

#include <Arduino.h>
#ifdef WISBLOCK_4631
#include <LoRaWan-RAK4630.h>
#endif
#include "connectCheck.h"

/**
 * 
 */
void onConnectCheck (EmbeddedCli *cli, char *args, void *context) {

#ifdef WISBLOCK_4631
	bool connected = false;  
	lmh_join_status ret = lmh_join_status_get();

	if (ret == LMH_SET)
	{
		connected = true;
	}
	else
	{
		connected = false;
	}
	printf("LoRa Network Join status: %d\r\n", connected);
	printf("\t0: LMH_RESET\r\n\t1: LMH_SET (Connected)\r\n\t2: LMH_ONGOING\r\n\t3: LMH_FAILED\r\n");

	MibRequestConfirm_t mibReq;
	mibReq.Type = MIB_UPLINK_COUNTER;
	LoRaMacMibGetRequestConfirm(&mibReq);
	uint32_t count = mibReq.Param.UpLinkCounter;
	printf("Uplink Count: %lu\r\n", count);

	mibReq.Type = MIB_DOWNLINK_COUNTER;
	LoRaMacMibGetRequestConfirm(&mibReq);
	count = mibReq.Param.DownLinkCounter;
	printf("Downlink Count: %lu\r\n", count);
#else
	printf("\tLoRaWan Connection status not supported\r\n");
#endif // WISBLOCK_4631
}

void init_connectCheck(EmbeddedCli *cli)
{
	embeddedCliAddBinding(cli, {"connectCheck",
		"Check status of network connection and up/down link counts",
		false,
		nullptr,
		onConnectCheck
    });
}

#endif // ENABLE_APP_CLI
