// IFTTApp.h

#ifndef _IFTTAPP_h
#define _IFTTAPP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "BuiltInApplication.h"
#include "Renderer.h"
#include "CustomScreen.h"
#include "GenericMenuScreen.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Layout.h"
#include "SSD1306.h"
#include "string.h"
#include "SettingsScreen.h"
class MenuPositionDataHolder {
public:
	char* name;
	char* action;

	MenuPositionDataHolder(char* name, char* action) {
		this->name = (char*)calloc(sizeof(char), sizeof(char)*strlen(name));
		this->action = (char*)calloc(sizeof(char), sizeof(char)*strlen(action));

		strcpy(this->name, name);
		strcpy(this->action, action);
	}
	~MenuPositionDataHolder() {
		free(this->name);
		free(this->action);
	}
};
class IFTTApp :public BuiltInApplication
{
	class IFTTTMainLayout :public CustomScreen {
	public:

		IFTTApp* app;
		GenericMenuScreen* mainMenu;
		IFTTTMainLayout(IFTTApp* app) {
			this->app = app;
			mainMenu = new GenericMenuScreen(this->app->UI);

			//Loading from config
		

		}
		void Draw(Renderer& r) override {
			mainMenu->Draw(r);
		}
		void Back(Renderer& r) override {
			//app->UI->ReturnToParentLayout();
			this->app->Exit();
		}
		void Down(Renderer& r) override {
	
			mainMenu->Down(r);
		}
		void Up(Renderer& r) override {
			mainMenu->Up(r);
		}
		void Ok(Renderer& r) override {
			mainMenu->Ok(r);
		}
	};
protected:
	IFTTTMainLayout* l;
	char key[30];

public:
	IFTTApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		Serial.println("Welcome to IFTTT app!");
		this->l = new IFTTTMainLayout(this);
		this->layout = l;
		this->name = "IFTTT";
		this->UI = UI;
		
		l->mainMenu->AddOption((char*)F("Add more..."), [this] {
			SettingsScreen* settingsScreen = new SettingsScreen(this->UI, settingsManager);
			settingsManager->OpenSettings();
			settingsScreen->UI = this->UI;
			this->UI->OpenChildLayout((Layout *)(settingsScreen));
		
		});
		char buffer[50];
		GetKeyValue("key", &buffer[0]);
		if (this->KeyExists("key") && strcmp("type in the api key",buffer ) == 0) {
			l->mainMenu->AddOption((char*)F("Cchange the API key!"), [this] {
				SettingsScreen* settingsScreen = new SettingsScreen(this->UI, settingsManager);
				settingsManager->OpenSettings();
				settingsScreen->UI = this->UI;
				this->UI->OpenChildLayout((Layout *)(settingsScreen));

			});
		}
		auto configFile = sm->appsManager->GetConfigForApplication("IFTTT");
		while (configFile.available()) {
			Serial.println("Next char...");
			char name[50];
			int sizeName = configFile.readBytesUntil('=', &name[0], 50);
			name[sizeName] = '\0';
			Serial.printf("Content: %s=", name);
			char value[50];
			int valueSize = configFile.readBytesUntil('\n', &value[0], 50);
			value[valueSize] = '\0';
			Serial.printf("Content: =%s",  value);
			if (strcmp(name, "key") != 0) {
			   	AddMenuOption(name);
			}
			else {
				strcpy(key, (const char*)&value[0]);
			}
			

		}
		Serial.println("CLOSING FILE");
		configFile.close();

	//	l->mainMenu->AddOption("Margarita", [this]() {this->app->Exit(); });
	//	l->mainMenu->AddOption("Fungi", [this]() {this->app->Exit(); });
	//	l->mainMenu->AddOption("Vesuvio", [this]() {this->app->Exit(); });

	}
	void OnOpen() override {
		Serial.println("IFTTT opened");
	}
	void OnExit() override {
		
	}
	
	void AddMenuOption(char* name) {
		char* j = new char[strlen(name) + 1];
		Serial.println("Copy...2");
		strcpy(j, name);
		l->mainMenu->AddOption(j, [this, j]() {DoWebhookRequest(j); }, true);
		Serial.println("Added...");
	}
	void DoWebhookRequest(char* h) {
		char buffer[150];
		char value[100];
		GetKeyValue(h, value);
		sprintf(buffer, "https://maker.ifttt.com/trigger/%s/with/key/%s", value, key);
		Serial.println("Sending request to:");
		Serial.println(buffer);
		auto d = settingsManager->http->MakeGetRequest(buffer);
		if (d == nullptr) {
			//TODO: SHOW WARNING SCREEN...
		}else{
			Serial.println(d->getString());

			settingsManager->http->EndRequest(d);
		}
	
	}
};



#endif

