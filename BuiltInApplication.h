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
private:
	bool deleteOnExit = true;
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
	 }
	 void Open() {
		 Serial.println("opening app");
		 OnOpen();
		 Serial.println("after onOpen()");
		 this->UI->OpenChildLayout(layout);
	 }
	 virtual void OnOpen()  {

	 }
	 void Exit() {
		 Serial.println("Exiting app");
		 OnExit();
		 this->UI->ShowLayout(*this->UI->GetMainLayout());
		 if (deleteOnExit) delete this;
	 }
	 virtual void OnExit() {

	 }
	 void UpdateKeyInConfig(char* key, char* value){
		 this->settingsManager->appsManager->UpdateKeyInConfig(this->name, key, value);
	 }
	bool KeyExists(char* key) {
		return this->settingsManager->appsManager->KeyExists(this->name, key);
	 }
	void AppendKey(char* key, char* value) {
		settingsManager->appsManager->AppendKeyToConfig(this->name, key, value);
	}
	
};



#endif

