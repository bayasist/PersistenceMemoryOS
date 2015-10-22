#include "../api.h"
#include <stdio.h>

void BayaMain(void){
	char aaaa[100] = "a";
	char bbbb[100] = "processA time:%d(second)";
	char cccc[100] = "MouseUp:%d";
	char cccc2[100] = "a";
	char dddd[100] = "MouseDown:%d";
	char dddd2[100] = "a";
	char eeee[100] = "MouseMove:%d";
	char eeee2[100] = "a";
	char ffff[100] = "Keyup:%d";
	char ffff2[100] = "a";
	int time = 0;
	int mouseUpCount = 0;
	int mouseDownCount = 0;
	int mouseMoveCount = 0;
	int keyupCount = 0;
	WaitEventData eventData;
	GetEventData getEventData;
	Point location;
	Size size;
	HANDLE handle, eventHandle, eventHandle2, eventHandle3, eventHandle4, eventHandle5;
	location.x = 20;
	location.y = 60;
	size.width = 200;
	size.height = 300;
	api_createWindow(location, size, &handle);
	
	eventData.type = WAIT_EVENT_TYPE_TIMER;
	eventData.data.timer.interval = 100;
	api_addEvent(&eventData, &eventHandle);
	
	eventData.type = WAIT_EVENT_TYPE_MOUSE_UP;
	eventData.data.mouse.windowHandle = handle;
	api_addEvent(&eventData, &eventHandle2);
	
	eventData.type = WAIT_EVENT_TYPE_MOUSE_DOWN;
	eventData.data.mouse.windowHandle = handle;
	api_addEvent(&eventData, &eventHandle3);
	
	eventData.type = WAIT_EVENT_TYPE_MOUSE_MOVE;
	eventData.data.mouse.windowHandle = handle;
	api_addEvent(&eventData, &eventHandle4);
	
	eventData.type = WAIT_EVENT_TYPE_KEYUP;
	eventData.data.mouse.windowHandle = handle;
	api_addEvent(&eventData, &eventHandle5);
	
	while(1){
		Point location;
		api_clear(handle);
		sprintf(aaaa, bbbb, time);
		sprintf(cccc2, cccc, mouseUpCount);
		sprintf(dddd2, dddd, mouseDownCount);
		sprintf(eeee2, eeee, mouseMoveCount);
		sprintf(ffff2, ffff, keyupCount);
		
		location.x = location.y = 0;
		api_print(handle, aaaa, location);
		location.y += 20;
		api_print(handle, cccc2, location);
		location.y += 20;
		api_print(handle, dddd2, location);
		location.y += 20;
		api_print(handle, eeee2, location);
		location.y += 20;
		api_print(handle, ffff2, location);
		api_draw(handle);
		
		
		HANDLE getHandle;
		
		getHandle = api_waitEventQueue(&getEventData);
		
		if(getHandle == eventHandle){
			time++;
		}
		if(getHandle == eventHandle2){
			mouseUpCount++;
		}
		if(getHandle == eventHandle3){
			mouseDownCount++;
		}
		
		if(getHandle == eventHandle4){
			mouseMoveCount++;
			mouseMoveCount = getEventData.data.mouse.location.x;
		}
		
		
		if(getHandle == eventHandle5){
			int i;
			ffff[0] = getEventData.data.key.keyChar;
			for(i = 0; i < 100; i++){
				ffff2[i] = 0;
			}
			keyupCount++;
		}
		
		
		
	}
	
	
}

