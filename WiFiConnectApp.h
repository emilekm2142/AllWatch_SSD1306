// WiFiConnectApp.h

#ifndef _WIFICONNECTAPP_h
#define _WIFICONNECTAPP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include "BuiltInApplication.h"
#include "Renderer.h"
#include "GenericMenuScreen.h"
#include "GenericTextScreen.h"
#include "CustomScreen.h"
#include "DelayedAction.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Layout.h"
#include "SSD1306.h"

namespace WiFiConnectApp_Icon{
	const int width = 36;
	const int height = 36;
	static unsigned char icon_bits[] PROGMEM = {
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0xf0, 0xff, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x07, 0x00, 0x80, 0xff, 0xff,
	   0x1f, 0x00, 0xe0, 0xff, 0xff, 0x7f, 0x00, 0xf0, 0x1f, 0x80, 0xff, 0x00,
	   0xf8, 0x03, 0x00, 0xfc, 0x01, 0xfc, 0x00, 0x00, 0xf0, 0x03, 0x7c, 0x00,
	   0x00, 0xe0, 0x03, 0x38, 0x80, 0x1f, 0xc0, 0x01, 0x00, 0xf0, 0xff, 0x00,
	   0x00, 0x00, 0xfc, 0xff, 0x03, 0x00, 0x00, 0xfe, 0xff, 0x07, 0x00, 0x00,
	   0xff, 0xff, 0x0f, 0x00, 0x00, 0x3f, 0xc0, 0x0f, 0x00, 0x00, 0x0e, 0x00,
	   0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0xc0, 0x3f, 0x00, 0x00, 0x00, 0xc0,
	   0x3f, 0x00, 0x00, 0x00, 0x80, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00,
	   0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

}

class WiFiConnectApp:public BuiltInApplication
{
private:
	class WiFiConnectAppLayout:public CustomScreen {
	public:
		WiFiConnectApp* app;
		Layout* currentScreen; //TODO: move this to customScreen
		GenericTextScreen* infoScreen;
		GenericMenuScreen* choiceMenu;
		GenericMenuScreen* menu;
		Run::DelayedActionDataHolder* taskRef;
		WiFiConnectAppLayout(WiFiConnectApp* app) {
			this->app = app;
			infoScreen = new GenericTextScreen(this->app->UI, (char*)F("Connecting..."), true);
			choiceMenu = new GenericMenuScreen(app->UI);
			choiceMenu->AddOption("Disconnect", [this]() {
				infoScreen->text = (char*)F("Disconnecting...");
				currentScreen = (Layout*)infoScreen;
				Draw(*this->app->UI->GetRenderer());
				this->app->settingsManager->wifiManager->Disconnect();
				infoScreen->text = (char*)F("OK!");
				Draw(*this->app->UI->GetRenderer());
				Run::After(2000, [this]() {
					currentScreen = (Layout*)choiceMenu;
				});
			});
			choiceMenu->AddOption("Connect to any", [this]() {
				bool status = this->app->settingsManager->wifiManager->ConnectToFirstFittingWiFiNetwork();
				if (status) {
					infoScreen->text = (char*)F("OK!");
					Draw(*this->app->UI->GetRenderer());
					Run::After(2000, [this]() {
						currentScreen = (Layout*)choiceMenu;
					});
				}
				else {
					infoScreen->text = (char*)F("Could not connect to any network :(");
					Draw(*this->app->UI->GetRenderer());
					Run::After(2000, [this]() {
						currentScreen = (Layout*)choiceMenu;
					});
				}
			});
			choiceMenu->AddOption("List networks", [this]() {
				char** names = new char*[25];
				for (int x = 0; x < 25; x++) names[x] = NULL;
				taskRef = Run::After(100, [this, names]() {
					int len = this->app->settingsManager->wifiManager->ScanNetworks();
					menu->ClearList();
					for (int i = 0; i < len; i++) {
						if (names[i] != NULL)
							delete[] names[i];
						names[i] = new char[150];
						this->app->settingsManager->wifiManager->GetNetwork(i).toCharArray(names[i], 150);
						Serial.println(names[i]);
						menu->AddOption(names[i], [this, names, i]() {
							//make copy
							int d = i;
							infoScreen->text = PSTR("Connecting...");
							currentScreen = (Layout*)infoScreen;
							Draw(*this->app->UI->GetRenderer());
							Serial.println("...");
							bool saved = this->app->settingsManager->wifiManager->IsNetworkSaved(this->app->settingsManager->SPIFFS, names[i]);
							if (!saved) {
								infoScreen->text = PSTR("Type in password");
								
								this->app->UI->DisplayKeyboard();
								
								this->app->UI->RedrawAll();
								this->app->UI->GetKeyboard()->onType = [this] {
									
									infoScreen->text = this->app->UI->GetKeyboard()->target;
								};
								this->app->UI->GetKeyboard()->onClose = [this, names, i] {
									auto kbrd = this->app->UI->GetKeyboard();
									infoScreen->text = PSTR("Connecting...");
									this->app->UI->RedrawAll();
									this->app->settingsManager->wifiManager->AppendWiFiNetwork(this->app->settingsManager->SPIFFS, names[i], (char*)kbrd->target);
									int status = this->app->settingsManager->wifiManager->ConnectToWiFiUsingSavedCredentials(names[i]);
									Run::After(2000, [this, status, names, i]()
										{
											if (!status) {
												infoScreen->text = PSTR("Could not connect...");
												//remove the credentials then
												this->app->settingsManager->wifiManager->RemoveWiFiNetwork(names[i]);

											}
											else {
												infoScreen->text = PSTR("It is working!");

											}
											this->app->UI->RedrawAll();
											Run::After(2000, [this, status]()
												{currentScreen = (Layout*)menu;


												});
										});
								};
							}
							else {
								int status = this->app->settingsManager->wifiManager->ConnectToWiFiUsingSavedCredentials(names[d]);
								Run::After(2000, [this, status]()
									{

										currentScreen = (Layout*)menu;

									});
							}
						
						});
					}
				});
				currentScreen = (Layout*)menu;
				Draw(*this->app->UI->GetRenderer());
			});
			menu = new GenericMenuScreen(app->UI);
			menu->setOnBackCallbackAction([this]() {Run::Cancel(taskRef); this->currentScreen = (Layout*)choiceMenu; });
			
			currentScreen = (Layout*)choiceMenu;
			

		}
		void Draw(Renderer& r) {
			currentScreen->Draw(r);
		}
		void Back(Renderer& r) override {
			this->app->Exit();
		}
		void Up(Renderer& r) override {
			currentScreen->Up(r);
		}
		void Down(Renderer& r) override {
			currentScreen->Down(r);
		}
		void Ok(Renderer& r) override {
			currentScreen->Ok(r);
		}
	};
 protected:
	 WiFiConnectAppLayout* l = new WiFiConnectAppLayout(this);

 public:


	 WiFiConnectApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		 this->layout = l;
		 this->name = "WiFi";
	 }

	
	 void OnOpen() override {





	 }
	 void OnExit() override {
		 Run::Cancel(this->l->taskRef);
	 }


};

#endif

