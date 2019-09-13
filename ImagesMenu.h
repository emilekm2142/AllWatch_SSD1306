// ImagesMenu.h

#ifndef _IMAGESMENU_h
#define _IMAGESMENU_h

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

typedef struct ImagesMenuEntry {
	int width;
	char* name;
	std::function<void()> callback;
	const unsigned char* image;
	int padding;
};

class ImagesMenu :CustomScreen
{
public:
	int initialPadding = 0;
	int tileSize = 40;
	int screenEdgePaddingX = 5;
	int screenEdgePaddingY = 16;
	int inRow = 2;
	int midPadding = 10;
	int moveFactor = 3;
	int scrollY = 0;
	int scrollX = 0;
	int currentIndex = 0;
	int animTime = 2;
	int stepSize =5;

	bool isRunInsideApplication = false;
	LinkedList<ImagesMenuEntry*>* entries = new LinkedList<ImagesMenuEntry*>();
	

	

	ImagesMenu(UserInterfaceClass* UI) {
		this->UI = UI;
		
		
	}
	ImagesMenu(UserInterfaceClass* UI, LinkedList<ImagesMenuEntry*>* entries, bool isRunInsideApplication=false) {
		this->UI = UI;
		this->isRunInsideApplication = isRunInsideApplication;
		this->entries = entries;
	}
	void AddOption(char* name, const unsigned char* image, std::function<void()> callback, bool copy = false) {
		ImagesMenuEntry* e = new ImagesMenuEntry();
		e->callback = callback;
		e->image = image;
		e->name = name;
		e->padding = 10;
	
		e->width = 37;
		this->entries->add(e);
	}
	virtual int CalculateHeight(Renderer& renderer) override {
		return renderer.GetScreenHeight();
	}
	virtual int CalculateWidth(Renderer& renderer) override {
		return renderer.GetScreenWidth();
	}
	virtual void Up(Renderer& renderer) override {
		int targetScroll = 0;
	
		if (currentIndex > 0) {
			targetScroll = scrollX + getRealWidth(entries->get(this->currentIndex)) + midPadding;
			currentIndex--;
			auto anim = new Animation(scrollX, targetScroll, animTime, stepSize);
			UI->RegisterAnimation(anim);
		}
		else {
			
			
			for (int i = 0; i < entries->size(); i++) {
				targetScroll -= getRealWidth(entries->get(i));
			}
			currentIndex = entries->size() - 1;
			auto anim = new Animation(scrollX, targetScroll, animTime, -stepSize*5);
			UI->RegisterAnimation(anim);
		}
		
	}
	virtual void Back(Renderer& r) override {
		UI->SetLayoutInFocues(*UI->GetMainLayout());
	}

	virtual void Down(Renderer& renderer) override {
		int targetScroll = 0;
		if (currentIndex < entries->size()-1) {
			int currWidth = getRealWidth(entries->get(this->currentIndex));
			int nextWidth = getRealWidth(entries->get(this->currentIndex+1));
			targetScroll = scrollX - currWidth - (abs(currWidth-nextWidth))/2- midPadding;
			currentIndex++;
			auto anim = new Animation(scrollX, targetScroll, animTime, -stepSize);
			UI->RegisterAnimation(anim);
		}
		else {
			currentIndex = 0;
			auto anim = new Animation(scrollX, targetScroll, animTime, stepSize*5);
			UI->RegisterAnimation(anim);
		}
		

	}
	virtual void Ok(Renderer& renderer) override {

		this->entries->get(currentIndex)->callback();
		
	}
	int getRealWidth(ImagesMenuEntry* e) {
		int w = UI->GetRenderer()->GetStringWidth(e->name);
		int width = w > 37 ? w : 37;
		return width;
	}

	
	virtual void Draw(Renderer& renderer) override {
		int currentX = 0;
		int initialPadding = renderer.GetHorizontalCenter()-entries->get(0)->width/2;
		for (int i = 0; i < entries->size(); i++) {
			auto entry = entries->get(i);
			int width = getRealWidth(entry);
			renderer.DrawBitmap(currentX + GlobalX + scrollX + initialPadding, 10, 36, 36, entry->image);
			renderer.DrawAlignedString(currentX + GlobalX + scrollX + initialPadding  + width / 2, 37, entry->name, width, renderer.Center);
			//renderer.DrawRectangle(currentX + GlobalX + scrollX + initialPadding, 18, 37,width);
			currentX += width + midPadding;
		}
	}

	virtual void CalculateLayout(Renderer& renderer) override {
		CalculateSizesPostOrderTreeWalk(renderer);
		ApplyGlobalCoordinatesPreorderTreeWalk(this, renderer);

	}
	

};




#endif

