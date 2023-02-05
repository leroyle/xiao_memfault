#ifdef ENABLE_APP_CLI
#include <Arduino.h>
#include "stackCheck.h"
#include "repeatCommand.h"

void dbgStackCheck();
static RepeatTest repeatFlag = STOPREPEAT;
static uint32_t lastRunTime = 0;

void dbgStackCheck(const char *repeat, const char * repeatTime);
void loc_vTaskList(char * pcWriteBuffer );
static void isRepeat (uint32_t cycleTime);
static char *loc_prvWriteNameToBuffer( char *pcBuffer, const char *pcTaskName );

void init_stackCheck(EmbeddedCli *cli)
{
	embeddedCliAddBinding(cli, {
		"stackCheck",
		"Dump Stack Statistics [T/F, # seconds]",
		true,
		nullptr,
		onStackCheck
    });
}

void onStackCheck(EmbeddedCli *cli, char *args, void *context) {
    Serial.println(F("onStackCheck: "));
    
    const char * _repeat = NULL;
    const char *  _repeatTime = NULL;
    embeddedCliTokenizeArgs(args);
    for (int i = 1; i <= embeddedCliGetTokenCount(args); ++i)
    {
        Serial.print(F("arg"));
        Serial.print((char)('0' + i));
        Serial.print(F(": "));
        const char *curArg = embeddedCliGetToken(args, i);
        Serial.println(curArg);

        if (i == 1)
        {
            _repeat = curArg;
        }
        if(i == 2)
        {
            _repeatTime = curArg;
        }
    }
    stackCheck(_repeat, _repeatTime);
    Serial.print("\r\n");
}

void stackCheck(const char *repeat, const char * repeatTime)
{
    // the Adafruit delivered version of dbgMemInfo() dumps out
    // heap info as well as stack, we only want stack.
    //dbgMemInfo();
    dbgStackCheck(repeat, repeatTime);
}

void dbgStackCheck(const char *repeat, const char * repeatTime)
{
  if (repeat != NULL || repeatTime != NULL)
  {
    RepeatTest processRes = processRepeatArgs(repeat, repeatTime, &isRepeat);
    if(processRes == NOCHANGE)
    {
      // leave as is
      ;
    }
    else if(processRes == REPEAT)
    {
      repeatFlag = REPEAT;
    } else if(processRes == STOPREPEAT)
    {
      repeatFlag = STOPREPEAT;
    }
  }

  // Print Task list with stack left stat
  uint32_t tasknum = uxTaskGetNumberOfTasks();
  char* buf = (char*) rtos_malloc(tasknum*80); // 40 bytes per task

  loc_vTaskList(buf);

  PRINTF("Task    State   Prio  StackLeft Num    Base Address   Approx Size(Bytes/Words\r\n");
  PRINTF("-------------------------------------------------------------------------------\r\n");
  PRINTF(buf);
  PRINTF("\n");
  
  rtos_free(buf);
}

static void __attribute__ ((unused)) isRepeat (uint32_t cycleTime)
{
  if(repeatFlag == REPEAT)
  {
    uint32_t currentTime = millis();

    if(currentTime > (lastRunTime + cycleTime))
    {
      lastRunTime = currentTime;
      dbgStackCheck(NULL, NULL);
    }
  }
}
/**
 * @brief This was copied from the FreeRTOS implementation file at:
 *  .platformio/packages/framework-arduinoadafruitnrf52/cores/nRF5/freertos/Source/tasks.c
 *  It's been modified to dump the stack start address as well
 *  as trying to fetch the allocated stack size
 * @param pcWriteBuffer 
 */
