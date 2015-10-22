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
	idt->igd.descriptorPlivilegeLevel = descriptorPlivilegeLevel;	//�������x��
	return ;
}


void initKeyboard(void);
void enableMouseMode(void);
int initIdt(void){
	initEvent();
//	cli();	//���荞�݋֎~
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
//	sti();	//���荞�݋���
	return 0;
}

int initPic(void){
	
	//�^�C�}�p
	outPortByte(0x43, 0x34);
	outPortByte(0x40, 0x9c);
	outPortByte(0x40, 0x2e);
	
	
	
	outPortByte( 0x20, 0x11 );	/* �}�X�^PIC�̃R�}���h���W�X�^��ICW1����������		*/
	outPortByte( 0xA0, 0x11 );	/* �X���[�uPIC�̃R�}���h���W�X�^��ICW1����������	*/	
	outPortByte( 0x21, 0x20 );	/* �}�X�^PIC�̃f�[�^���W�X�^��ICW2����������	*/
	outPortByte( 0xA1, 0x28 );	/* �X���[�uPIC�̃f�[�^���W�X�^��ICW2����������	*/
	outPortByte( 0x21, 0x04 );	/* �}�X�^PIC�̃f�[�^���W�X�^��ICW3����������	*/
	outPortByte( 0xA1, 0x02 );	/* �X���[�uPIC�̃f�[�^���W�X�^��ICW3����������	*/
	outPortByte( 0x21, 0x01 );	/* �}�X�^PIC�̃f�[�^���W�X�^��ICW4����������	*/
	outPortByte( 0xA1, 0x01 );	/* �X���[�uPIC�̃f�[�^���W�X�^��ICW4����������	*/
	
	/* PIC�@�������֐�����	*/
	/* --------------------------------------------------------------------- */
	/* 	send IMR ot PIC							 */
	/* --------------------------------------------------------------------- */
	//outPortByte( 0x21, (~0x01) & (~0x02 )	);
	
	//���荞�݋���
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
	�C�x���g�o�b�t�@��ǂݍ��݂܂��B
	�������Œl��Ԃ��܂��B
	�܂��A�����ǂݍ���ŁA�܂����̃o�b�t�@�����݂���ꍇ��1��Ԃ��܂��B
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
	
	//�C�x���g�I���ʒm
	outPortByte( 0x20, 0x60 + 0 );
	writeEventBuf(EVENT_TYPE_TIMER, 0);
	
	
	//�C�x���g�Ď��p�v���Z�X�������Ă���΋N����(�ŗD����s)
	if(initedApp && existTask(taskWaitList, eventTask)){
		wakeUpTaskFirst(eventTask);
		/*
		lastTaskSwitchTimer = timerCount;
		taskSwitchCount++;
		taskSwitch();
		*/
	}
	
	//�v���Z�X�؂�ւ�
	//if(timerCount % TASK_SWITCH_TIME_SLICE == 0){
	if(initedApp && timerCount - lastTaskSwitchTimer > TASK_SWITCH_TIME_SLICE && allowTaskSwitch){
		lastTaskSwitchTimer = timerCount;
		taskSwitchCount++;
		taskSwitch();
	}
}

void keyboardEvent(int *esp){
	int data;
	//�C�x���g�I���ʒm
	outPortByte( 0x20, 0x60 + 1 );
	data = inPortByte(0x0060);
	writeEventBuf(EVENT_TYPE_KEYBOARD, data);
	
	//�C�x���g�Ď��p�v���Z�X�������Ă���΋N����(�ŗD����s)
	if(initedApp && existTask(taskWaitList, eventTask)){
		wakeUpTaskFirst(eventTask);
	}
}


void mouseEvent(int *esp){
	int data;
	Point p;
	p.x = p.y = 0;
	//�C�x���g�I���ʒm
	outPortByte( 0xA0, 0x60 + 4 );
	outPortByte( 0x20, 0x60 + 2 );
	data = inPortByte(0x0060);
	writeEventBuf(EVENT_TYPE_MOUSE, data);
	
	//�C�x���g�Ď��p�v���Z�X�������Ă���΋N����(�ŗD����s)
	if(initedApp && existTask(taskWaitList, eventTask)){
		wakeUpTaskFirst(eventTask);
	}
}

