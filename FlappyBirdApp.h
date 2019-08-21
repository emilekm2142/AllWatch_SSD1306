// FlappyBirdApp.h

#ifndef _FLAPPYBIRDAPP_h
#define _FLAPPYBIRDAPP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "BuiltInApplication.h"
#include "Renderer.h"
#include "CustomScreen.h"
#include "FlappyBird.h"
class FlappyBirdApp: public BuiltInApplication
{
private:
	 
	 class FlappyBirdLayout :public CustomScreen {
	 public:
		 FlappyBirdApp* app;
		 FlappyBirdLayout(FlappyBirdApp* a){
			 app = a;
		 }
		 void Draw(Renderer& r) override {}
		 void Up(Renderer& r) override {}
		 void Down(Renderer& r) override {}
		 void Back(Renderer& r) override {
			 app->UI->ReturnToParentLayout();
			 this->app->Exit();
		 }
		 void Ok(Renderer& r) override {}
		 void DrawActiveIndicator(Renderer& r) override{}

	 };

 public:
	 FlappyBirdLayout* l = new FlappyBirdLayout(this);
	 flappyBird::FlappyBird* game;
	 ESPert* espertLayer;
	 FlappyBirdApp(UserInterfaceClass* UI, SettingsManager* sm) : BuiltInApplication((Layout*)this->l, UI, sm) {
		 Serial.println("opening flappy bird!");
		 UI->DisableDrawing();
		 this->layout = l;
		 espertLayer =new ESPert(UserInterface.GetRenderer());
		 this->name = (char*)F("Flappy Bird");
		 this->game = new flappyBird::FlappyBird();

		 game->init(espertLayer, false);
	 }
	 void OnOpen() override {
		
		
	}
	 void OnExit() override {
		 delete game;
		 delete espertLayer;
		 UI->EnableDrawing();
	 }
	 void Loop(Renderer& r) override {
		
		 game->update();
		 game->render();
	 }
};



#endif

