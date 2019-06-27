// SimpleRequestsApp.h

#ifndef _SIMPLEREQUESTSAPP_h
#define _SIMPLEREQUESTSAPP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "BuiltInApplication.h"
#include "Renderer.h"
#include "CustomScreen.h"
#include "Layout.h"
class SimpleRequestsApp : public BuiltInApplication
{
private:
	//TODO: genericmenuscreen z tego 
	class SimpleRequestsAppLayout : public Layout {
		SimpleRequestsApp* app;
		SimpleRequestsAppLayout(SimpleRequestsApp* app) {
			this->app = app;


		}
	};
	SimpleRequestsAppLayout* l;
public:
	SimpleRequestsApp(char* name, UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		this->layout = l;
		this->name = "";
	}
};



#endif

