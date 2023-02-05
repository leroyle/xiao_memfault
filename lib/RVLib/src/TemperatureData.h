/*
 * TemperatureData.h
 *
 *  Created on: April 21, 2020
 *      Author: leroy
 */

#ifndef TEMPERATUREDATA_H_
#define TEMPERATUREDATA_H_

#define DHT11_Sensor
//#define DS18B20

#ifdef DHT11_Sensor
#include <DHT.h>
#endif

#ifdef DS18B20
//#include <OneWire.h>
#include <DallasTemperature.h>
#endif

#include "SensorData.h"
#include <core/MyMessage.h>


#ifdef DS18B20
#ifndef ONE_WIRE_BUS
#define ONE_WIRE_BUS 29
#endif
#ifndef TEMPERATURE_PRECISION
#define TEMPERATURE_PRECISION 9
#endif
#endif
class TemperatureData: public SensorData {

	public:
	
	typedef enum  {SENSOR_TYPE_DHT11, SENSOR_TYPE_DS18B20, SENSOR_STYLE_UNKNOWN} SensorStyle;
\
#ifdef DS18B20
	DeviceAddress deviceAddress;
#endif

	private:
	
	float _currentTemperature;
	float _lastTemperature;
	float _currentHumidity;
	float _lastHumidity;
	float _testTemperature;
	float _testHumidity;
	float _upperTempRange;
	float _lowerTempRange;
	mysensors_data_ext_t _readingReturnType;
	SensorStyle _sensorStyle;
#ifdef DS18B20
	DallasTemperature *_dsSensor = NULL;
	DeviceAddress * _dsThermometerAddr = NULL;
#endif
#ifdef DHT11_Sensor
	DHT *_dhtSensor = NULL;
#endif 

public:

	TemperatureData();
	virtual ~TemperatureData();
	void initSensorArray();
	float toFarenheit (TemperatureData * sensor);
	float toFarenheit ();
	float toCelcius();
	float calcFarenheitHeatIndex();
	float calcCelciusHeatIndex();

	void setLastTemperature(float value) {
		this->_lastTemperature = value;
	}

	float getLastTemperature() {
		return this->_lastTemperature;
	}

	float getCurrentTemperature() {
		return this->_currentTemperature;
	}

	float setCurrentTemperature(float value) {
		return this->_currentTemperature = value;
	}

	void setLastHumidity(float value) {
		this->_lastHumidity = value;
	}

	float getLastHumidity() {
		return this->_lastHumidity;
	}

	float getCurrentHumidity() {
		return this->_currentHumidity;
	}

	float setCurrentHumidity(float value) {
		return this->_currentHumidity = value;
	}

	void setTestTemperature(float value) {
		this->_testTemperature = value;
	}

	float getTestTemperature() {
		return this->_testTemperature;
	}

	void setTestHumidity(float value) {
		this->_testHumidity = value;
	}

	float getTestHumidity() {
		return this->_testHumidity;
	}

	void setSensorStyle(SensorStyle value) {
		this->_sensorStyle = value;
	}

	SensorStyle getSensorStyle() {
		return this->_sensorStyle;
	}

#ifdef DHT11_Sensor
	void setDHTSensor(DHT * value) {
		this->_dhtSensor = value;
	}

	DHT * getDHTSensor() {
		return this->_dhtSensor;
	}
#endif

#ifdef DS18B20
	void setDSSensor(DallasTemperature * value) {
		this->_dsSensor = value;
	}

	DallasTemperature * getDSSensor() {
		return this->_dsSensor;
	}

	void setDSDeviceAddr(DeviceAddress * value) {
		this->_dsThermometerAddr = value;
	}

	DeviceAddress * getDSDeviceAddr() {
	 	return this->_dsThermometerAddr;
	}
#endif


	void setUpperTempRange(float value) {
		// if(value < 0) {
		// 	this->_upperTempRange = 0;
		// } else {
		// 	this->_upperTempRange = value;
		// }
		this->_upperTempRange = value;
	}

	float getUpperTempRange() {
		return this->_upperTempRange;
	}

	void setLowerTempRange(float value) {
		// if(value < 0) {
		// 	this->_lowerTempRange = value;
		// } else {
		// 	this->_lowerTempRange = value;
		// }

		this->_lowerTempRange = value;

	}

	float getLowerTempRange() {
		return this->_lowerTempRange;
	}
	
	// V_LEVEL or V_PERCENTAGE
	mysensors_data_ext_t getReturnType () {
		return this->_readingReturnType;
	}

	// V_LEVEL or V_PERCENTAGE
	void setReturnType(mysensors_data_ext_t value) {
		this->_readingReturnType = value;		
	}

	void setReturnCelcius () {
        this->setReturnType(V_TEMP_TYPE_CELCIUS);
    }

    void setReturnFarenheit() {
        this->setReturnType(V_TEMP_TYPE_FAHRENHEIT);
    }

	void setReturnHumidity() {
        this->setReturnType(V_TEMP_TYPE_HUMIDITY);
    }

	void setReturnHeatIndexToFarenheit() {
        this->setReturnType(V_TEMP_TYPE_HEAT_INDEX_FARENHEIT);
    }

	void setReturnHeatIndexToCelcius() {
        this->setReturnType(V_TEMP_TYPE_HEAT_INDEX_CELCIUS);
    }

};

#endif /* TEMPERATUREDATA_H_ */

