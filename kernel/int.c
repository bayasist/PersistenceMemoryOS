#include "idt.h"
#include "screen.h"
#include "int.h"
#include "app/app.h"
#include "memory.h"
#include "common.h"
#include "app/apiData.h"
#include "mouse.h"
#include "keyboard.h"

WaitEventTaskListCell *waitTimerTaskList;
WaitEventTaskListCell *waitMouseUpTaskList;
WaitEventTaskListCell *waitMouseDownTaskList;
WaitEventTaskListCell *waitMouseMoveTaskList;
WaitEventTaskListCell *waitKeyupTaskList;
WaitEventTaskListCell *waitKeydownTaskList;
INIT_LIST_FUNCTION(initWaitEventTaskList, WaitEventTaskListCell)

TaskEventQueueListCell *taskEventQueueList;


INSERT_QUEUE_FUNCTION(insertEventQueue, TaskEventQueueCell, TaskEventData)
DELETE_QUEUE_FUNCTION(deleteEventQueue, TaskEventQueueCell, TaskEventData)
INIT_LIST_FUNCTION(initEventQueue, TaskEventQueueCell)


GET_KEY_VALUE_PAIR(getTaskEventQueueCell, TaskEventQueueListCell, Task*, TaskEventQueueCell*)
INSERT_KEY_VALUE_PAIR(insertTaskEventQueueCell, getTaskEventQueueCell, TaskEventQueueListCell, Task*, TaskEventQueueCell*);

TaskListCell *taskWaitList;
int insertTaskEventQueue(Task* task, TaskEventData data){
	TaskEventQueueCell *q;
	if(!getTaskEventQueueCell(taskEventQueueList, task, &q)){
		return -1;
	}
	insertEventQueue(q, data);
	
	/*
	TaskEventQueueListCell *p, *q;
	TaskEventQueueCell *p2;
	q = NULL;
	for(p = taskEventQueueList; p->next != NULL; p = p->next){
		if(p->next->task == task){
			q = p->next;
			break;
		}
	}
	if(q == NULL){
		return -1;
	}
	
	for(p2 = q->value; p2->next != NULL; p2 = p2->next);
	p2->next = allocMemoryType(TaskEventQueueCell);
	p2->next->data = data;
	p2->next->next = NULL;
	*/
	return 0;
}

HANDLE deleteTaskEventQueue(Task* task,TaskEventData *data){
	TaskEventQueueCell *q;
	if(!getTaskEventQueueCell(taskEventQueueList, task, &q)){
		return (HANDLE)NULL;
	}
	
	
	if(!deleteEventQueue(q, data)){
		return (HANDLE)NULL;
	}
	
	
	/*
	TaskEventQueueListCell *p, *q;
	TaskEventQueueCell *p2;
	q = NULL;
	for(p = taskEventQueueList; p->next != NULL; p = p->next){
		if(p->next->task == task){
			q = p->next;
			break;
		}
	}
	if(q == NULL){
		return (HANDLE)NULL;
	}
	//キューが空
	if(q->value->next == NULL){
		return (HANDLE)NULL;
	}
	
	*data = q->value->next->data;
	p2 = q->value->next->next;
	freeMemory(q->value->next, sizeof(TaskEventQueueListCell));
	q->value->next = p2;
	*/
	return data->handle;
}


void initTaskEventQueue(Task* task){
	TaskEventQueueCell *tmp;
	initEventQueue(&tmp);
	//tmp = allocMemoryType(TaskEventQueueCell);
	//tmp->next = NULL;
	if(!insertTaskEventQueueCell(taskEventQueueList, task, tmp)){
		freeMemory(tmp, sizeof(TaskEventQueueCell));
	}
	
	
	
	
	/*
	TaskEventQueueListCell *p;
	for(p = taskEventQueueList; p->next != NULL; p = p->next){
		if(p->next->task == task){
			return;
		}
	}
	
	p->next = allocMemoryType(TaskEventQueueListCell);
	p->next->task = task;
	p->next->next = NULL;
	
	p->next->value = allocMemoryType(TaskEventQueueCell);
	p->next->value->next = NULL;
	*/
}
INIT_LIST_FUNCTION(inittaskEventQueueListt, TaskEventQueueListCell)

