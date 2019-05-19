// SSH1106Renderer.h

#ifndef _SSH1106RENDERER_h
#define _SSH1106RENDERER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SSH1106RendererClass
{
 protected:


 public:
	void init();
};

extern SSH1106RendererClass SSH1106Renderer;

#endif

