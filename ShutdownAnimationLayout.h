// ShutdownAnimationLayout.h

#ifndef _SHUTDOWNANIMATIONLAYOUT_h
#define _SHUTDOWNANIMATIONLAYOUT_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "Layout.h"
#include "Widget.h"
#include "TimeKepper.h"
#include "Animation.h"
#include "UserInterface.h"
#include "SettingsManager.h"


class ShutdownAnimationLayout :public Layout
{
 protected:
	

 public:
	 UserInterfaceClass* UI;

	 virtual void Up(Renderer& renderer) override {

	 }
	 virtual void Down(Renderer& renderer) override {

	 }
	 virtual int CalculateHeight(Renderer& renderer) override {
		 return 18;
	 }
	 virtual int CalculateWidth(Renderer& renderer) override {
		 return renderer.GetScreenWidth();
	 }
	 virtual void Ok(Renderer& renderer) override {}
	 virtual void Back(Renderer& renderer) override {}
	 ShutdownAnimationLayout(UserInterfaceClass* UI) {
		 this->UI = UI;
		
	 }
	int GetAnimationLength()
	 {
		return 1100;
	 }
	int topRectangleHeight = 0;
	int bottomRectangleHeight = 0;
	int midLineWidth = 0;
	int circleRadius = 0;

	//const unsigned char* frames[3] = { UIAssets::s3_bits, UIAssets::s4_bits, UIAssets::s5_bits };
	//int index = 0;
	//int len =3;
	
	void PlayShutdownAnimation(Renderer& renderer)
	{
		//https://www.youtube.com/watch?v=-MH6JZdGZcI

		std::function<float(double)> lineFunc = Animation::GetBezier(.86f, .02f, 1.0f, .02f);
		std::function<float(double)> circleFunc = Animation::GetBezier(.78f, .37f, 0.0f, 1.6f);

		auto circleAnimation = new Animation(circleRadius, 4, 1, 1);//, circleFunc, 200);
		
			auto a1 = new Animation(topRectangleHeight, renderer.GetScreenHeight() / 2, 1, 4);/// , lineFunc, 300);
		Renderer* r = &renderer;
		
		auto a2 = new Animation(midLineWidth, r->GetScreenWidth() * 2, 1, 4);// , lineFunc, 450);
		auto a3 = new Animation(midLineWidth, 0, 1, 1);// , lineFunc, 200);
		
		UI->RegisterAnimation(a1);
		Run::After(200, [this, a2,circleAnimation] {UI->RegisterAnimation(a2); UI->RegisterAnimation(circleAnimation); });
		//Run::After(900, [this,a2, a3, circleAnimation] {
			//UI->CancelAnimation(a2);
			//UI->RegisterAnimation(a3);
			//circleRadius = 0;
	//	UI->RegisterAnimation(circleAnimation);
		//});
	}
	
	void Draw(Renderer& renderer) override
	{

		renderer.FillRectangle(0, 0, topRectangleHeight, renderer.GetScreenWidth(), true);
		renderer.FillRectangle(0, renderer.GetScreenHeight() - topRectangleHeight, topRectangleHeight, renderer.GetScreenWidth(), true);


		renderer.FillRectangle(renderer.GetHorizontalCenter() - (midLineWidth / 2), renderer.GetVerticalCenter(), 1, midLineWidth);

		renderer.FillCircle(renderer.GetHorizontalCenter(), renderer.GetVerticalCenter(), circleRadius);
	}
};



#endif

