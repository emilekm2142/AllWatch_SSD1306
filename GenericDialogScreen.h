// GenericDialogScreen.h

#ifndef _GENERICDIALOGSCREEN_h
#define _GENERICDIALOGSCREEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "Layout.h"
#include "Widget.h"
#include "SettingsManager.h"
#include "Animation.h"
#include "UserInterface.h"
#include "StaticResources.h"
#include "SettingsScreen.h"

struct DialogScreenOptionDataHolder
{
	const char* name;
	std::function<void()> callback;
};
class GenericDialogScreen :CustomScreen
{
protected:
	bool scrollable = true;
	bool headerScroll = true;
	int offset = 15;
	LinkedList<DialogScreenOptionDataHolder*> optionsList = LinkedList<DialogScreenOptionDataHolder*>();
	int currentObjectIndex = 0;
public:
	const char* text;
	char* header;
	const char* btnText;
	
	GenericDialogScreen(UserInterfaceClass* UI, const char* text, const char* btnText, bool scrollable, char* header = nullptr, bool headerScroll = true) {
		this->text = text;
		this->scrollable = scrollable;
		this->UI = UI;
		this->btnText = btnText;
		this->header = header;
		this->headerScroll = headerScroll;
	}
	UserInterfaceClass* UI;
	virtual void Back(Renderer& r) override {

		UI->SetLayoutInFocues(*UI->GetMainLayout());
	}
	virtual void Ok(Renderer& r) override {
		optionsList.get(currentObjectIndex)->callback();
	}
	virtual void Down(Renderer& r) override {
		currentObjectIndex++;
		if (currentObjectIndex==optionsList.size())
		{
			currentObjectIndex = 0;
		}
	}
	void Add(const char* text, std::function<void()> callback)
	{
		auto d = new DialogScreenOptionDataHolder{ text, callback };
		this->optionsList.add(d);
	}
	virtual void Up(Renderer& r) override {
		currentObjectIndex--;
		if (currentObjectIndex<0)
		{
			currentObjectIndex = optionsList.size()-1;
		}
	}

	virtual void Draw(Renderer& r) override {
		r.DrawAlignedString(GlobalX + 0, GlobalY + offset, this->text, r.GetScreenWidth(), r.Left);
		for (int i =0; i<optionsList.size(); i++)
		{
			Serial.println(optionsList.get(i)->name);
		}
	}
	virtual void DrawActiveIndicator(Renderer &renderer) override {
		// Serial.println("Drawing generic text indicator");
		renderer.FillRectangle(0, renderer.GetScreenHeight() - 1, 1, renderer.GetScreenWidth());
	}

};


#endif

