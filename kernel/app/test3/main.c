#include "../api.h"
#include <stdio.h>
void BayaMain(void){
	char aaaa[100] = "a";
	char bbbb[100] = "processC time:%d(0.3second)";
	int time = 0;
	WaitEventData eventData;
	GetEventData getEventData;
	Point location;
	Size size;
	HANDLE handle, eventHandle;
	location.x = 0;
	location.y = 100;
	size.width = 300;
	size.height = 50;
	api_createWindow(location, size, &handle);
	
	eventData.type = WAIT_EVENT_TYPE_TIMER;
	eventData.data.timer.interval = 30;
	//api_addEvent(&eventData, &eventHandle);
	
	
	while(1){
		Point location;
		location.x = location.y = 0;
		time++;
		api_clear(handle);
		sprintf(aaaa, bbbb, time);
		api_print(handle, aaaa, location);
		api_draw(handle);
		
		/*
		if(api_waitEventQueue(&getEventData) == eventHandle){
			time++;
			
		}
		*/
		
		/*
		if(getEventData.type == WAIT_EVENT_TYPE_TIMER){
			//aaaa[0] += 1;
			time++;
		}
		*/
		
		
		
	}
	
	
	
}


