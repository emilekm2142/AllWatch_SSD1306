// AbstractInputInterpreter.h

#ifndef _ABSTRACTINPUTINTERPRETER_h
#define _ABSTRACTINPUTINTERPRETER_h
#include "flatbuffers_common_reader.h"
#include "Command_reader.h"
#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class AbstractInputInterpreter
{
 protected:


 public:
	void init();
};



#endif

