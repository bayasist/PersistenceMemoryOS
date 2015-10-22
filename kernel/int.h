#include "app/app.h"
#include "idt.h"
#include "api.h"
#include "window.h"
#include "mouse.h"

#ifndef DEFINED_INT_FILE
#define DEFINED_INT_FILE 1
int initInt(void);


typedef struct{
	int interval;
	int start;
} WaitTimerTask;

typedef struct{
	HANDLE windowHandle;
} WaitMouseTask;

typedef struct{
	HANDLE windowHandle;
} WaitKeyTask;


typedef struct{
	EventType type;
	Task* task;
	union{
		WaitTimerTask timer;
		WaitMouseTask mouse;
		WaitKeyTask key;
	} value;
} WaitEventTask;
/*
typedef struct WaitEventTaskListCell{
	struct WaitEventTaskListCell *next;
	WaitEventTask value;
} WaitEventTaskListCell;
*/
LIST_DEFINE(WaitEventTaskListCell, WaitEventTask);

HANDLE addWaitEventTask(WaitEventTask *waitEventTask);


typedef struct{
	EventType type;
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
} TaskEventData;

/*
typedef struct TaskEventQueueCell{
	struct TaskEventQueueCell* next;
	TaskEventData data;
} TaskEventQueueCell;
*/
LIST_DEFINE(TaskEventQueueCell, TaskEventData);
KEY_VALUE_PAIR_DEFINE(TaskEventQueueListCell, Task*, TaskEventQueueCell*);

/*
typedef struct TaskEventQueueListCell{
	struct TaskEventQueueListCell* next;
	Task *task;
	TaskEventQueueCell *value;
} TaskEventQueueListCell;
*/

void initTaskEventQueue(Task* task);
HANDLE deleteTaskEventQueue(Task* task,TaskEventData *data);
void timerEventTask(long long count);
void mouseUpEventTask(Window *window, Point, MouseButtons);
void mouseDownEventTask(Window *window, Point, MouseButtons);
void mouseMoveEventTask(Window *window, Point, MouseButtons);
void eventMainLoop(void);
void keyupEventTask(Window*, KeyCode, char);
void keydownEventTask(Window*, KeyCode, char);

#endif