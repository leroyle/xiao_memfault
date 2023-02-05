/*
 * SensorData1.h
 *
 *  Created on: Aug 31, 2018
 *      Author: leroy
 */

#ifndef SENSORDATA_H_
#define SENSORDATA_H_

#include "Arduino.h"
#include "string.h"
#include <SensorDefs.h>
#include <core/MyMessage.h>

class SensorData {

	public:
	
	private:
	alarmState_t _alarmState;
	alarmState_t _lastSentAlarmState;
	char _name[SENSOR_NAME_MAX];
	char _description[SENSOR_DESCRIPTION_MAX];
	uint32_t _serialNumber;
	// char _nodeType[SENSOR_TYPE_MAX];
	NODE_TYPE _nodeType;
	char _childSendValueBuff[100];
	int8_t _sensorId;
	int8_t _type;
	int8_t _powerPin;
	int8_t _readPin;
	int8_t _ledPin;
	int8_t _mode;
	float  _curFloatVal;
	int16_t _minVal;
	int16_t _maxVal;
	int16_t _lastReading;
    int16_t _testReading;
	int8_t _direction;  // 0 use primaryPin, 1 use secondaryPin
   	float _defaultPercentageFactor = 0;
    int8_t _readsPerMeasurment;
    mysensors_data_t _readingReturnType;   // V_LEVEL, V_PERCENTAGE;
	sensorEnabled_t _sensorEnabled;
	int8_t _nodeChildId;  // deprecate

#ifdef ENABLE_PERIODIC_TEST_MSG
	bool _testSendTestMsg;
	uint32_t _testLastsendTestMsgTime;
	uint32_t _testSendMsgInterval;
#endif


public:

	SensorData();
	virtual ~SensorData();
	void initSensorArray();
	static alarmState_t waterAlarmStr2Enum (const char *str);
	const char * waterAlarmEnum2Str (alarmState_t enumVal);
	const char * getAlarmStateString();
	sensorEnabled_t sensorEnabledStr2Enum (const char *str);
	const char * sensorEnabledEnum2Str (sensorEnabled_t enumVal);

	alarmState_t getAlarmState() const {
		return this->_alarmState;
	}

	void setAlarmState(alarmState_t alarmState) {
		this->_alarmState = alarmState;
	}

	alarmState_t getLastSentAlarmState() const {
		return this->_lastSentAlarmState;
	}

	void setLastSentAlarmState(alarmState_t alarmState) {
		this->_lastSentAlarmState = alarmState;
	}

	bool isSensorEnabled() const {
		if(this->_sensorEnabled == ENABLED) {
			return true;
		} else {
			return false;
		}
	}

	sensorEnabled_t getSensorEnableState() {
		return this->_sensorEnabled;
	}

	void enableSensor () {
		this->_sensorEnabled = ENABLED;
	}

	void disableSensor() {
		this->_sensorEnabled = DISABLED;
	}

	void setSensorEnable(sensorEnabled_t value) {
		if (value == ENABLED || value == DISABLED) {
			this->_sensorEnabled = value;
		}
	}

	// void setSensorEnabled(const char * value) {
	// 	if (!strcmp(value, "ON")) {
	// 			this->_sensorEnabled = _ON;
	// 	} else if (!strcmp(value, "OFF")) {
	// 		this->_sensorEnabled = _OFF;
	// 	}
	// }

	int8_t getSensorId() const {
		return this->_sensorId;
	}

	void setSensorId(int8_t id) {
		this->_sensorId = id;
	}

	int16_t getMaxVal() const {
		return this->_maxVal;
	}

	void setMaxVal(int16_t maxVal) {
		if (maxVal < 0){
			this->_maxVal = 0;
		} else {	
			this->_maxVal = maxVal;
		}
	}

	int16_t getMinVal() const {
		return this->_minVal;
	}

	void setMinVal(int16_t minVal) {
		if (minVal < 0){
			this->_minVal = 0;
		} else {	
			this->_minVal = minVal;
		}
	}

	int8_t getMode() const {
		return this->_mode;
	}

	void setMode(int8_t mode) {
		this->_mode = mode;
	}

	const char* getName() const {
		return this->_name;
	}
	
	void setName(const char * name) {
		memset(this->_name, '\0', sizeof(this->_name));
		strncpy(this->_name, name, SENSOR_NAME_MAX - 1);
	}

