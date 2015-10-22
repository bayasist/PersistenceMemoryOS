#include "../gdt.h"
#include "../memory.h"

#ifndef DEFINED_APP_FILE
	#define DEFINED_APP_FILE 1
	typedef struct{
		int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
		int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
		int es, cs, ss, ds, fs, gs;
		int ldtr, iomap;
	} __attribute__ ((packed)) TSS32;


	#define DEFAULT_STACK_SIZE (64 * 1024)
	#define STARTER_TASK_DEFAULT_STACK_SIZE (1024)
	typedef struct{
		unsigned version;
		unsigned startAddress;
		unsigned empty[64 - 3];
		char appCode[4];
	} ApplicationHeader;
	typedef struct{
		ApplicationHeader *header;
		TSS32 tss;
		GdtIndex codeSegmentIndex;
		GdtIndex dataSegmentIndex;
		GdtIndex tssIndex;
		//char* startAddress;	/*アプリケーションが始まる物理アドレス*/
		//char* startStackAddress;
		MemoryRange codeMemoryRange;
		MemoryRange stackMemoryRange;
		MemoryRange starterTaskMemoryRange;
	} Task;

/*
	typedef struct TaskListCell{
		Task *value;
		struct TaskListCell* next;
	} TaskListCell;
*/
LIST_DEFINE(TaskListCell, Task*)

	void _asm_taskswitch(int eip, int gdtIndex);
	void _asm_load_tr(int index);
	void loadTr(int gdtIndex);
	void taskswitch(Task*);
	int taskSwitch(void);
	void initApp(void);
	TaskListCell* taskQueueInsert(TaskListCell *list,Task *value);
	void taskQueueDelete2(TaskListCell *list,Task **value);
	int getTaskCount(TaskListCell *list);
	void _asm_launch_task(int eip, int cs, int esp, int ds, int *tss_esp0);
void wakeUpTask(Task *task);
int sleepTask(Task *task);
int existTask(TaskListCell *list, Task *value);
void taskQueueInsertFirst(TaskListCell *list, Task *task);
void wakeUpTaskFirst(Task *task);
#endif