#include "screen.h"
#include "common.h"
#include "gdt.h"
#include "api.h"
#include "app/app.h"
#include "int.h"
#include "common.h"
#include "window.h"

extern SegmentDescriptor gdt[GDT_COUNT];
extern Task *nowTask;
extern Task tasks[100];
extern SegmentDescriptor gdt[GDT_COUNT];

extern ImageSheet *windowImageSheet;
extern TaskListCell *taskReadyList;


void CallAPI(SystemCallSendData* ebx){
	ADDRESS codeBase = (ADDRESS)getBaseAddressFromSegmentDescriptor(nowTask->dataSegmentIndex);
	SystemCallSendData* originEbx;
	originEbx = (SystemCallSendData*)((char*)ebx + codeBase);
	
	Point p;
	TaskListCell *ppp;
	p.x = p.y = 0;
	
	
	switch(originEbx->type){
	case SYSTEM_CALL_TYPE_PRINT:
		{
			char* text = originEbx->data.print.value + codeBase;
			Window *window;
			window = convertWindowHandle(originEbx->data.print.windowHandle);
			Point location;
			Color color;
			location = originEbx->data.print.location;
			color = COLOR_BLACK;
			drawString(window->innerSheet, text, location, color);
			break;
		}
	case SYSTEM_CALL_TYPE_DRAW:
		{
			//TODO 指定されたWindowのみをDrawする
//			draw();
			Window *window;
			window = convertWindowHandle(originEbx->data.print.windowHandle);
			transferBackBuffer(window->innerSheet);
			break;
		}
	case SYSTEM_CALL_TYPE_CLEAR:
		{
			Window *window;
			window = convertWindowHandle(originEbx->data.clear.windowHandle);
			clearWindow(window, COLOR_WHITE);
			break;
		}
	case SYSTEM_CALL_TYPE_ADD_EVENT:
		{
			WaitEventTask eventData;
			
			AddEventSystemCallReceiveData* result;
			result = (AddEventSystemCallReceiveData*)((char*)originEbx->result.addEvent + codeBase);
			
			
			switch(originEbx->data.addEvent.type){
			case WAIT_EVENT_TYPE_TIMER:
				eventData.type = EVENT_TYPE_TIMER;
				eventData.task = nowTask;
				eventData.value.timer.interval = originEbx->data.addEvent.data.timer.interval;
				eventData.value.timer.start = 0; /*とりあえず0を入れておく、将来的には、この地点でのタイマの値を入れる。最初の呼び出しで、何秒後に来るかわからなくなるのを防ぐ*/
				result->handle = addWaitEventTask(&eventData);
				break;
			case WAIT_EVENT_TYPE_MOUSE_UP:
			case WAIT_EVENT_TYPE_MOUSE_DOWN:
			case WAIT_EVENT_TYPE_MOUSE_MOVE:
				if(originEbx->data.addEvent.type == WAIT_EVENT_TYPE_MOUSE_UP)
					eventData.type = EVENT_TYPE_MOUSE_UP;
				else if(originEbx->data.addEvent.type == WAIT_EVENT_TYPE_MOUSE_DOWN)
					eventData.type = EVENT_TYPE_MOUSE_DOWN;
				else if(originEbx->data.addEvent.type == WAIT_EVENT_TYPE_MOUSE_MOVE)
					eventData.type = EVENT_TYPE_MOUSE_MOVE;
				eventData.task = nowTask;
				eventData.value.mouse.windowHandle = originEbx->data.addEvent.data.mouse.windowHandle;
				result->handle = addWaitEventTask(&eventData);
				break;
			case WAIT_EVENT_TYPE_KEYDOWN:
			case WAIT_EVENT_TYPE_KEYUP:
				if(originEbx->data.addEvent.type == WAIT_EVENT_TYPE_KEYUP)
					eventData.type = EVENT_TYPE_KEYUP;
				else if(originEbx->data.addEvent.type == WAIT_EVENT_TYPE_KEYDOWN)
					eventData.type = EVENT_TYPE_KEYDOWN;
				eventData.task = nowTask;
				eventData.value.key.windowHandle = originEbx->data.addEvent.data.key.windowHandle;
				result->handle = addWaitEventTask(&eventData);
				break;
			case WAIT_EVENT_TYPE_NONE:
				break;
			}
			break;
		}
	/*
	case SYSTEM_CALL_TYPE_PEEK_EVENT_QUEUE:
		{
			TaskEventData eventData;
			originEbx->result.peekEventQueue = (GetEventData*)((char*)originEbx->result.peekEventQueue + codeBase);
			if((originEbx->result.peekEventQueue->handle = deleteTaskEventQueue(nowTask, &eventData)) == (HANDLE)NULL){
				originEbx->result.peekEventQueue->type = WAIT_EVENT_TYPE_NONE;
				return;
			}
			switch(eventData.type){
			case EVENT_TYPE_TIMER:
				originEbx->result.peekEventQueue->type = WAIT_EVENT_TYPE_TIMER;
				break;
			default:
				originEbx->result.peekEventQueue->type = WAIT_EVENT_TYPE_NONE;
				break;
			}
			break;
		}
	*/
	case SYSTEM_CALL_TYPE_PEEK_EVENT_QUEUE:
	case SYSTEM_CALL_TYPE_WAIT_EVENT_QUEUE:
		{
			TaskEventData eventData;
			originEbx->result.waitEventQueue = (GetEventData*)((char*)originEbx->result.waitEventQueue + codeBase);
			while((originEbx->result.peekEventQueue->handle = deleteTaskEventQueue(nowTask, &eventData)) == (HANDLE)NULL){
				if(originEbx->type == SYSTEM_CALL_TYPE_PEEK_EVENT_QUEUE){
					originEbx->result.peekEventQueue->type = WAIT_EVENT_TYPE_NONE;
					return;
				}
				sleepTask(nowTask);
			}
			switch(eventData.type){
			case EVENT_TYPE_TIMER:
				originEbx->result.waitEventQueue->type = WAIT_EVENT_TYPE_TIMER;
				break;
			case EVENT_TYPE_MOUSE_UP:
			case EVENT_TYPE_MOUSE_DOWN:
			case EVENT_TYPE_MOUSE_MOVE:
				if(eventData.type == EVENT_TYPE_MOUSE_UP) 
					originEbx->result.waitEventQueue->type = WAIT_EVENT_TYPE_MOUSE_UP;
				else if(eventData.type == EVENT_TYPE_MOUSE_DOWN) 
					originEbx->result.waitEventQueue->type = WAIT_EVENT_TYPE_MOUSE_DOWN;
				else if(eventData.type == EVENT_TYPE_MOUSE_MOVE) 
					originEbx->result.waitEventQueue->type = WAIT_EVENT_TYPE_MOUSE_MOVE;
				originEbx->result.waitEventQueue->data.mouse.location = eventData.data.mouse.location;
				originEbx->result.waitEventQueue->data.mouse.buttons = eventData.data.mouse.buttons;
				break;
			case EVENT_TYPE_KEYUP:
			case EVENT_TYPE_KEYDOWN:
				if(eventData.type == EVENT_TYPE_KEYUP) 
					originEbx->result.waitEventQueue->type = WAIT_EVENT_TYPE_KEYUP;
				else if(eventData.type == EVENT_TYPE_KEYDOWN) 
					originEbx->result.waitEventQueue->type = WAIT_EVENT_TYPE_KEYDOWN;
				originEbx->result.waitEventQueue->data.key.keyCode = eventData.data.key.keyCode;
				originEbx->result.waitEventQueue->data.key.keyChar = eventData.data.key.keyChar;
				break;
			default:
				originEbx->result.waitEventQueue->type = WAIT_EVENT_TYPE_NONE;
				break;
			}
			break;
		}
	case SYSTEM_CALL_TYPE_CREATE_WINDOW:
		{
			Point point;
			CreateWindowSystemCallReceiveData *result;
			point.x = point.y = 0;
			Window *window; // = allocMemoryType(Window);
			initWindow(&window, originEbx->data.createWindow.location, originEbx->data.createWindow.size);
			
			result = (CreateWindowSystemCallReceiveData*)((char*)originEbx->result.createWindow + codeBase);
			result->windowHandle = (HANDLE)window;
			break;
			
		}
	}
}