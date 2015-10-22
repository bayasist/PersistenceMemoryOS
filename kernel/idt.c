#include "idt.h"
#include "common.h"
#include "screen.h"
#include "memory.h"
#include "api.h"
#include "app/app.h"
#include "int.h"

IdtDescriptorTable idt;
InterruptDescriptorTableRegister idtr;
int timer_tick;
int initIdt(void);
int initPic(void);
EventBuf eventBuf;


int allowTaskSwitch;
long long lastTaskSwitchTimer;
long long taskSwitchCount;

long long timerCount;
int initEvent(void){
	eventBuf.nextWrite	= 0;
	eventBuf.nextRead	= 0;
	timerCount = 0;
	lastTaskSwitchTimer = 0;
	allowTaskSwitch = 1;
	taskSwitchCount = 0;
	return 0;
}
void setInterruptGateDescriptor(IdtDescriptor *idt, int base, unsigned short selector,GateDescriptorBits gateSize, PlivilegeLevel descriptorPlivilegeLevel, int present){
	idt->igd.type = INTERRUPT_GATE_DESCRIPTOR_TYPE;
	idt->igd.baseLo	= ( unsigned short )(   base & 0x0000FFFF );
	idt->igd.baseHi	= ( unsigned short )( ( base & 0xFFFF0000 ) >> 16 );
	idt->igd.gateSize	= gateSize;
	idt->igd.segmentPresentFlag	= present;
	idt->igd.segmentSelector	= selector;
	idt->igd.unused1	= 0;
	idt->igd.unused2	= 0;
	idt->igd.descriptorPlivilegeLevel = descriptorPlivilegeLevel;	//特権レベル
	return ;
}


void initKeyboard(void);
void enableMouseMode(void);
int initIdt(void){
	initEvent();
//	cli();	//割り込み禁止
	timer_tick = 0;
	setInterruptGateDescriptor(&idt[0x20 + 0]	, (int)_asm_timer_event		, 0x08, 	GATE_DESCRIPTOR_32BITS, PLIVILEGE_LEVEL_RING0, 1);
	setInterruptGateDescriptor(&idt[0x20 + 1]	, (int)_asm_keyboard_event	, 0x08, 	GATE_DESCRIPTOR_32BITS, PLIVILEGE_LEVEL_RING0, 1);
	setInterruptGateDescriptor(&idt[0x20 + 12]	, (int)_asm_mouse_event		, 0x08, 	GATE_DESCRIPTOR_32BITS, PLIVILEGE_LEVEL_RING0, 1);
	setInterruptGateDescriptor(&idt[0x40]		, (int)_asm_api				, 0x08, 	GATE_DESCRIPTOR_32BITS, PLIVILEGE_LEVEL_RING3, 1);
	idtr.size = IDT_COUNT * sizeof( IdtDescriptor );
	idtr.base	= ( IdtDescriptor *)idt;
	load_idt();
	initPic();
	initKeyboard();
	enableMouseMode();
//	sti();	//割り込み許可
	return 0;
}

int initPic(void){
	
	//タイマ用
	outPortByte(0x43, 0x34);
	outPortByte(0x40, 0x9c);
	outPortByte(0x40, 0x2e);
	
	
	
	outPortByte( 0x20, 0x11 );	/* マスタPICのコマンドレジスタにICW1を書き込み		*/
	outPortByte( 0xA0, 0x11 );	/* スレーブPICのコマンドレジスタにICW1を書き込み	*/	
	outPortByte( 0x21, 0x20 );	/* マスタPICのデータレジスタにICW2を書き込み	*/
	outPortByte( 0xA1, 0x28 );	/* スレーブPICのデータレジスタにICW2を書き込み	*/
	outPortByte( 0x21, 0x04 );	/* マスタPICのデータレジスタにICW3を書き込み	*/
	outPortByte( 0xA1, 0x02 );	/* スレーブPICのデータレジスタにICW3を書き込み	*/
	outPortByte( 0x21, 0x01 );	/* マスタPICのデータレジスタにICW4を書き込み	*/
	outPortByte( 0xA1, 0x01 );	/* スレーブPICのデータレジスタにICW4を書き込み	*/
	
	/* PIC　初期化関数続き	*/
	/* --------------------------------------------------------------------- */
	/* 	send IMR ot PIC							 */
	/* --------------------------------------------------------------------- */
	//outPortByte( 0x21, (~0x01) & (~0x02 )	);
	
	//割り込み許可
	outPortByte( 0x21, (~0x01) & (~0x02) & (~0x04));
	outPortByte( 0xA1, (~0x10));
	return 0;
}

