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
#include "Config.h"
#include "Barometer.h"
#include "Thermometer.h"
#define SEALEVELPRESSURE_HPA (1013.25)
class BME280Barometer:public Barometer, public Thermometer
{

	

 public:
	 float baseHeight = SEALEVELPRESSURE_HPA;

	 Adafruit_BME280 bme;
	 BME280Barometer() {
	 
		 Wire.begin(RTC_SDA, RTC_SCL);
		 bme.begin(118);
		 delay(100);
		 
		// bme->begin();
	}
	 float getPressure() override {
		 Serial.printf("\n Reading pressure from a barometer: %.1f", bme.readPressure());
		 return bme.readPressure() / 100.0F;
	}
	 float getHeight() override {
		 return bme.readAltitude(baseHeight);
	 }
	 float getTemperatureC() override {

	 	//Readings are a bit too high. 
		 Serial.printf("\n Reading tmp from a barometer: %.1f", bme.readTemperature()-3);
		 return bme.readTemperature()-3;
	 }
	void sleep() override
	 {
		
	 }

};


#endif

