// SimpleMenuRecord.h

#ifndef _SIMPLEMENURECORD_h
#define _SIMPLEMENURECORD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
struct SimpleMenuRecord {
	char* name;
	std::function<void()> callback;
};

#endif

