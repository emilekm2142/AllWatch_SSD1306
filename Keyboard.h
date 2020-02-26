// Keyboard.h

#ifndef _KEYBOARD_h
#define _KEYBOARD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "Layout.h"
#include "Widget.h"
#include "SSD1306Fonts.h"
#include <SSD1306.h>

class Keyboard :public Layout
{
protected:
	char* signs = "abcdefghijklmnoprstuwyxz 1234567890-=!@#$%^&*()_+[];'\\\"|,./><?~`ABCDEFGHIJKLMNOPRSTUWYXZ";
public:
	bool visible = false;
	std::function<void()> onClose = nullptr;
	std::function<void()> onType = nullptr;


	void Close() {
		visible = false;
		if (onClose!=nullptr) onClose();
		onClose = nullptr;
		onType = nullptr;

	}
	void Show() {
		currentIndex = 0;
		positionInWord = 0;
		this->target[0] = '\0';
		this->visible = true;
	}
	virtual void Up(Renderer& renderer) override {
		if (!visible) return;
		currentIndex--;
		currentIndex = currentIndex % strlen(this->signs);

	}
	virtual void Down(Renderer& renderer) override {
		if (!visible) return;
		currentIndex++;
		currentIndex = currentIndex % strlen(this->signs);
	}
	virtual void Ok(Renderer& r) override {
		if (!visible) return;
		if (target == NULL) return;
		target[positionInWord] = this->signs[currentIndex];
		target[positionInWord+1] = '\0';
		positionInWord++;
		if (onType != nullptr) onType();
		
	}
	virtual void Back(Renderer& r) override {
		if (!visible) return;
		if (target!=NULL) target[positionInWord] = '\0';
		Close();
	}
	virtual int CalculateHeight(Renderer& renderer) override {
		return 18;
	}
	virtual int CalculateWidth(Renderer& renderer) override {
		return renderer.GetScreenWidth();
	}
	
	
	char target[50];
	Keyboard() {
	
	}
	int positionInWord = 0;
	int currentIndex = 0;
	int x, y,h,w = 0;
	void Draw(Renderer& renderer) override
	{
		renderer.SetFont((uint8_t*)&ArialMT_Plain_10);
		if (!visible) return;
		renderer.FillRectangle(x, y, h, w, true);
		int order = 0;
		for (int i = currentIndex - 7; i < currentIndex + 7 + 1; i++) {
			int charId = i;
			if (charId < 0) {
				charId = strlen(signs) - charId - 1;
			}
			else if (charId >= strlen(signs)) {
				charId = charId % strlen(signs);
			}
			//print
			//Serial.println(charId);
			
			char txt[2];
			txt[0] = signs[charId];
			txt[1] = '\0';
			//Serial.println(signs[charId]);
			renderer.DrawAlignedString(order * 8 + x + 8, y + h/2 - 6,txt,10,renderer.Center);
			renderer.DrawRectangle(8 * 8 + x, y, 3, 1);
			renderer.DrawRectangle(8 * 8 + x, y + h - 3, 3, 1);
			renderer.DrawRectangle(x, y, h, w);
			order++;
		}
		renderer.SetFont((uint8_t*)Orbitron_Medium_10);
	}
};



#endif

