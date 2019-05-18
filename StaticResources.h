// StaticResources.h

#ifndef _STATICRESOURCES_h
#define _STATICRESOURCES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace StaticResources8266SSD1306 {
	const unsigned char activeSymbol[] PROGMEM = {
	B00000000,
	B00000000,
	B00011000,
	B00100100,
	B01000010,
	B01000010,
	B00100100,
	B00011000
	};

	const unsigned char inactiveSymbol[] PROGMEM = {
		B00000000,
		B00000000,
		B00000000,
		B00000000,
		B00011000,
		B00011000,
		B00000000,
		B00000000
	};
}



#endif

