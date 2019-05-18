// BuiltInApplication.h

#ifndef _BUILTINAPPLICATION_h
#define _BUILTINAPPLICATION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "Layout.h"
#include "UserInterface.h"
#include "SettingsManager.h"
class BuiltInApplication
{
 protected:
	 SettingsManager* settingsManager;
	 UserInterfaceClass* UI;
 public:
	
	  char* name;
	 Layout* layout;

	 BuiltInApplication(Layout* layout, UserInterfaceClass* UI, SettingsManager* sm) {
		 this->layout = layout;
		 this->UI = UI;
		 this->settingsManager = sm;
		 this->settingsManager->CreateBuildInApplication(name, this);
	 }
	 void Open() {
		 OnOpen();
		 this->UI->OpenChildLayout(layout);
	 }
	 virtual void OnOpen()  {

	 }
	 void Exit() {
		 Serial.println("Exiting app");
		 OnExit();
		 this->UI->ShowLayout(*this->UI->GetMainLayout());
	 }
	 virtual void OnExit() {

	 }
	
};



#endif

