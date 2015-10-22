/*
#define	DEF_IDT_FLAGS_INTGATE_16BIT		0x06
#define	DEF_IDT_FLAGS_TSKGATE			0x05
#define	DEF_IDT_FLAGS_CALL_GATE			0x0C
#define	DEF_IDT_FLAGS_INTGATE_32BIT		0x0E
#define	DEF_IDT_FLAGS_TRPGATE			0x0F
#define	DEF_IDT_FLAGS_DPL_LV0			0x00
#define	DEF_IDT_FLAGS_DPL_LV1			0x20
#define	DEF_IDT_FLAGS_DPL_LV2			0x40
#define	DEF_IDT_FLAGS_DPL_LV3			0x60
#define	DEF_IDT_FLAGS_PRESENT			0x80

#define	DEF_IDT_INT_SELECTOR			0x08
*/

#ifndef DEFINED_IDT_FILE
#define DEFINED_IDT_FILE 1
typedef enum{
	TASK_GATE_DESCRIPTOR_TYPE = 5,
	INTERRUPT_GATE_DESCRIPTOR_TYPE = 6,
	TRAP_GATE_DESCRIPTOR_TYPE = 7
} GateDiscriptorType;

typedef enum{
	GATE_DESCRIPTOR_32BITS = 1,
	GATE_DESCRIPTOR_16BITS = 0
} GateDescriptorBits;

typedef enum{
	PLIVILEGE_LEVEL_RING0 = 0,
	PLIVILEGE_LEVEL_RING1 = 1,
	PLIVILEGE_LEVEL_RING2 = 2,
	PLIVILEGE_LEVEL_RING3 = 3,
} PlivilegeLevel;
typedef struct{
	unsigned int unused1:16;
	unsigned int segmentSelector:16;
	unsigned int unused2:8;
	GateDiscriptorType type:3;
	unsigned int unused3:2;
	PlivilegeLevel descriptorPlivilegeLevel:2;
	unsigned int segmentPresentFlag:1;
	unsigned int unused4:16;
} __attribute__ ((packed)) TaskGateDescriptor;

typedef struct{
	unsigned int baseLo:16;
	unsigned int segmentSelector:16;
	unsigned int unused1:8;
	GateDiscriptorType type:3;
	GateDescriptorBits gateSize:1;
	unsigned int unused2:1;
	PlivilegeLevel descriptorPlivilegeLevel:2;
	unsigned int segmentPresentFlag:1;
	unsigned int baseHi:16;
} __attribute__ ((packed)) InterruptGateDescriptor;

typedef struct{
	unsigned int baseLo:16;
	unsigned int segmentSelector:16;
	unsigned int unused1:8;
	GateDiscriptorType type:3;
	GateDescriptorBits gateSize:1;
	unsigned int unused2:1;
	PlivilegeLevel descriptorPlivilegeLevel:2;
	unsigned int segmentPresentFlag:1;
	unsigned int baseHi:16;
} __attribute__ ((packed)) TrapGateDescriptor;

/*
#define	DEF_IDT_FLAGS_INTGATE_16BIT		0x06
#define	DEF_IDT_FLAGS_TSKGATE			0x05
#define	DEF_IDT_FLAGS_CALL_GATE			0x0C
#define	DEF_IDT_FLAGS_INTGATE_32BIT		0x0E
#define	DEF_IDT_FLAGS_TRPGATE			0x0F
#define	DEF_IDT_FLAGS_DPL_LV0			0x00
#define	DEF_IDT_FLAGS_DPL_LV1			0x20
#define	DEF_IDT_FLAGS_DPL_LV2			0x40
#define	DEF_IDT_FLAGS_DPL_LV3			0x60
#define	DEF_IDT_FLAGS_PRESENT			0x80

#define	DEF_IDT_INT_SELECTOR			0x08
*/
typedef union IdtDescriptor{
	TaskGateDescriptor tagd;
	InterruptGateDescriptor igd;
	TrapGateDescriptor trgd;
} IdtDescriptor;

#define IDT_COUNT 256
typedef IdtDescriptor IdtDescriptorTable[IDT_COUNT];

typedef struct{
	unsigned short size;
	IdtDescriptor *base;
} __attribute__ ((packed)) InterruptDescriptorTableRegister;

int initIdt(void);
#define	load_idt( ) ({ __asm__ __volatile__ ( "lidt idtr" ); })






/*

ƒCƒxƒ“ƒgŠÖŒW
*/

typedef enum{
	EVENT_TYPE_NONE,
	EVENT_TYPE_KEYBOARD,
	EVENT_TYPE_MOUSE,
	EVENT_TYPE_MOUSE_UP,
	EVENT_TYPE_MOUSE_DOWN,
	EVENT_TYPE_MOUSE_MOVE,
	EVENT_TYPE_KEYUP,
	EVENT_TYPE_KEYDOWN,
	EVENT_TYPE_TIMER,
} EventType;


typedef struct{
	EventType type;
	//EventData* data;
	int data;
} EventArg;



#define EVENT_BUF_COUNT 128
typedef struct{
	int nextRead, nextWrite;
	EventArg item[EVENT_BUF_COUNT];
} EventBuf;
void _asm_keyboard_event(void);
void _asm_mouse_event(void);
void _asm_timer_event(void);

int readEventBuf(EventArg*);

#endif