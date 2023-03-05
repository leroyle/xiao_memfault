
#include "stdio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "memfault/core/debug_log.h"

char* sbrk(int incr);
int dbgHeapTotal(void);
int dbgHeapUsed(void);

int memfault_demo_cli_cmd_test_hang(MEMFAULT_UNUSED int argc, MEMFAULT_UNUSED char *argv[]) {
  // spin waiting for Watchdog timer to expire
  while(1) {

  }

  return -1;
}

int memfault_demo_cli_cmd_heap_check(MEMFAULT_UNUSED int argc, MEMFAULT_UNUSED char *argv[]) {

  uint32_t sbrkVal = (uint32_t ) sbrk(0);
  MEMFAULT_LOG_INFO("\r\n Heap Space Check()\r\n=========================> \r\n");
  MEMFAULT_LOG_INFO("Current sbrk() value: 0x%x\r\n", (unsigned int)sbrkVal);
  int total = dbgHeapTotal();
  int used = dbgHeapUsed();
  int free =  total - used;

  MEMFAULT_LOG_INFO("Total:  %d (0x%x) \r\nUsed:   %d (0x%x) \r\nFree:   %d (0x%x)\r\n", total, total, used, used, free, free);
  MEMFAULT_LOG_INFO("=========================> \r\n\r\n");

  return free;
}


void loc_vTaskList(char * pcWriteBuffer );
static char *loc_prvWriteNameToBuffer( char *pcBuffer, const char *pcTaskName );


int memfault_demo_cli_cmd_stack_check(MEMFAULT_UNUSED int argc, MEMFAULT_UNUSED char *argv[]) {

  // Print Task list with stack left stat
  uint32_t tasknum = uxTaskGetNumberOfTasks();
  char* buf = (char*) pvPortMalloc(tasknum*80); // 40 bytes per task

  loc_vTaskList(buf);

  MEMFAULT_LOG_INFO("Task    State   Prio  StackLeft Num    Base Address   Approx Size(Bytes/Words\r\n");
  MEMFAULT_LOG_INFO("-------------------------------------------------------------------------------\r\n");

  // max len of a single Memfault log line
  
  char * printPtr = buf;
  int16_t amtLeft = strlen(printPtr);

  MEMFAULT_LOG_INFO("DataSize: %d", amtLeft);

  while(amtLeft > 0)
  {
        MEMFAULT_LOG_INFO("%s",(char*) printPtr);
        amtLeft -= MEMFAULT_LOG_MAX_LINE_SAVE_LEN +1;
        if( amtLeft > 0)
        {    
            printPtr +=  MEMFAULT_LOG_MAX_LINE_SAVE_LEN +1;

            if (amtLeft > MEMFAULT_LOG_MAX_LINE_SAVE_LEN +1)
            {
                continue;
            }
            else
            {
                MEMFAULT_LOG_INFO("%s",(char*) printPtr);
                break;
            }
        }
  }
  MEMFAULT_LOG_INFO("\n");
  
  vPortFree( buf );
  return (0);
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
	#define tskRUNNING_CHAR		( 'X' )
	#define tskBLOCKED_CHAR		( 'B' )
	#define tskREADY_CHAR		( 'R' )
	#define tskDELETED_CHAR		( 'D' )
	#define tskSUSPENDED_CHAR	( 'S' )
	
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
