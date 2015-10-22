#include "screen.h"
#include "idt.h"
#include "main.h"
#include "memory.h"
#include "gdt.h"
#include "app/app.h"
#include "int.h"

extern SegmentDescriptor gdt[GDT_COUNT];
extern GdtToc gdtToc;
extern ImageSheet *mainImageSheet;
extern ImageSheet *windowImageSheet;
extern int initedApp;
int endInitScreen;
int main(void){
	//int count = 0;
	Point p;
	cli();
	p.x = p.y = 0;
	endInitScreen = 0;
	initedApp = 0;
	initGdt();
	initMemory();
	initScreen();
	initIdt();
	initInt();
	initApp();
	endInitScreen = 1;
	sti();
	eventMainLoop();
	return 0;
	
}


