/*
 * ValveData1.cpp
 *
 *  Created on: Nov 21, 2019
 *      Author: leroy
 */

#include <TemperatureData.h>

#ifdef DS18B20

// Data wire is plugged into port 2 on the Arduino


// // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
// static OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
// DallasTemperature DS_Sensors(&oneWire);

// // arrays to hold device addresses
// DeviceAddress insideThermometer, outsideThermometer;

#endif
TemperatureData::TemperatureData() {
	// TODO Auto-generated constructor stub
	this->initSensorArray();
}

TemperatureData::~TemperatureData() {
	// TODO Auto-generated destructor stub
}

void TemperatureData::initSensorArray() {
	// do we need to rerun this??
	SensorData::initSensorArray();
	this->_readingReturnType = V_TEMP_TYPE_FAHRENHEIT;
	this->_currentTemperature = 0;
	this->_lastTemperature = 0;
	this->_testTemperature = 0;
	this->_currentHumidity = 0;
	this->_lastHumidity = 0;
	this->_testHumidity = 0;
	this->_upperTempRange = 1023;
	this->_lowerTempRange = 1;
	this->_sensorStyle = SENSOR_STYLE_UNKNOWN;
#ifdef DS18B20
	this->_dsSensor = NULL;
	this->_dsThermometerAddr = NULL;
#endif
	this->_dhtSensor = NULL;
	
#ifdef DS18B20
	this->setDSDeviceAddr(&deviceAddress);
#endif
	
}

float TemperatureData::toFarenheit () {
	return (this->getLastTemperature() * 9/5) + 32;
};

float TemperatureData::toFarenheit (TemperatureData * sensor) {
	if (sensor->getReturnType() == V_TEMP_TYPE_FAHRENHEIT) {
		return sensor->getLastTemperature();
	}
	return (sensor->getLastTemperature() * 9/5) + 32;
};

float TemperatureData::toCelcius() {
	return this->getLastTemperature();
}

float TemperatureData::calcFarenheitHeatIndex() {
	// our temp readings are all in Celcius, we convert 
	// when the cloud requests
	return calcCelciusHeatIndex();
};

float TemperatureData::calcCelciusHeatIndex() {

	if (this->getDHTSensor() == NULL)
	{
		return NAN;
	}
	// calculate using DHT 
	DHT * _dht = this->getDHTSensor();
	if (_dht == NULL) {
		return NAN;
	}

	return _dht->computeHeatIndex(this->getLastTemperature(), this->getLastHumidity(), false);

};
	

