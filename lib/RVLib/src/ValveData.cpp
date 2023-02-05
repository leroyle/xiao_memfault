/*
 * ValveData1.cpp
 *
 *  Created on: Nov 21, 2019
 *      Author: leroy
 */
#include <ValveData.h>

ValveData::ValveData() {
	// TODO Auto-generated constructor stub
	this->initSensorArray();
}

ValveData::~ValveData() {
	// TODO Auto-generated destructor stub
}

void ValveData::initSensorArray() {
	SensorData::initSensorArray();
	this->_valveOnOffState = VALVE_OFF;
	this->_lastSentValveOnOffState = VALVE_OFF;
	this->_onPin = 0;
	this->_offPin = 0;
}

ValveOnOffState_t ValveData::waterValveStr2enum (const char *str)
{
    uint8_t j;
	uint8_t arraySize = sizeof (waterValveDefines) / sizeof (waterValveDefines[0]);
    for (j = 0;  j < arraySize;  ++j) {
        if (!strcmp (str, waterValveDefines[j].str)) {
            return waterValveDefines[j].val;    
		}
	}
	Serial.println ("Unknown Valve command. Should be ValveOn or ValveOff");
	return VALVE_UNKNOWN;
};

const char * ValveData::waterValveEnum2Str (ValveOnOffState_t enumVal)
{
	size_t numberOfElements = sizeof(waterValveDefines)/sizeof(waterValveDefines[0]);
	if (enumVal < numberOfElements - 1) {
		return waterValveDefines[enumVal].str;;
	}
	return waterValveDefines[numberOfElements - 1].str;
};