void waitKbcSendReady(void){
	for(;;){
		if((inPortByte(0x0064) & 0x02) == 0){
			break;
		}
	}
	return;
}

void initKeyboard(void){
	waitKbcSendReady();
	outPortByte(0x0064, 0x60);
	waitKbcSendReady();
	outPortByte(0x0060, 0x47);
}

void enableMouseMode(void){
	waitKbcSendReady();
	outPortByte(0x0064, 0xd4);
	waitKbcSendReady();
	outPortByte(0x0060, 0xf4);
}

/*
	イベントバッファを読み込みます。
	第一引数で値を返します。
	また、これを読み込んで、また次のバッファが存在する場合は1を返します。
*/


int readEventBuf(EventArg* out){
	if(eventBuf.nextWrite == eventBuf.nextRead){
		out->type = EVENT_TYPE_NONE;
		return 0;
	}
	int readIndex = eventBuf.nextRead;
	*out = eventBuf.item[readIndex];
	if(readIndex + 1 >= EVENT_BUF_COUNT){
		eventBuf.nextRead = 0;
	}else{
		eventBuf.nextRead++;
	}
	return eventBuf.nextRead != eventBuf.nextWrite;
}


/*
int writeEventBuf(EventType type, EventData *data){
	int writeIndex;
	writeIndex = eventBuf.nextWrite;
	eventBuf.item[writeIndex].type = type;
	eventBuf.item[writeIndex].data = data;
	if(eventBuf.nextWrite + 1 >= EVENT_BUF_COUNT){
		eventBuf.nextWrite = 0;
	}else{
		eventBuf.nextWrite++;
	}
	return 0;
}
*/


int writeEventBuf(EventType type, int data){
	int writeIndex;
	writeIndex = eventBuf.nextWrite;
	eventBuf.item[writeIndex].type = type;
	eventBuf.item[writeIndex].data = data;
	if(eventBuf.nextWrite + 1 >= EVENT_BUF_COUNT){
		eventBuf.nextWrite = 0;
	}else{
		eventBuf.nextWrite++;
	}
	return 0;
}

extern Task *eventTask;
extern TaskListCell *taskWaitList;
extern Task tasks[100];
extern Task *nowTask;
#define TASK_SWITCH_TIME_SLICE 3
extern ImageSheet* mainImageSheet;

extern ImageSheet *windowImageSheet;
extern int initedApp;


void timerEvent(int *esp){
	timerCount++;
	
	//イベント終了通知
	outPortByte( 0x20, 0x60 + 0 );
	writeEventBuf(EVENT_TYPE_TIMER, 0);
	
	
	//イベント監視用プロセスが眠っていれば起こす(最優先実行)
	if(initedApp && existTask(taskWaitList, eventTask)){
		wakeUpTaskFirst(eventTask);
		/*
		lastTaskSwitchTimer = timerCount;
		taskSwitchCount++;
		taskSwitch();
		*/
	}
	
	//プロセス切り替え
	//if(timerCount % TASK_SWITCH_TIME_SLICE == 0){
	if(initedApp && timerCount - lastTaskSwitchTimer > TASK_SWITCH_TIME_SLICE && allowTaskSwitch){
		lastTaskSwitchTimer = timerCount;
		taskSwitchCount++;
		taskSwitch();
	}
}

void keyboardEvent(int *esp){
	int data;
	//イベント終了通知
	outPortByte( 0x20, 0x60 + 1 );
	data = inPortByte(0x0060);
	writeEventBuf(EVENT_TYPE_KEYBOARD, data);
	
	//イベント監視用プロセスが眠っていれば起こす(最優先実行)
	if(initedApp && existTask(taskWaitList, eventTask)){
		wakeUpTaskFirst(eventTask);
	}
}


void mouseEvent(int *esp){
	int data;
	Point p;
	p.x = p.y = 0;
	//イベント終了通知
	outPortByte( 0xA0, 0x60 + 4 );
	outPortByte( 0x20, 0x60 + 2 );
	data = inPortByte(0x0060);
	writeEventBuf(EVENT_TYPE_MOUSE, data);
	
	//イベント監視用プロセスが眠っていれば起こす(最優先実行)
	if(initedApp && existTask(taskWaitList, eventTask)){
		wakeUpTaskFirst(eventTask);
	}
}

