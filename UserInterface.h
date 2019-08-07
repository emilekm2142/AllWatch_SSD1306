// UserInterface.h

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
	 //	DependenciesHolder* dependencies;
	 char currentScreenName[50];
	 void RegisterAnimation(Animation* animation) {
		 animations.add(animation);
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
		 childLayout = NULL;
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

#endif

