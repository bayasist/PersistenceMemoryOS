#include "../common.h"
#include "../mouse.h"
#include "../keyboard.h"
#ifndef API_DATA_DEFINED
#define API_DATA_DEFINED 1

typedef ADDRESS HANDLE;
typedef enum{
	SYSTEM_CALL_TYPE_PRINT = 100,
	SYSTEM_CALL_TYPE_CLEAR,
	SYSTEM_CALL_TYPE_CREATE_WINDOW,
	SYSTEM_CALL_TYPE_DRAW  = 200,
	SYSTEM_CALL_TYPE_ADD_EVENT = 1,
	SYSTEM_CALL_TYPE_PEEK_EVENT_QUEUE = 2,
	SYSTEM_CALL_TYPE_WAIT_EVENT_QUEUE,
} SYSTEM_CALL_TYPE;

typedef enum{
	WAIT_EVENT_TYPE_NONE = 0,
	WAIT_EVENT_TYPE_TIMER = 1,
	WAIT_EVENT_TYPE_MOUSE_UP,
	WAIT_EVENT_TYPE_MOUSE_DOWN,
	WAIT_EVENT_TYPE_MOUSE_MOVE,
	WAIT_EVENT_TYPE_KEYUP,
	WAIT_EVENT_TYPE_KEYDOWN,
} WaitEventType;

typedef struct{
	int interval;
} WaitTimerEventData;

typedef struct{
	HANDLE windowHandle;
} MouseEventData;

typedef struct{
	HANDLE windowHandle;
} KeyEventData;


typedef struct{
	WaitEventType type;
	union{
		WaitTimerEventData timer;
		MouseEventData mouse;
		KeyEventData key;
	} data;
} WaitEventData;


typedef struct{
	WaitEventType type;
	HANDLE handle;
	union{
		struct{
			Point location;
			MouseButtons buttons;
		} mouse;
		struct{
			KeyCode keyCode;
			char keyChar;
		} key;
	} data;
} GetEventData;

typedef struct{
	char *value;
	Point location;
	HANDLE windowHandle;
} PrintSystemCallSendData;

typedef struct{
	HANDLE windowHandle;
} ClearSystemCallSendData;

typedef struct{
	HANDLE windowHandle;
} DrawSystemCallSendData;


typedef struct{
	Point location;
	Size size;
} CreateWindowSystemCallSendData;

typedef struct{
	HANDLE windowHandle;
} CreateWindowSystemCallReceiveData;

typedef struct{
	HANDLE handle;
} AddEventSystemCallReceiveData;

typedef struct{
	int version;
	SYSTEM_CALL_TYPE type;
	union{
		PrintSystemCallSendData print;
		WaitEventData addEvent;
		CreateWindowSystemCallSendData createWindow;
		ClearSystemCallSendData clear;
		ClearSystemCallSendData draw;
	} data;
	union{
		AddEventSystemCallReceiveData *addEvent;
		GetEventData *peekEventQueue;
		GetEventData *waitEventQueue;
		CreateWindowSystemCallReceiveData *createWindow;
	} result;
} SystemCallSendData;
#endif