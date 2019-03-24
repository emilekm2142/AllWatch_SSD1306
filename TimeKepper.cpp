// 
// 
// 

#include "TimeKepper.h"

int TimeKepper::GetHour() {
	return hour;
}
int TimeKepper::GetMinute() {
	return minute;
}
int TimeKepper::GetSecond() {
	return second;
}
/// <summary>
/// return -1 when the data is invalid
/// </summary>
int TimeKepper::SetTime(int hour, int minute, int second) {
	if (hour < 24 && hour >= 0 && minute >= 0 && minute < 60 && second >= 0 && second < 60) {
		this->minute = minute;
		this->hour = hour;
		this->second = second;
		return 0;
	}
	else {
		return -1;
	}
}
void TimeKepper::IncrementSecond() {
	this->second += 1;
}
void TimeKepper::RewalidateTime() {
	if (second >= 60) {
		minute++;
		second = 0;
	}
	if (minute >= 60) {
		hour++;
		minute = 0;
	}
	if (hour >= 24) {
		hour = 0;
	//	OnDayEnd();
	}
}


