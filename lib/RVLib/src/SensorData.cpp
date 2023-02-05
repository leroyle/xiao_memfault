/*
 * SensorData1.cpp
 *
 *  Created on: Aug 31, 2018
 *      Author: leroy
 */
#include <SensorData.h>

SensorData::SensorData() {
	initSensorArray();
}

SensorData::~SensorData() {

}

void SensorData::initSensorArray() {
	
	this->setAlarmState(AlarmDisabled);
	this->setLastSentAlarmState(AlarmDisabled);
	this->setName("NOT_SET");
	this->setDescription("NOT_SET");
	this->setSerialNumber(0xFF);
	this->setNodeType(NODE_TYPE_UNKNOWN);
	this->setSensorId(-1);
	this->setType(-1);
	this->setPowerPin(-1);
	this->setReadPin(-1);
	this->setLedPin(-1);
	this->setMode(-1);
	this->setCurVal(-1);
	this->setMinVal(-1);
	this->setMaxVal(-1);
	this->setTestReading(-1);
	this->setLastReading(-1);
	this->_direction = 0;
	this->setDefaultPercentageFactor(1);
	this->setReadsPerMeasurment((int8_t) 1);
	this->setReturnType(V_TEMP);
	this->disableSensor();
	this->setNodeChildId(-1);
#ifdef ENABLE_PERIODIC_TEST_MSG
	this->_testSendTestMsg = false;	
#endif
}

alarmState_t SensorData::waterAlarmStr2Enum (const char *str)
{
    uint8_t j;
	uint8_t arraySize = sizeof (waterAlarmDefines) / sizeof (waterAlarmDefines[0]);
    for (j = 0;  j < arraySize;  ++j) {
        if (!strcmp (str, waterAlarmDefines[j].str)) {
            return waterAlarmDefines[j].val;    
		}
	}
	return AlarmStateUnknown;
};

const char * SensorData::waterAlarmEnum2Str (alarmState_t enumVal)
{
	size_t numberOfElements = sizeof(waterAlarmDefines)/sizeof(waterAlarmDefines[0]);

	if (enumVal < numberOfElements - 1) {
		return waterAlarmDefines[enumVal].str;;
	}
	return waterAlarmDefines[numberOfElements - 1].str;
};

sensorEnabled_t SensorData::sensorEnabledStr2Enum (const char *str)
{
    uint8_t j;
	uint8_t arraySize = sizeof (sensorEnabledDefines) / sizeof (sensorEnabledDefines[0]);
    for (j = 0;  j < arraySize;  ++j) {
        if (!strcmp (str, sensorEnabledDefines[j].str)) {
            return sensorEnabledDefines[j].val;    
		}
	}
	return sensorEnabledDefines[arraySize - 1].val;
};

const char * SensorData::sensorEnabledEnum2Str (sensorEnabled_t enumVal)
{
	size_t numberOfElements = sizeof(sensorEnabledDefines)/sizeof(sensorEnabledDefines[0]);

	if (enumVal < numberOfElements - 1) {
		return sensorEnabledDefines[enumVal].str;;
	}
	return sensorEnabledDefines[numberOfElements - 1].str;
};

const char * SensorData::getAlarmStateString() {
		return waterAlarmEnum2Str(this->_alarmState);
};