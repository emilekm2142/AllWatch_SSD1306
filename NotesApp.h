#pragma once

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

namespace NotesApp_Icon {
	const int width = 36;
	const int height = 36;
	static unsigned char icon_bits[] PROGMEM = {
	   0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0x3f, 0x00, 0xf8, 0xff,
   0xff, 0xff, 0x00, 0x1c, 0x00, 0x00, 0xc0, 0x01, 0x0c, 0x10, 0x00, 0x80,
   0x01, 0x06, 0x10, 0x00, 0x00, 0x03, 0x06, 0x10, 0x00, 0x00, 0x03, 0xc6,
   0xf1, 0x70, 0x00, 0x03, 0x06, 0x12, 0x89, 0x00, 0x03, 0xc6, 0x13, 0x09,
   0x00, 0x03, 0x26, 0x12, 0x09, 0x00, 0x03, 0x26, 0x12, 0x89, 0x00, 0x03,
   0xc6, 0xf3, 0x70, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00,
   0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00,
   0x03, 0x06, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x03, 0x06,
   0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00,
   0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x03,
   0x06, 0x00, 0x00, 0x00, 0x03, 0x06, 0x02, 0x00, 0x00, 0x03, 0x06, 0x1e,
   0x00, 0x00, 0x03, 0x06, 0xfe, 0x01, 0x00, 0x03, 0x06, 0xc2, 0x0f, 0x00,
   0x03, 0x06, 0x02, 0x7e, 0x00, 0x03, 0x06, 0x02, 0xe0, 0x07, 0x03, 0x0c,
   0x03, 0x00, 0xbf, 0x01, 0x9c, 0x03, 0x00, 0xf8, 0x01, 0xf8, 0xff, 0xff,
   0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

}
//TODO: This app has its own directory and own config management system. I should probably move it somewhere higher in the hierarchy later.
class NotesApp :public BuiltInApplication
{
private:
	class NotesAppLayout :public CustomScreen {
	public:
		NotesApp* app;
		GenericMenuScreen* menu;
		GenericTextScreen* newNoteScreen;
		bool editingNewNote = false;

		LinkedList<char*> menuPositionNames = LinkedList<char*>();
		char internalBuffer[150];
		void refreshList() {
			for (int j = 0; j < menuPositionNames.size(); j++) {
				free(menuPositionNames.get(j));
			}
			menuPositionNames.clear();
			menu->ClearList();
			int notesAmount = app->GetNotesAmount();
			for (int i = 0; i < notesAmount; i++) {
				auto newPositionText = (char*)calloc(15, sizeof(char));
				auto file = app->GetNoteFileObjectNamed(i);
				auto readBytes = file.readBytes(newPositionText, 14);
				newPositionText[readBytes] = '\0';
				Serial.println(readBytes);
				menuPositionNames.add(newPositionText);
				Serial.println(newPositionText);
				i++;
			}
			
			menu->AddOption("New note", [this] {
				DisplayNewNoteScreen();
				});
			for (int i = 0; i < menuPositionNames.size(); i++) {
				menu->AddOption(menuPositionNames.get(i), [this,i] {
					DisplayNote(i);
					});
			}
		}
		NotesAppLayout(NotesApp* app) {
			this->app = app;
	
			menu = new GenericMenuScreen(app->UI);
			refreshList();

			newNoteScreen = new GenericTextScreen(app->UI);
			newNoteScreen->onBackCallback = [this] {
				if (editingNewNote) {
					this->app->CreateNewNote("a", this->app->UI->GetKeyboard()->target);
					refreshList();
				}
				DisplayScreen((Layout*)menu);
			};
			newNoteScreen->onDownCallback = [this] {};
			newNoteScreen->onUpCallback = [this] {};
			newNoteScreen->onOkCallback = [this] {
				if (editingNewNote) {
					this->app->CreateNewNote("a", this->app->UI->GetKeyboard()->target);
					refreshList();
				}
				DisplayScreen((Layout*)menu);
			};
			
			DisplayScreen((Layout*)menu);
		}
		void DisplayNote(int name) {
			editingNewNote = false;
			this->newNoteScreen->scrollable = true;
			app->GetNoteNamed(name, &internalBuffer[0]);
			newNoteScreen->text = internalBuffer;
			this->DisplayScreen((Layout*)newNoteScreen);
		}
		void Draw(Renderer& r) {
			currentScreen->Draw(r);
		}
		void Back(Renderer& r) override {
			if (currentScreen == (Layout*)menu)
				app->Exit();
			else
			currentScreen->Back(r);
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
		void DisplayNewNoteScreen() {
			this->app->UI->DisplayKeyboard();
			editingNewNote = true;
			auto kbrd = this->app->UI->GetKeyboard();
			this->newNoteScreen->scrollable = false;
			kbrd->onClose = [this] {this->newNoteScreen->scrollable = true; };
			newNoteScreen->text = kbrd->target;
			this->DisplayScreen((Layout*)newNoteScreen);
		}
	};
protected:
	NotesAppLayout* l = new NotesAppLayout(this);

public:


	NotesApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		this->layout = l;
		this->name = (char*)PSTR("Notes");

		auto spiffs = sm->SPIFFS;
		if (!spiffs->exists("/n")) {
			spiffs->mkdir("/n");
		}

		
	}
	fs::Dir GetNotesFolder() {
		auto spiffs = settingsManager->SPIFFS;
		auto f = spiffs->openDir("/n");
		return f;
	}
	void CreateNewNote(char* filename, char* content) {
		auto spiffs = settingsManager->SPIFFS;
		char c[32];
		int nAmount = GetNotesAmount();
		Serial.println(nAmount);
		sprintf(c, "/n/%d", nAmount);
		auto f = spiffs->open(c, "w+");
		Serial.println(content);
		Serial.println(c);
		f.printf("%s", content);
		f.close();
	}
	fs::File GetNoteFileObjectNamed(int filename) {
		auto spiffs = settingsManager->SPIFFS;
		char c[32];
		sprintf(c, "/n/%d", filename);
		Serial.println(c);
		auto f = spiffs->open(c, "r");
		return f;
	}
	int GetNoteNamed(int filename, char* content) {
		auto spiffs = settingsManager->SPIFFS;
		char c[32];
		sprintf(c, "/n/%d", filename);
		auto f = spiffs->open(c, "r");
		auto len = f.readBytesUntil('\0', (uint8_t*)content, 500);
		f.close();
		return len;
	}
	int GetNotesAmount() {
		auto spiffs = settingsManager->SPIFFS;
		auto f = spiffs->openDir("/n");
		int amount = 0;
		while (f.next()) {
			amount++;
		}
		return amount;
	}
	

	void OnOpen() override {





	}
	void OnExit() override {
		
	}


};