void loc_vTaskList(char * pcWriteBuffer )
	{
	
	const char tskRUNNING_CHAR = 'X';
	const char tskBLOCKED_CHAR = 'B';
	const char tskREADY_CHAR = 'R';
	const char tskDELETED_CHAR = 'D';
	const char tskSUSPENDED_CHAR = 'S';
	
	TaskStatus_t *pxTaskStatusArray;
	volatile UBaseType_t uxArraySize, x;
	char cStatus;
	volatile UBaseType_t uxCurrentNumberOfTasks 	= ( UBaseType_t ) 0U;

		/*
		 * PLEASE NOTE:
		 *
		 * This function is provided for convenience only, and is used by many
		 * of the demo applications.  Do not consider it to be part of the
		 * scheduler.
		 *
		 * vTaskList() calls uxTaskGetSystemState(), then formats part of the
		 * uxTaskGetSystemState() output into a human readable table that
		 * displays task names, states and stack usage.
		 *
		 * vTaskList() has a dependency on the sprintf() C library function that
		 * might bloat the code size, use a lot of stack, and provide different
		 * results on different platforms.  An alternative, tiny, third party,
		 * and limited functionality implementation of sprintf() is provided in
		 * many of the FreeRTOS/Demo sub-directories in a file called
		 * printf-stdarg.c (note printf-stdarg.c does not provide a full
		 * snprintf() implementation!).
		 *
		 * It is recommended that production systems call uxTaskGetSystemState()
		 * directly to get access to raw stats data, rather than indirectly
		 * through a call to vTaskList().
		 */


		/* Make sure the write buffer does not contain a string. */
		*pcWriteBuffer = 0x00;

		/* Take a snapshot of the number of tasks in case it changes while this
		function is executing. */
		uxCurrentNumberOfTasks = uxTaskGetNumberOfTasks();
		
		uxArraySize = uxCurrentNumberOfTasks;

		/* Allocate an array index for each task.  NOTE!  if
		configSUPPORT_DYNAMIC_ALLOCATION is set to 0 then pvPortMalloc() will
		equate to NULL. */
		pxTaskStatusArray = (TaskStatus_t *) pvPortMalloc( uxCurrentNumberOfTasks * sizeof( TaskStatus_t ) );

		if( pxTaskStatusArray != NULL )
		{
			/* Generate the (binary) data. */
			uxArraySize = uxTaskGetSystemState( pxTaskStatusArray, uxArraySize, NULL );

			/* Create a human readable table from the binary data. */
			for( x = 0; x < uxArraySize; x++ )
			{

				switch( pxTaskStatusArray[ x ].eCurrentState )
				{
					case eRunning:		cStatus = tskRUNNING_CHAR;
										break;

					case eReady:		cStatus = tskREADY_CHAR;
										break;

					case eBlocked:		cStatus = tskBLOCKED_CHAR;
										break;

					case eSuspended:	cStatus = tskSUSPENDED_CHAR;
										break;

					case eDeleted:		cStatus = tskDELETED_CHAR;
										break;

					default:			/* Should not get here, but it is included
										to prevent static checking errors. */
										cStatus = 0x00;
										break;
				}

				/* Write the task name to the string, padding with spaces so it
				can be printed in tabular form more easily. */
				pcWriteBuffer = loc_prvWriteNameToBuffer( pcWriteBuffer, pxTaskStatusArray[ x ].pcTaskName );
				/* Write the rest of the string. */
				
				/** trying to get to the size of the allocated stack buffer
				  * The implementation of malloc Adafruit/FreeRTOS uses
				  * https://github.com/eblot/newlib/blob/master/newlib/libc/stdlib/nano-mallocr.c
				  * stores the allocated size at the returned address -1 or -2.
				  * The size is stored at -2 if malloc needs to pad for alignment, addr -1 is the
				  * negative offset to the size address.
				  * if no padding is required, size is found at addr-1
				  * Thus we can test addr-1, not negative we have the size, if it
				  * is negative we must get the size from addr-2. Seems to work for
				  * this implementation of malloc.  
				*/ 
				
				volatile int32_t myValue = 0;
				volatile int32_t myValueWords = 0;

				myValue = *(pxTaskStatusArray[x].pxStackBase-1);
				if (myValue < (int32_t) 0)
				{
					myValue = *(pxTaskStatusArray[x].pxStackBase-2);
					 if (myValue < (int32_t) 0)
					{
						myValue = 0;
					}
				}
				if (myValue != 0)
				{
					myValueWords = myValue/4;
				}
				// sprintf( pcWriteBuffer, "\t%c\t%u\t%u\t%u\r\n", cStatus, ( unsigned int ) pxTaskStatusArray[ x ].uxCurrentPriority, ( unsigned int ) pxTaskStatusArray[ x ].usStackHighWaterMark, ( unsigned int ) pxTaskStatusArray[ x ].xTaskNumber);
				sprintf( pcWriteBuffer, "\t%c\t%u\t%u\t%u\t%p\t%lu/%lu\r\n", cStatus, ( unsigned int ) pxTaskStatusArray[ x ].uxCurrentPriority, ( unsigned int ) pxTaskStatusArray[ x ].usStackHighWaterMark, ( unsigned int ) pxTaskStatusArray[ x ].xTaskNumber, pxTaskStatusArray[x].pxStackBase, myValue, myValueWords );
				pcWriteBuffer += strlen( pcWriteBuffer );
			
			}

			/* Free the array again.  NOTE!  If configSUPPORT_DYNAMIC_ALLOCATION
			is 0 then vPortFree() will be #defined to nothing. */
			vPortFree( pxTaskStatusArray );
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}

/**
 * @brief copied from FreeRTOS implementation
 * 
 * @param pcBuffer 
 * @param pcTaskName 
 * @return char* 
 */
static char *loc_prvWriteNameToBuffer( char *pcBuffer, const char *pcTaskName )
	{
	size_t x;

		/* Start by copying the entire string. */
		strcpy( pcBuffer, pcTaskName );

		/* Pad the end of the string with spaces to ensure columns line up when
		printed out. */
		for( x = strlen( pcBuffer ); x < ( size_t ) ( configMAX_TASK_NAME_LEN - 1 ); x++ )
		{
			pcBuffer[ x ] = ' ';
		}

		/* Terminate. */
		pcBuffer[ x ] = 0x00;

		/* Return the new end of string. */
		return &( pcBuffer[ x ] );
	}
#endif // ENABLE_APP_CLI