int initInt(void){
	//タスクのリストにダミーセルを作成
	initWaitEventTaskList(&waitTimerTaskList);
	initWaitEventTaskList(&waitMouseUpTaskList);
	initWaitEventTaskList(&waitMouseDownTaskList);
	initWaitEventTaskList(&waitMouseMoveTaskList);
	initWaitEventTaskList(&waitKeyupTaskList);
	initWaitEventTaskList(&waitKeydownTaskList);
	/*
	waitTimerTaskList = allocMemoryType(WaitEventTaskListCell);
	waitTimerTaskList->next = NULL;
	waitMouseUpTaskList = allocMemoryType(WaitEventTaskListCell);
	waitMouseUpTaskList->next = NULL;
	waitMouseDownTaskList = allocMemoryType(WaitEventTaskListCell);
	waitMouseDownTaskList->next = NULL;
	waitMouseMoveTaskList = allocMemoryType(WaitEventTaskListCell);
	waitMouseMoveTaskList->next = NULL;
	waitKeyupTaskList = allocMemoryType(WaitEventTaskListCell);
	waitKeyupTaskList->next = NULL;
	waitKeydownTaskList = allocMemoryType(WaitEventTaskListCell);
	waitKeydownTaskList->next = NULL;
	
	*/
	
	//タスクのリストにダミーセルを作成
	inittaskEventQueueListt(&taskEventQueueList);
	/*
	taskEventQueueList = allocMemoryType(TaskEventQueueListCell);
	taskEventQueueList->next = NULL;
	*/
	
	return 0;
}
ADD_LIST_FUNCTION(addWaitEventTask2, WaitEventTaskListCell, WaitEventTask)
/*
HANDLE addWaitEventTask2(WaitEventTaskListCell *list, WaitEventTask *waitEventTask){
	WaitEventTaskListCell *p;
	HANDLE result;
	for(p = list; p->next != NULL; p = p->next);
	p->next = allocMemoryType(WaitEventTaskListCell);
	result = (HANDLE)(p->next);
	p->next->next = NULL;
	p->next->value = *waitEventTask;
	return result;
}
*/

HANDLE addWaitEventTask(WaitEventTask *waitEventTask){
	HANDLE result;
	WaitEventTaskListCell *list;
	
	switch(waitEventTask->type){
	case EVENT_TYPE_TIMER:
		//result = addWaitEventTask2(waitTimerTaskList, waitEventTask);
		list = waitTimerTaskList;
		break;
	case EVENT_TYPE_MOUSE_UP:
		list = waitMouseUpTaskList;
		break;
	case EVENT_TYPE_MOUSE_DOWN:
		list = waitMouseDownTaskList;
		break;
	case EVENT_TYPE_MOUSE_MOVE:
		list = waitMouseMoveTaskList;
		break;
	case EVENT_TYPE_KEYUP:
		list = waitKeyupTaskList;
		break;
	case EVENT_TYPE_KEYDOWN:
		list = waitKeydownTaskList;
		break;
	default:
		return (HANDLE)0;
		break;
	}
	result = (HANDLE)addWaitEventTask2(list, *waitEventTask);
	return result;
}


void timerEventTask(long long count){
	WaitEventTaskListCell *p;
	for(p = waitTimerTaskList; p->next != NULL; p = p->next){
		Task *task = p->next->value.task;
		if((count - p->next->value.value.timer.start) % p->next->value.value.timer.interval == 0){
			TaskEventData data;
			data.type = EVENT_TYPE_TIMER;
			data.handle = (HANDLE)p->next;
			insertTaskEventQueue(task, data);
			//タスクが寝ていたら起こす
			if(existTask(taskWaitList, task)){
				wakeUpTask(task);
			}
		}
	}
}

void mouseEventTask(Window *window, EventType eventType, Point location, MouseButtons buttons){
	WaitEventTaskListCell *p;
	switch(eventType){
	case EVENT_TYPE_MOUSE_UP:
		p = waitMouseUpTaskList;
		break;
	case EVENT_TYPE_MOUSE_DOWN:
		p = waitMouseDownTaskList;
		break;
	case EVENT_TYPE_MOUSE_MOVE:
		p = waitMouseMoveTaskList;
		break;
	default:
		return;
	}
	for(; p->next != NULL; p = p->next){
		Task *task = p->next->value.task;
		if(p->next->value.value.mouse.windowHandle == (HANDLE)window){
			TaskEventData data;
			data.type = eventType;
			data.handle = (HANDLE)p->next;
			data.data.mouse.location = location;
			data.data.mouse.buttons = buttons;
			insertTaskEventQueue(task, data);
			//タスクが寝ていたら起こす
			if(existTask(taskWaitList, task)){
				wakeUpTask(task);
			}
		}
	}
}

void mouseUpEventTask(Window *window, Point location, MouseButtons buttons){
	mouseEventTask(window, EVENT_TYPE_MOUSE_UP, location, buttons);
}
void mouseDownEventTask(Window *window, Point location, MouseButtons buttons){
	mouseEventTask(window, EVENT_TYPE_MOUSE_DOWN, location, buttons);
}
void mouseMoveEventTask(Window *window, Point location, MouseButtons buttons){
	mouseEventTask(window, EVENT_TYPE_MOUSE_MOVE, location, buttons);
}


