// DelayedAction.h

#ifndef _DELAYEDACTION_h
#define _DELAYEDACTION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
namespace Run {
	struct DelayedActionDataHolder {
		int delay;
		std::function<void()> action;
		int startTime;
		int type; //0 - 
	};
	LinkedList<DelayedActionDataHolder*> __a = LinkedList<DelayedActionDataHolder*>();
	DelayedActionDataHolder* After(int delay, std::function<void()> action) {
		auto b = new DelayedActionDataHolder();
		b->delay = delay;
		b->action = action;
		b->type = 0;
		b->startTime = millis();
		__a.add(b);
		return b;
	}
	DelayedActionDataHolder* Every(int delay, std::function<void()> action) {
		action();
		auto b = new DelayedActionDataHolder();
		b->delay = delay;
		b->action = action;
		b->type = 1;
		b->startTime = millis();
		__a.add(b);
		return b;
	}
	void Cancel(DelayedActionDataHolder* b) {
		if (b == NULL || b == nullptr) { return; }
		for (int i = 0; i < __a.size(); i++) {
			if (__a.get(i) == b) {
				__a.remove(i);
				//TOOD: Deallocate;
				delete b;
				b = nullptr;
			
			}
		}
	}
	void Loop() {
		for (int i = 0; i < __a.size(); i++) {
			if (millis() - __a.get(i)->startTime > __a.get(i)->delay) {
				__a.get(i)->action();
				if (__a.get(i)->type == 0) {
					__a.remove(i);
					break;
				}
				if (__a.get(i)->type == 1) {
					__a.get(i)->startTime = millis();
				}
				
			}
		}
	}
}
#endif

