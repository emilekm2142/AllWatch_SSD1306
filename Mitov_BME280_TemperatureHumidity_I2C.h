////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//     This software is supplied under the terms of a license agreement or    //
//     nondisclosure agreement with Mitov Software and may not be copied      //
//     or disclosed except in accordance with the terms of that agreement.    //
//         Copyright(c) 2002-2019 Mitov Software. All Rights Reserved.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef _MITOV_BME280_TEMPERATUREHUMIDITY_I2C_h
#define _MITOV_BME280_TEMPERATUREHUMIDITY_I2C_h

#include <Wire.h> //I2C Arduino Library
#include <Mitov_BME280_TemperatureHumidity.h>
#include <Mitov_Basic_I2C.h>

namespace Mitov
{
//---------------------------------------------------------------------------
	template<typename T_I2C, T_I2C &C_I2C, uint8_t C_ADDRESS> class BME280_TemperatureHumidityI2C_Impl
	{
	protected:
		uint8_t read8(uint8_t reg)
		{
			C_I2C.beginTransmission( C_ADDRESS );
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( C_ADDRESS, (uint8_t)1 );
			return C_I2C.read();
		}

		uint16_t read16(uint8_t reg)
		{
			C_I2C.beginTransmission( C_ADDRESS );
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( C_ADDRESS, (uint8_t)2 );
			uint16_t AValue = C_I2C.read();
			return ( AValue << 8 ) | C_I2C.read();
		}

		uint16_t read16_LE(uint8_t reg)
		{
			C_I2C.beginTransmission( C_ADDRESS );
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( C_ADDRESS, (uint8_t)2 );
			uint16_t AValue = C_I2C.read();
			return AValue | ( uint16_t( C_I2C.read() ) << 8 );
		}

		uint32_t read24(uint8_t reg)
		{
			C_I2C.beginTransmission( C_ADDRESS );
			C_I2C.write( reg );
			C_I2C.endTransmission();
			C_I2C.requestFrom( C_ADDRESS, (uint8_t)3 );
    
			uint32_t AValue = C_I2C.read();
			AValue <<= 8;
			AValue |= C_I2C.read();
			AValue <<= 8;
			AValue |= C_I2C.read();

			return AValue;
		}

		void write8(uint8_t reg, uint8_t value)
		{
			C_I2C.beginTransmission( C_ADDRESS );
			C_I2C.write((uint8_t)reg);
			C_I2C.write((uint8_t)value);
			C_I2C.endTransmission();
		}

	};
//---------------------------------------------------------------------------
	template<
		typename T_I2C, T_I2C &C_I2C,
		uint8_t C_ADDRESS,
		typename T_Enabled,
//		typename T_HumidityOutputPin,
		typename T_IIRFilterCoefficient,
		typename T_InFahrenheit,
		typename T_InactiveDuration,
//		typename T_Oversampling_Humidity,
		typename T_Oversampling_Pressure,
		typename T_Oversampling_Temperature,
		typename T_PressureOutputPin,
		typename T_TemperatureOutputPin
	> class BMP280_TemperaturePressure_I2C : 
		public BME280_TemperatureHumidity<
			BME280_TemperatureHumidityI2C_Impl<T_I2C, C_I2C, C_ADDRESS>, 
				T_Enabled,
				Mitov::AnalogPin_NoImplementation<88>,
				T_IIRFilterCoefficient,
				T_InFahrenheit,
				T_InactiveDuration,
				Mitov::ConstantProperty<66, uint32_t, 1>,
				T_Oversampling_Pressure,
				T_Oversampling_Temperature,
				T_PressureOutputPin,
				Mitov::ConstantProperty<3, bool, false>,
				T_TemperatureOutputPin
			>
	{
	};
//---------------------------------------------------------------------------
	template<
		typename T_I2C, T_I2C &C_I2C,
		uint8_t C_ADDRESS,
		typename T_Enabled,
		typename T_HumidityOutputPin,
		typename T_IIRFilterCoefficient,
		typename T_InFahrenheit,
		typename T_InactiveDuration,
		typename T_Oversampling_Humidity,
		typename T_Oversampling_Pressure,
		typename T_Oversampling_Temperature,
		typename T_PressureOutputPin,
		typename T_TemperatureOutputPin
	> class BME280_TemperatureHumidity_I2C : 
		public BME280_TemperatureHumidity<
			BME280_TemperatureHumidityI2C_Impl<T_I2C, C_I2C, C_ADDRESS>, 
				T_Enabled,
				T_HumidityOutputPin,
				T_IIRFilterCoefficient,
				T_InFahrenheit,
				T_InactiveDuration,
				T_Oversampling_Humidity,
				T_Oversampling_Pressure,
				T_Oversampling_Temperature,
				T_PressureOutputPin,
				Mitov::ConstantProperty<3, bool, false>,
				T_TemperatureOutputPin
			>
	{
	};
//---------------------------------------------------------------------------
}

#endif