extern Window *mainImageSheet;
void keyEventTask(Window *window, EventType eventType, KeyCode keyCode, char keyChar){
	WaitEventTaskListCell *p;
	//Point po;
	//po.x = po.y = 0;
	switch(eventType){
	case EVENT_TYPE_KEYUP:
		p = waitKeyupTaskList;
		break;
	case EVENT_TYPE_KEYDOWN:
		p = waitKeydownTaskList;
		break;
	default:
		return;
	}
	//putInt(mainImageSheet, (unsigned int)window, po, COLOR_BLACK);
	//po.y += 20;
	//putInt(mainImageSheet, (unsigned int)(p->next->value.value.key.windowHandle), po, COLOR_BLACK);
	for(; p->next != NULL; p = p->next){
		Task *task = p->next->value.task;
		if(p->next->value.value.key.windowHandle == (HANDLE)window){
			TaskEventData data;
			data.type = eventType;
			data.handle = (HANDLE)p->next;
			data.data.key.keyCode = keyCode;
			data.data.key.keyChar = keyChar;
			insertTaskEventQueue(task, data);
			//タスクが寝ていたら起こす
			if(existTask(taskWaitList, task)){
				wakeUpTask(task);
			}
		}
	}
}

void keyupEventTask(Window *window, KeyCode keyCode, char keyChar){
	keyEventTask(window, EVENT_TYPE_KEYUP, keyCode, keyChar);
}
void keydownEventTask(Window *window, KeyCode keyCode, char keyChar){
	keyEventTask(window, EVENT_TYPE_KEYDOWN, keyCode, keyChar);
}


extern ImageSheet *windowImageSheet;



//----------------TESTここから----------------------------
/*
KEY_VALUE_PAIR_DEFINE(____Test, int, int);
____Test *_____test;
GET_KEY_VALUE_PAIR(_____getTest, ____Test, int, int)
INSERT_KEY_VALUE_PAIR(_____insertTest, _____getTest, ____Test, int, int)
int __test;
*/
//----------------TESTここまで----------------------------

extern TaskListCell *taskReadyList;
extern Task *eventTask;
extern int allowTaskSwitch;
extern long long taskSwitchCount;
void eventMainLoop(void){
	Point point;
	long long timerCount = 0;
	point.x = point.y = 0;
	cli();
	
	clearScreen(windowImageSheet, COLOR_WHITE);
	transferBackBuffer(windowImageSheet);
	/*
	putMemoryInfo(windowImageSheet);
	_____test = allocMemoryType(____Test);
	_____test->next = NULL;
	_____insertTest(_____test, 1, 10);
	_____insertTest(_____test, 2, 20);
	_____insertTest(_____test, 3, 30);
	_____getTest(_____test, 2, &__test);
	putInt(windowImageSheet, getTaskCount(taskReadyList), point, COLOR_BLACK);
	*/
	
	
	while(1){
		EventArg value;
		int isHlt = !readEventBuf(&value);
		int data = value.data;
		
		
		
		switch(value.type){
		case EVENT_TYPE_NONE:
			break;
		case EVENT_TYPE_KEYBOARD:
			{
				allowTaskSwitch = 0;
				keyboardMain(data);
				allowTaskSwitch = 1;
				break;
			}
		case EVENT_TYPE_MOUSE:
			{
				allowTaskSwitch = 0;
				mouseMain(data);
				allowTaskSwitch = 1;
				break;
			}
		case EVENT_TYPE_TIMER:
			{
				timerCount++;
				allowTaskSwitch = 0;
				timerEventTask(timerCount);
				allowTaskSwitch = 1;
				break;
			}
		default:
			break;
		}
		
		
		
		if(isHlt){
			clearScreen(windowImageSheet, COLOR_WHITE);
			point.y = 0;
			putInt(windowImageSheet, timerCount, point, COLOR_BLACK);
			point.y += 16;
			putInt(windowImageSheet, taskSwitchCount, point, COLOR_BLACK);
			transferBackBuffer(windowImageSheet);
			draw();
			if(getTaskCount(taskReadyList) > 0){
				sti();
				//__asm__ __volatile__  ("hlt");
				sleepTask(eventTask);
				cli();
			}else{
				//実行可能状態のプロセスがなければhltする
				sti();
				__asm__ __volatile__  ("hlt");
				cli();
			}
			
		}
		
		
		
	}
	
}