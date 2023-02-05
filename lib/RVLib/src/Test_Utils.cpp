/*
 * TUtils.cpp
 *
 *  Created on: Aug 26, 2018
 *      Author: leroy
 */
#ifdef UNIT_TEST
#include "Test_Utils.h"

#include <Arduino.h>



#include "core/MyMessage.h"


TestUtils::TestUtils() {
	// TODO Auto-generated constructor stub

}

TestUtils::~TestUtils() {
	// TODO Auto-generated destructor stub
}

static void pinMode(int pinNumber, int mode ) {

	Serial.print(F("Setting PinMode Pin: "));
	Serial.print(pinNumber);
	Serial.print(F(" Mode: "));
	if (mode == OUTPUT) {
		Serial.println(F("Output"));
	} else if (mode == INPUT) {
		Serial.println(F("Input"));
	} else {
		Serial.println(mode);
	}

	pinMode(pinNumber, mode);

}

/*
static void present(int id, int type) {
	Serial.print(F("present() API was sent: "));
	char dbgMsgBuf[200];
	sprintf(dbgMsgBuf,"%d, %d", id, type);
	Serial.println(dbgMsgBuf);
	wait(2000);
}
*/
void TestUtils::sendSketchInfo(char * msg1, char * msg2) {
	Serial.print(F("TestUtils::sendSketchInfo() API was called: "));
	Serial.print(msg1);
	Serial.print(F("  "));
	Serial.println(msg2);
	delay(2000);
}

void TestUtils::callPresentation() {
	//presentation();
}

// Only use in test case
// void TestUtils::sleep(uint32_t time) {
// 	delay(time);
// }
/* moved to MyUtils class as we need them for MySensor testing
uint8_t TestUtils::protocolH2i(char c)
{
	uint8_t i = 0;
	if (c <= '9') {
		i += c - '0';
	} else if (c >= 'a') {
		i += c - 'a' + 10;
	} else {
		i += c - 'A' + 10;
	}
	return i;
}


bool TestUtils::protocolParse(MyMessage &message, char *inputString)
{
	Serial.println(F("In TestUtils::protocolParse"));
	char *str, *p, *value=NULL;
	uint8_t bvalue[MAX_PAYLOAD];
	uint8_t blen = 0;
	int i = 0;
	uint8_t command = 0;

	// Extract command data coming on serial line
	for (str = strtok_r(inputString, ";", &p); // split using semicolon
			str && i < 6; // loop while str is not null an max 5 times
			str = strtok_r(NULL, ";", &p) // get subsequent tokens
	) {
		switch (i) {
		case 0: // Radio id (destination)
			message.destination = atoi(str);
			break;
		case 1: // Child id
			message.sensor = atoi(str);
			break;
		case 2: // Message type
			command = atoi(str);
			mSetCommand(message, command);
			break;
		case 3: // Should we request ack from destination?
			mSetRequestAck(message, atoi(str)?1:0);
			break;
		case 4: // Data type
			message.type = atoi(str);
			break;
		case 5: // Variable value
			if (command == C_STREAM) {
				while (*str) {
					uint8_t val;
					val = protocolH2i(*str++) << 4;
					val += protocolH2i(*str++);
					bvalue[blen] = val;
					blen++;
				}
			} else {
				value = str;
				// Remove trailing carriage return and newline character (if it exists)
				uint8_t lastCharacter = strlen(value)-1;
				if (value[lastCharacter] == '\r') {
					value[lastCharacter] = 0;
				}
				if (value[lastCharacter] == '\n') {
					value[lastCharacter] = 0;
				}
			}
			break;
		}
		i++;
	}
	// Check for invalid input
	if (i < 5) {
		Serial.println(F("parse FALSE LESS THAN 5"));
		return false;
	}
	message.sender = -1;
	message.last = -1;
	mSetAck(message, false);
	if (command == C_STREAM) {
		message.set(bvalue, blen);
	} else {
		message.set(value);
	}
	Serial.println("parse TRUE");
	return true;
}

//char myStr[] = "100;10;1;0;27;55";
// See if there is input to test the receive
bool TestUtils::checkInput(MyMessage& msgObj) {

#ifdef LAL
	const char * cmd_table[] = {
		"106;10;1;0;2;1",     // 0   (water sensor #10 on, no ack)
		"106;10;1;0;2;0",     // 1   (water sensor #10 off, no ack)
		"106;20;1;1;2;1",     // 2   (water sensor #20 on, ack)
		"106;20;1;1;2;0",     // 3   (water sensor #20 off, ack)
		"106;99;1;1;27;220",  // 4   (set delay to 220 sensor number is ignored, ack)
		"106;99;1;1;27;567",  // 5   (set delay to 567 sensor number is ignored, ack)
		"106;30;1;1;24;1",    // 6   (sensor 10, set min value)
		"106;40;1;1;25;100",  // 7   (sensor 10, set max value)
		"106;10;1;1;26;0"     // 8   (sensor 10, reset/clear Alarm)
	};

#else
	const char  str_0[] PROGMEM = "106;10;1;0;2;1";     // 0   (water sensor #10 on, no ack)
	const char  str_1[] PROGMEM = "106;10;1;0;2;0";     // 1   (water sensor #10 off, no ack)
	const char  str_2[] PROGMEM = "106;20;1;1;2;1";     // 2   (water sensor #20 on, ack)
	const char  str_3[] PROGMEM = "106;20;1;1;2;0";     // 3   (water sensor #20 off, ack)
	const char  str_4[] PROGMEM = "106;99;1;1;27;220";  // 4   (set delay to 220 sensor number is ignored, ack)
	const char  str_5[] PROGMEM = "106;99;1;1;27;567";  // 5   (set delay to 567 sensor number is ignored, ack)
	const char  str_6[] PROGMEM = "106;30;1;1;24;1";    // 6   (sensor 10, set min value)
	const char  str_7[] PROGMEM = "106;40;1;1;25;100";  // 7   (sensor 10, set max value)
	const char  str_8[] PROGMEM = "106;10;1;1;26;0";     // 8   (sensor 10, reset/clear Alarm)

	PROGMEM const char *cmd_table[] = {
			str_0,
			str_1,
			str_2,
			str_3,
			str_4,
			str_5,
			str_6,
			str_7,
			str_8
	};
#endif
	String readString;
	char pStr[20];

	Serial.println(F("In TestUtils::checkInput"));
	Serial.println(F("5 seconds to input data"));
	wait(5000);

	while (Serial.available()) {
		wait(3);  //delay to allow buffer to fill
		if (Serial.available() >0) {
			char c = Serial.read();  //gets one byte from serial buffer
			readString += c; //makes the string readString
		}
	}

	// TODO: test this
	if (readString.length() == 1) {
		Serial.print(F("Input: "));
		Serial.println(readString); //see what was received
		//packMsg( readString);
		int pos = atoi(readString.c_str());

		strcpy(pStr, cmd_table[pos]);
		if( protocolParse(msgObj, pStr)) {
			Serial.println(F("Good Parse"));
			return true;
		} else {
			Serial.println(F("Bad Parse"));
		}
	}
	return false;
}
*/
#endif

