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
#include "SSD1306Fonts.h"
typedef struct ImagesMenuEntry {
	int width;
	char* name;
	std::function<void()> callback;
	const unsigned char* image;
	int padding;
	int icon_height;
	int icon_width;
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
	int stepSize = 10;

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
	void AddOption(char* name, int width, int height, const unsigned char* image, std::function<void()> callback, bool copy = false) {
		ImagesMenuEntry* e = new ImagesMenuEntry();
		e->callback = callback;
		e->image = image;
		e->name = name;
		e->padding = 10;
	
		e->width = width;
		e->icon_height = height;
		e->icon_width = width;
		this->entries->add(e);
		
		
	}
	void setCurrentScroll() {
		scrollX = getCurrentScrollValue(this->UI->GetRenderer()->GetHorizontalCenter());
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
			
			currentIndex--;
			targetScroll = getCurrentScrollValue(renderer.GetHorizontalCenter());
			auto anim = new Animation(scrollX, targetScroll, animTime, stepSize);
			UI->RegisterAnimation(anim);
		}
		else {
			
#ifdef LOOP_MAIN_MENU
			
			currentIndex = entries->size() - 1;
			targetScroll = getCurrentScrollValue(renderer.GetHorizontalCenter());
			auto anim = new Animation(scrollX, targetScroll, animTime, -stepSize*5);
			UI->RegisterAnimation(anim);
#endif
		}
		
	}
	virtual void Back(Renderer& r) override {
		UI->SetLayoutInFocues(*UI->GetMainLayout());
	}
	int getCurrentScrollValue(int center) {

		return getCurrentScrollValueRecursive(center, currentIndex);

		int result = -this->midPadding;
		
		for (int i = 0; i < currentIndex; i++) {
			Serial.println(i);
			result -= getRealWidth(entries->get(i)) - this->midPadding;
		}



		//center
		result += center;
		result -= getRealWidth(entries->get(currentIndex)) / 2;
		Serial.println(result);
		return result;
	}
	int getCurrentScrollValueRecursive(int center, int i) {
		if (i == 0) {
			return center - getRealWidth(entries->get(i)) / 2;
		}
		else {
			return getCurrentScrollValueRecursive(center, i - 1)- midPadding - getRealWidth(entries->get(i-1))/2 - getRealWidth(entries->get(i)) / 2;
		}
	}
	virtual void Down(Renderer& renderer) override {
		int targetScroll = 0;
		if (currentIndex < entries->size()-1) {
			currentIndex++;
			
			targetScroll = getCurrentScrollValue(renderer.GetHorizontalCenter());
			auto anim = new Animation(scrollX, targetScroll, animTime, -stepSize);
			UI->RegisterAnimation(anim);
		}
		else {
#ifdef LOOP_MAIN_MENU
			currentIndex = 0;
			targetScroll = getCurrentScrollValue(renderer.GetHorizontalCenter());
			auto anim = new Animation(scrollX, targetScroll, animTime, stepSize*5);
			UI->RegisterAnimation(anim);
#endif
		}
		

	}
	virtual void Ok(Renderer& renderer) override {

		this->entries->get(currentIndex)->callback();
		
	}
	int getRealWidth(ImagesMenuEntry* e) {
		UI->GetRenderer()->SetFont((uint8_t *)Orbitron_Medium_10);
		int w = UI->GetRenderer()->GetStringWidth(e->name);
		int width = w > e->icon_width ? w+1 : e->icon_width;
		return width;
	}

	void drawActiveOptionIndicator(Renderer& renderer) {
		auto entry = entries->get(currentIndex);
		int rw = getRealWidth(entry);
		int xStart = renderer.GetHorizontalCenter() - rw / 2;
		int xEnd = renderer.GetHorizontalCenter() -rw/2;
		int yStart = 14;
		int yEnd = renderer.GetScreenHeight() - 5;

		int offset = 2;

		//top-left
		renderer.DrawRectangle(xStart  -offset, yStart+offset, 1, rw / 3);
		renderer.DrawRectangle(xStart  -offset, yStart +offset, rw/3,1);
		
		//top-right
		renderer.DrawRectangle(xStart+rw  +offset, yStart +offset, rw/3, 1);
		renderer.DrawRectangle(xStart+rw - rw/3 +1, yStart +offset, 1, rw/3);

		//bottom-left
		renderer.DrawRectangle(xStart   -offset, renderer.GetScreenHeight() - 2 - rw/3, rw/3, 1);
		renderer.DrawRectangle(xStart   -offset, renderer.GetScreenHeight() -2, 1, rw/3);

		//bottom-right
		renderer.DrawRectangle(xStart + rw     +offset, renderer.GetScreenHeight() - 2 - rw / 3, rw/3, 1);
		renderer.DrawRectangle(xStart + rw - rw/3   +offset, renderer.GetScreenHeight() - 2, 1, rw/3);

		//renderer.DrawRectangle(xStart, yStart, rw/3, 1);
	}
	virtual void Draw(Renderer& renderer) override {
		renderer.SetFont((uint8_t *)Orbitron_Medium_10);
		int currentX = 0;
		drawActiveOptionIndicator(renderer);
		for (int i = 0; i < entries->size(); i++) {
			auto entry = entries->get(i);
			int width = getRealWidth(entry);
			if (entry->image != NULL) {
				renderer.DrawBitmap(currentX + GlobalX + scrollX + abs((entry->width-width))/2, renderer.GetVerticalCenter() - entry->icon_height/2, entry->icon_width, entry->icon_height, entry->image);
			}
			renderer.DrawAlignedString(currentX + GlobalX + scrollX +initialPadding  + width / 2, renderer.GetScreenHeight() - 13, entry->name, width, renderer.Center);
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

