// BME280Barometer.h

#ifndef _BME280BAROMETER_h
#define _BME280BAROMETER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "Barometer.h"
#include "Thermometer.h"
#define SEALEVELPRESSURE_HPA (1013.25)
class BME280Barometer:public Barometer, public Thermometer
{

	

 public:
	 float baseHeight = SEALEVELPRESSURE_HPA;
	 Adafruit_BME280* bme;
	 BME280Barometer() {
		 bme = new Adafruit_BME280();
		 bme->begin();
	}
	 float getPressure() override {
		 return bme->readPressure() / 100.0F;
	}
	 float getHeight() override {
		 return bme->readAltitude(baseHeight);
	 }
	 int getTemperatureC() override {
		 return bme->readTemperature();
	 }

};


#endif

