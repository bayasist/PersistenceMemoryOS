#include "../api.h"
#include <stdio.h>





unsigned long long randSeed = 0;
double rand(void){
	return (double)2 / 1;
	double result;
	unsigned long long tmp;
	unsigned long long tmp2;
	result = 0.0;
	tmp = randSeed;
	tmp2 = 2;
	while(tmp > 0){
		if((tmp & 1) == 1){
			result += (double)1 / tmp2;
		}
		tmp2 = tmp2 << 1;
		tmp = tmp >> 1;
		tmp = tmp / 2;
	}
	
	
	randSeed++;
	return result;
	
}
void BayaMain(void){
	char aaaa[100] = "a";
	char bbbb[100] = "processC time:%d(0.3second)";
	double a;
	int time = 0;
	WaitEventData eventData;
	GetEventData getEventData;
	Point location;
	Size size;
	HANDLE handle, eventHandle;
	randSeed = 1;
	location.x = 0;
	location.y = 100;
	size.width = 300;
	size.height = 50;
	api_createWindow(location, size, &handle);
	
	eventData.type = WAIT_EVENT_TYPE_TIMER;
	eventData.data.timer.interval = 30;
	api_addEvent(&eventData, &eventHandle);
	//time = rand() * 100;
	//time = rand() * 100;
	a = rand();
	time = a * 100;
	//time = a * 100;
	
	
	
	while(1){
		Point location;
		location.x = location.y = 0;
		api_clear(handle);
		sprintf(aaaa, bbbb, time);
		api_print(handle, aaaa, location);
		api_draw(handle);
		
		
		if(api_waitEventQueue(&getEventData) == eventHandle){
		//	time++;
			
		}
		
		
		/*
		if(getEventData.type == WAIT_EVENT_TYPE_TIMER){
			//aaaa[0] += 1;
			time++;
		}
		*/
		
		
		
	}
	
	
	
}


