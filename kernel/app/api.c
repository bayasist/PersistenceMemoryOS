#include "api.h"
#include <stdio.h>
void api_print(HANDLE windowHandle,char* text, Point location){
	int ebx;
	SystemCallSendData data;
	data.version = 1;
	data.type = SYSTEM_CALL_TYPE_PRINT;
	data.data.print.windowHandle = windowHandle;
	data.data.print.value = text;
	data.data.print.location = location;
	ebx = (int)(&data);
	__asm__ __volatile__  ("movl %0, %%ebx"::"r"(ebx): "ebx");
	__asm__ __volatile__  ("int $0x40");
	
}

void api_draw(HANDLE windowHandle){
	int ebx;
	SystemCallSendData data;
	data.version = 1;
	data.type = SYSTEM_CALL_TYPE_DRAW;
	data.data.draw.windowHandle = windowHandle;
	ebx = (int)(&data);
	__asm__ __volatile__  ("movl %0, %%ebx"::"r"(ebx): "ebx");
	__asm__ __volatile__  ("int $0x40");
	
}

void api_clear(HANDLE windowHandle){
	int ebx;
	SystemCallSendData data;
	data.version = 1;
	data.type = SYSTEM_CALL_TYPE_CLEAR;
	data.data.clear.windowHandle = windowHandle;
	ebx = (int)(&data);
	__asm__ __volatile__  ("movl %0, %%ebx"::"r"(ebx): "ebx");
	__asm__ __volatile__  ("int $0x40");
	
}

void api_addEvent(WaitEventData* eventData, HANDLE *handle){
	int ebx;
	SystemCallSendData data;
	AddEventSystemCallReceiveData result;
	data.version = 1;
	data.type = SYSTEM_CALL_TYPE_ADD_EVENT;
	data.data.addEvent = *eventData;
	data.result.addEvent = &result;
	ebx = (int)(&data);
	__asm__ __volatile__  ("movl %0, %%ebx"::"r"(ebx): "ebx");
	__asm__ __volatile__  ("int $0x40");
	*handle = result.handle;
}

HANDLE api_peekEventQueue(GetEventData* eventData){
	int ebx;
	SystemCallSendData data;
	data.version = 1;
	data.type = SYSTEM_CALL_TYPE_PEEK_EVENT_QUEUE;
	data.result.peekEventQueue = eventData;
	ebx = (int)(&data);
	__asm__ __volatile__  ("movl %0, %%ebx"::"r"(ebx): "ebx");
	__asm__ __volatile__  ("int $0x40");
	return eventData->handle;
}

HANDLE api_waitEventQueue(GetEventData* eventData){
	int ebx;
	SystemCallSendData data;
	data.version = 1;
	data.type = SYSTEM_CALL_TYPE_WAIT_EVENT_QUEUE;
	data.result.waitEventQueue = eventData;
	ebx = (int)(&data);
	__asm__ __volatile__  ("movl %0, %%ebx"::"r"(ebx): "ebx");
	__asm__ __volatile__  ("int $0x40");
	return eventData->handle;
}

void api_createWindow(Point location, Size size, HANDLE* handle){
	int ebx;
	CreateWindowSystemCallReceiveData result;
	SystemCallSendData data;
	data.version = 1;
	data.type = SYSTEM_CALL_TYPE_CREATE_WINDOW;
	data.data.createWindow.size = size;
	data.data.createWindow.location = location;
	data.result.createWindow = &result;
	ebx = (int)(&data);
	__asm__ __volatile__  ("movl %0, %%ebx"::"r"(ebx): "ebx");
	__asm__ __volatile__  ("int $0x40");
	*handle = (HANDLE)result.windowHandle;
	
}


