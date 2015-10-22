#include "common.h"
void outPortByte( unsigned short address, unsigned char value )
{
//	__asm__ __volatile__( "out %%dx, %%al" : : "d"(address), "a"(value) );
//	__asm__ __volatile__( "out %%al, %%dx" : : "a"(value), "d"(address) );
	__asm__ __volatile__( "out %0, %1" : : "r"(value), "r"(address) );
}
unsigned char inPortByte( unsigned short address )
{
	unsigned char data;
	
	__asm__ __volatile__( "mov %%ax, %%dx" : : "a"(address) :"dx");
	__asm__ __volatile__( "in %%dx, %%al" : : :"ax" );
	__asm__ __volatile__( "mov %%al, %%bl" : "=b"(data) : :"ax" );
	return( data );
}


int isHitRectanglePoint(Point rectPoint, Size rectSize, Point point){
	if(rectPoint.x <= point.x && point.x < rectPoint.x + rectSize.width){
		if(rectPoint.y <= point.y && point.y < rectPoint.y + rectSize.height){
			return 1;
		}
	}
	return 0;
}


int cliCount;
void initCommon(void){
	cliCount = 0;
}

void cli(void){
	_cli();
	cliCount++;
}
void sti(void){
	cliCount--;
	if(cliCount <= 0){
		_sti();
	}
}