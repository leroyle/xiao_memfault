/*
 * ValveData.h
 *
 *  Created on: Nov 21, 2019
 *      Author: leroy
 */

#ifndef VALVEDATA_H_
#define VALVEDATA_H_

#include "SensorData.h"

class ValveData: public SensorData {

	public:
	
	private:
	ValveOnOffState_t _valveOnOffState;
	ValveOnOffState_t _lastSentValveOnOffState;
	uint8_t _onPin;
	uint8_t _offPin;

public:

	ValveData();
	virtual ~ValveData();
	void initSensorArray();

	ValveOnOffState_t waterValveStr2enum (const char *str);
	const char * waterValveEnum2Str (ValveOnOffState_t enumVal);

	bool isValveOn()  {
		if (this->_valveOnOffState ==  VALVE_ON)
		{
			return true;
		}
		return false;
	}

	void setValveStateOn() {
		setValveState(VALVE_ON);
	}

	void setValveStateOff() {
		setValveState(VALVE_OFF);
	}

	void setValveState (ValveOnOffState_t state)
	{
		this->_valveOnOffState = state;
	}

	ValveOnOffState_t getValveState() {
		return this->_valveOnOffState;
	}

	void setLastSentValveState (ValveOnOffState_t state)
	{
		this->_lastSentValveOnOffState = state;
	}

	ValveOnOffState_t getLastSentValveState() {
		return this->_lastSentValveOnOffState;
	}

	void setOnPin (uint8_t pin) {
		this-> _onPin = pin;
	}

	uint8_t getOnPin () {
		return this->_onPin;
	}

	void setOffPin ( uint8_t pin) {
		this-> _offPin = pin;
	}

	uint8_t getOffPin () {
		return this->_offPin;
	}

};
#endif /* VALVEDATA_H_ */
