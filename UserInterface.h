﻿// UserInterface.h

#ifndef _USERINTERFACE_h
#define _USERINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "Widget.h"
#include "Layout.h"
#include "Renderer.h"
#include "Animation.h"

//#include "DependenciesHolder.h"
class UserInterfaceClass//:public Dependency
{
private:
	boolean shouldDraw = true;
	Renderer* renderer;
	Layout* mainLayout;
	Layout* focusedLayout=nullptr;
	LinkedList<Layout*> layouts = LinkedList<Layout*>();
	LinkedList<Animation*> animations = LinkedList<Animation*>();

	//Parenting
	Layout* layoutThatWasInFocus;
	Layout* parentLayout;
	Layout* childLayout;
	bool isChildBeingShown = false;

	bool anyChangesStaged = false;
	bool IsAnyLayoutDisplayed() {
		return mainLayout != nullptr;
	}
 protected:
	  
	 
	 
 public:
	 boolean GetShouldDraw() {
		 return shouldDraw;
	 }
	 void DisableDrawing() {
		 shouldDraw = false;
	 }
	 void EnableDrawing() {
		 shouldDraw = true;
	 }
	 //	DependenciesHolder* dependencies;
	 char currentScreenName[50];
	 void RegisterAnimation(Animation* animation) {
		 animations.add(animation);
		 OnLoop();
	 }
	 Layout* GetMainLayout() {
		 return mainLayout;
	 }
	 void StageChanges() {
		 anyChangesStaged = true;
	 }
	 void AddSecondaryLayout(Layout* l) {
		 layouts.add(l);
	 }
	 void ReturnToParentLayout() {
		 mainLayout = parentLayout;
		 auto oldChildLayout = childLayout;
		 childLayout = NULL;
		 delete oldChildLayout;
		 ShowLayout(*mainLayout);
		 isChildBeingShown = false;
		 SetLayoutInFocues(*layoutThatWasInFocus);
		 RedrawAll();

		 Serial.println("UserInterface::ReturnToParentLayout: finished");
	 }

	 void OpenChildLayout(Layout* l) {
		 layoutThatWasInFocus = focusedLayout;
		 parentLayout = mainLayout;
		 childLayout = l;
		 SetLayoutInFocues(*l);
		 ShowLayout(*l);
		 isChildBeingShown = true;
		 Serial.print("Opened child layout");
		 RedrawAll();

	 }
	 void OnLoop() {
		 bool update = false;
		 for (int i = 0; i < animations.size(); i++) {
			 auto a = animations.get(i);
			 if (a->ShouldStepBeTaken()) {
				 a->ExecuteStep();
				 update = true;
			 }
			 if (a->IsFinished()) {
				 animations.remove(i);
				 delete a;
			 }
		 }

		 if (anyChangesStaged) {
			 renderer->Clear();
			 if (IsAnyLayoutDisplayed()) {
				 update = true;
			 }
			 anyChangesStaged = false;
		 }
		 if (update) RedrawAll();
	 
		
	 }
	 void SetRenderer(Renderer& rndr) {
		 this->renderer = &rndr;
	 }
	 Renderer* GetRenderer() {
		 return this->renderer;
	 }
	 void Down() {
		 if (IsAnyLayoutDisplayed()) {
			 focusedLayout->Down(*renderer);
		 }
		 
	 }
	 void Up() {
		 if (IsAnyLayoutDisplayed()) {
			 focusedLayout->Up(*renderer);
		 }
	 }
	 void Ok() {
		 if (IsAnyLayoutDisplayed()) {
			 focusedLayout->Ok(*renderer);
		 }
	 }
	 void Back() {
		 if (IsAnyLayoutDisplayed()) {
			 focusedLayout->Back(*renderer);
		 }
	 }
	 void RedrawAll() {
		 if (!shouldDraw) { return; }
		 renderer->Clear();

		 if (!isChildBeingShown)
			 ShowLayout(*mainLayout);
		 else
			 ShowLayout(*childLayout);
		 DrawSecondaryLayouts();

		 if (focusedLayout != nullptr) {
			 focusedLayout->DrawActiveIndicator(*renderer);
		 }


		 renderer->Update();
	 }
	 void SetLayoutInFocues(Layout& layout) {
		 focusedLayout = &layout;
		 focusedLayout->OnGetInFocus(*renderer);
		 layout.DrawActiveIndicator(*renderer);
	 }
	 void SetMainLayout(Layout& layout) {
		 mainLayout = &layout;
		 SetLayoutInFocues(layout);
	 }
	 void ShowLayout(Layout& layout) {
		
		 
		 layout.CalculateLayout(*renderer);
		 layout.DrawWithState(*renderer);
		 if (focusedLayout == &layout) {
			
		//	 layout.DrawActiveIndicator(*renderer);
		 }

	}
	 void DrawSecondaryLayouts() {
		 for (int i = 0; i < layouts.size(); i++) {
			 auto w = layouts.get(i);
			 ShowLayout(*w);
		 }
	 }
	 void ClearMemory() {
		 delete mainLayout;
	 }
};

extern UserInterfaceClass UserInterface;

namespace UIAssets {
#define baseline_access_time_black_18dp_width 18
#define baseline_access_time_black_18dp_height 18
	static unsigned char baseline_access_time_black_18dp_bits[] PROGMEM = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1f, 0x00, 0x70, 0x38, 0x00,
		0x18, 0x60, 0x00, 0x0c, 0xc1, 0x00, 0x0c, 0xc1, 0x00, 0x04, 0x81, 0x00,
		0x04, 0x81, 0x00, 0x04, 0x83, 0x00, 0x04, 0x84, 0x00, 0x0c, 0xc8, 0x00,
		0x0c, 0xc0, 0x00, 0x18, 0x60, 0x00, 0x70, 0x38, 0x00, 0xe0, 0x1f, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define cpu_width 8
#define cpu_height 8
	static unsigned char cpu_bits[] PROGMEM = {
	 0xDB, 0x81, 0x3C, 0xBD, 0xBD, 0x3C, 0x81, 0xDB };


#define baseline_videogame_asset_black_18dp_width 36
#define baseline_videogame_asset_black_18dp_height 36
	static unsigned char baseline_videogame_asset_black_18dp_bits[] PROGMEM = {
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff,
	   0xff, 0x01, 0xfc, 0xff, 0xff, 0xff, 0x03, 0xfc, 0xff, 0xff, 0xff, 0x03,
	   0xfe, 0xf1, 0xff, 0xff, 0x07, 0xfe, 0xf1, 0xff, 0xff, 0x07, 0xfe, 0xf1,
	   0xff, 0x87, 0x07, 0xfe, 0xf1, 0xff, 0x87, 0x07, 0xfe, 0xf1, 0xff, 0x87,
	   0x07, 0x1e, 0x00, 0xff, 0xcf, 0x07, 0x1e, 0x00, 0x3f, 0xff, 0x07, 0xfe,
	   0xf1, 0x1f, 0xfe, 0x07, 0xfe, 0xf1, 0x1f, 0xfe, 0x07, 0xfe, 0xf1, 0x1f,
	   0xfe, 0x07, 0xfe, 0xf1, 0xff, 0xff, 0x07, 0xfe, 0xf1, 0xff, 0xff, 0x07,
	   0xfc, 0xff, 0xff, 0xff, 0x03, 0xfc, 0xff, 0xff, 0xff, 0x03, 0xf8, 0xff,
	   0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
}

#endif