	uint32_t getSerialNumber() const {
		return this->_serialNumber;
	}
	
	void setSerialNumber(uint32_t value) {
		// memset(this->_serialNumber, '\0', sizeof(this->_serialNumber));
		// strncpy(this->_serialNumber, value, SENSOR_SERIAL_NUMBER_MAX - 1);
		_serialNumber = value;
	}

	NODE_TYPE getNodeType() const {
		return this->_nodeType;
	}
	
	void setNodeType(NODE_TYPE value) {
		// memset(this->_nodeType, '\0', sizeof(this->_nodeType));
		// strncpy(this->_nodeType, value, SENSOR_TYPE_MAX - 1);
		_nodeType = value;
	}

	const char* getChildSendValueBuff() const {
		return this->_childSendValueBuff;
	}

	int8_t getPowerPin() const {
		return this->_powerPin;
	}

	void setPowerPin(int8_t pin) {
		this->_powerPin = pin;
	}

	int8_t getReadPin() const {
		return this->_readPin;
	}

	void setReadPin(int8_t pin) {
		this->_readPin = pin;
	}

	int8_t getLedPin() const {
		return this->_ledPin;
	}

	void setLedPin(int8_t pin) {
		this->_ledPin = pin;
	}

	int8_t getType() const {
		return this->_type;
	}

	void setType(int8_t type) {
		this->_type = type;
	}

	void setDescription(const char * description) {
		memset(this->_description, '\0', sizeof(this->_description));
			strncpy(this->_description, description, SENSOR_DESCRIPTION_MAX - 1);
	}

	const char* getDescription() const {
		return this->_description;
	}


	float getCurVal() const {
		return this->_curFloatVal;
	}

	void setCurVal(float curVal) {
		this->_curFloatVal = curVal;
	}

	void setNodeChildId (int8_t id) {
		this->_nodeChildId = id;
	}

	int8_t getNodeChildId () {
		return this->_nodeChildId;
	}

	void setLastReading(int16_t value) {
        this->_lastReading = value;
    }

	int16_t getLastReading() {
			return this->_lastReading;
	}

	void setTestReading(int16_t value) {
			this->_testReading = value;
	}

	int16_t getTestReading() {
			return this->_testReading;
	}

	void setReadsPerMeasurment(int8_t value) {
		if(value < 0) {
				value = 1;  // default at least one read
		}
		this->_readsPerMeasurment = value;
	}

	int8_t getReadsPerMeasurment() {
		return this->_readsPerMeasurment;
	}

 	// V_LEVEL or V_PERCENTAGE
        mysensors_data_t getReturnType () {
                return this->_readingReturnType;
        }

        // V_LEVEL or V_PERCENTAGE
        void setReturnType(mysensors_data_t value) {
                this->_readingReturnType = value;
        }

        void setReturnValue () {
        this->setReturnType(V_LEVEL);
    }

    void setReturnPercentage() {
        this->setReturnType(V_PERCENTAGE);
    }

    void setDefaultPercentageFactor(float value) {
		this->_defaultPercentageFactor = value;
	}

	float getDefaultPercentageFactor() {
		return this->_defaultPercentageFactor;
	}

#ifdef ENABLE_PERIODIC_TEST_MSG
	bool getTestSendTestMsg() {
		return this->_testSendTestMsg;
	}

	bool setTestSendTestMsg(uint32_t value) {
		this->_testSendTestMsg = value;
	}

	uint32_t getTestLastSendTestMsgTime() {
		return this->_testLastsendTestMsgTime;
	}

	uint32_t setTestLastSendTestMsgTime(uint32_t value) {
		this->_testLastsendTestMsgTime = value;
	}

	uint32_t getTestSendMsgInterval() {
		return this->_testSendMsgInterval;
	}

	uint32_t setTestSendMsgInterval(uint32_t value) {
		this->_testSendMsgInterval = value;
	}
#endif   // ENABLE_PERIODIC_TEST_MSG

#ifdef UNIT_TEST  // only for UNIT_TEST
                void setMoistureLevel_Test(int16_t value) {
                        if (value < 0) {
                                _testReading = 0;
                        } else {
                                _testReading = value;
                        }
                }
#endif // UNIT_TEST

};



#endif /* SENSORDATA_H_ */
