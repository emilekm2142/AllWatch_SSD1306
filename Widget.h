// Widget.h

#ifndef _WIDGET_h
#define _WIDGET_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "Renderer.h"
class Widget
{
protected:
	
	int x=0, y=0, width, height;
	int GlobalX=0, GlobalY=0;
	bool wasVisited_Sizes = false; bool wasVisited_Coordinates = false;
	bool border=false;

	void CalculateSizesPostOrderTreeWalk(Renderer& renderer) {
		auto children = GetChildren();
		if ( children != NULL) {
			for (int i = 0; i < children->size(); i++) {
				Widget* child = children->get(i);
				child->CalculateSizesPostOrderTreeWalk(renderer);
			}
		}
		CalculateSize(renderer);

	}
	void ApplyGlobalCoordinatesPreorderTreeWalk(Widget* parent, Renderer& renderer) {
		ApplyGlobalCoordinates(parent,renderer);
		auto children = GetChildren();
		if (children != NULL) {
			for (int i = 0; i < children->size(); i++) {
				Widget* child = children->get(i);
				child->ApplyGlobalCoordinatesPreorderTreeWalk(this,renderer);
			}
		}
		
	}
	void Unmark() {
		wasVisited_Coordinates = false;
		wasVisited_Sizes = false;
		auto children = GetChildren();
		if (children != NULL) {
			for (int i = 0; i < children->size(); i++) {
				Widget* child = children->get(i);
				child->Unmark();
			}
		}

	}

public:
	void setBorder(bool draw) {
		border = draw;
	}

	bool canBeActivated = false;
	bool isActive=false;
	bool isHighlighted = false;
	void SetActive(bool a) {
		if (a) {
			onActivate();
		}
		isActive = a;
		
	}
	void CalculateSize(Renderer& renderer) {
	
		width = GetWidth(renderer);
		height = GetHeight(renderer);
		wasVisited_Sizes = true;
	}
	virtual void ApplyGlobalCoordinates(Widget* parent, Renderer& renderer) {
		if (parent != this) {
			setGlobalX(parent->GlobalX + x);
			setGlobalY(parent->GlobalY + y);
		}
		else {
			
		}
		wasVisited_Coordinates = true;
	}


	virtual void CalculateLayout(Renderer& renderer) {}
	virtual void DrawHighlighted(Renderer& renderer) { Draw(renderer); }
	void(*onActivate)();
	virtual void DrawWithState(Renderer& renderer) {
		/*Serial.println("-----Object-----");
		Serial.println(GlobalX);
		Serial.println(GlobalY);*/
		

		if (isHighlighted) DrawHighlighted(renderer);
		else Draw( renderer);
		if (border) DrawBorder(renderer);
	}
	virtual void DrawBorder(Renderer& r) {
		/*Serial.println("---Border----");
		Serial.println(GlobalX);
		Serial.println(GlobalY);
		Serial.println(GetHeight(r));
		Serial.println(GetWidth(r));*/
		r.DrawRectangle(GlobalX, GlobalY, GetHeight(r), GetWidth(r));
	}
	virtual void Draw(Renderer& renderer)=0;
	virtual int GetHeight(Renderer& renderer) {
		if (wasVisited_Sizes) {
			return height;
		}
		else {
			auto x= CalculateHeight(renderer);
			height = x;
			return x;
		}
	}
	virtual int GetWidth(Renderer& renderer) {
		if (wasVisited_Sizes) {
			return width;
		}
		else {
			auto x = CalculateWidth(renderer);
			width = x;
			return x;
		}
	}

	virtual int CalculateHeight(Renderer& renderer) = 0;

	virtual int CalculateWidth(Renderer& renderer) = 0;

	void setX(int x) {
		this->x = x;
	}
	void setY(int y) {
		this->y = y;
	}
	void setGlobalX(int x) {
		this->GlobalX = x;
	}
	void setGlobalY(int y) {
		this->GlobalY = y;
	}
	virtual LinkedList<Widget*>*  GetChildren() {
		return NULL;
	}
};



#endif

