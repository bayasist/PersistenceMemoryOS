#include "screen.h"
#include "memory.h"
#include "common.h"
#include "app/app.h"
#include "window.h"
#include "mouse.h"
Font *pFont;

#ifdef	IS_PALET_MODE
ColorValue getColorValue(Color color){
	return (ColorValue)color;
}
#endif
#ifndef	IS_PALET_MODE
ColorValue getColorValue(Color color){
	ColorValue value;
	switch(color){
	case COLOR_BLACK:
		value.r = value.g = value.b = 0;
		break;
	case COLOR_WHITE:
		value.r = value.g = value.b = 255;
		break;
	}
	return value;
}
#endif


char *VmBufferAddress;



ImageSheet *mainImageSheet;
ImageSheet *windowImageSheet;
ImageSheet *mouseImageSheet;
extern Task tasks[100];
extern Task *nowTask;
int aaaaa;

void setPalette(unsigned char index, unsigned char r, unsigned char g, unsigned char b){
	outPortByte(0x03c8, index);
	outPortByte(0x03c9, r / 4);
	outPortByte(0x03c9, g / 4);
	outPortByte(0x03c9, b / 4);
}
	

void initScreen(void){
	unsigned char tempList[] = {0, 64, 128, 192, 255};
	int r,g,b;
	for(r = 0; r < 5; r++){
		for(g = 0; g < 5; g++){
			for(b = 0; b < 5; b++){
				setPalette(r * 25 + g * 5 + b, tempList[r], tempList[g], tempList[b]);
			}
		}
	}
	aaaaa = 0;
	
	VmBufferAddress = allocMemoryTypeSize(char, SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_COLOR_BYTE);
	Point location;
	Size size;
	size.width = SCREEN_WIDTH;
	size.height = SCREEN_HEIGHT;
	location.x = location.y = 0;
	mainImageSheet = allocMemoryType(ImageSheet);
	initImageSheet(mainImageSheet, location, size);
	clearScreen(mainImageSheet, COLOR_WHITE);
	

	initWindows(mainImageSheet);
	initMouse(mainImageSheet);
}


int isTransparent(ColorValue value){
	Point po;
	po.x = po.y = 0;
	if(value == getColorValue(COLOR_TRANSPARENT)){
		return 1;
	}
	return 0;
}

void drawImageSheet(ImageSheet *sheet, Point parentPoint, Size parentSize, Point drawRangePoint, Size drawRangeSize){
	int x, y;
	ImageSheetListCell *list;
	Point thisLocation; //今から描画するシートの場所
	Size thisSize = sheet->size; //今から描画するシートのサイズ
	Point drawChildRangePoint;
	Size drawChildRangeSize;
	
	
	thisLocation.x = parentPoint.x + sheet->location.x;
	thisLocation.y = parentPoint.y + sheet->location.y;
	if(thisLocation.x + thisSize.width > parentPoint.x + parentSize.width){
		thisSize.width = parentPoint.x + parentSize.width - thisLocation.x;
	}
	if(thisLocation.y + thisSize.height > parentPoint.y + parentSize.height){
		thisSize.height = parentPoint.y + parentSize.height - thisLocation.y;
	}
	
	drawChildRangePoint = thisLocation;
	drawChildRangeSize = thisSize;
	
	if(thisLocation.x <= drawRangePoint.x){
		drawChildRangeSize.width -= drawRangePoint.x - thisLocation.x;
		drawChildRangePoint.x = drawRangePoint.x;
	}
	
	if(thisLocation.y <= drawRangePoint.y){
		drawChildRangeSize.height -= drawRangePoint.y - thisLocation.y;
		drawChildRangePoint.y = drawRangePoint.y;
	}
	
	
	if(thisLocation.x + thisSize.width > drawRangePoint.x + drawRangeSize.width){
		drawChildRangeSize.width -= (thisLocation.x + thisSize.width) - (drawRangePoint.x + drawRangeSize.width);
	}
	if(thisLocation.y + thisSize.height > drawRangePoint.y + drawRangeSize.height){
		drawChildRangeSize.height -= (thisLocation.y + thisSize.height) - (drawRangePoint.y + drawRangeSize.height);
	}
	
	
	
	for(y = 0; y < sheet->size.height; y++){
		int yPoint = y + parentPoint.y + sheet->location.y;
		/*
		if(yPoint < parentPoint.y || parentPoint.y + parentSize.height <= yPoint){
			continue;
		}
		*/
		if(yPoint < drawRangePoint.y || drawRangePoint.y + drawRangeSize.height <= yPoint){
			continue;
		}
		for(x = 0; x < sheet->size.width; x++){
			int xPoint = x + parentPoint.x + sheet->location.x;
			/*
			if(xPoint < parentPoint.x || parentPoint.x + parentSize.width <= xPoint){
				continue;
			}
			*/
			if(xPoint < drawRangePoint.x || drawRangePoint.x + drawRangeSize.width <= xPoint){
				continue;
			}
			
			//透明色なら無視
			if(isTransparent(*((ColorValue*)((char*)sheet->bitmap + SCREEN_COLOR_BYTE * (x + y * sheet->size.width))))){
				continue;
			}
			
			*((ColorValue*)((char*)VmBufferAddress + SCREEN_COLOR_BYTE * (xPoint + yPoint * SCREEN_WIDTH)))
				= *((ColorValue*)((char*)sheet->bitmap + SCREEN_COLOR_BYTE * (x + y * sheet->size.width)));
		}
	}
	
	for(list = sheet->sheetList; list->next != NULL; list = list->next){
		
		drawImageSheet(list->next->value, thisLocation, thisSize, drawChildRangePoint, drawChildRangeSize);
	}
}
void draw(void){
	unsigned long long i;
	Point location;
	Size size;
	size.width = SCREEN_WIDTH;
	size.height = SCREEN_HEIGHT;
	location.x = location.y = 0;
	
	
	
	//while(aaaaa > 0);
	aaaaa++;
	
	drawImageSheet(mainImageSheet, location, size, location, size);
	aaaaa--;
	
	
	for(i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * SCREEN_COLOR_BYTE; i++){
		*(VM_START + i) = *((char*)VmBufferAddress + i);
	}
}

int drawScreenPoint(ImageSheet *sheet, Point point, Color color){
	if(point.x < 0 || sheet->size.width <= point.x){
		return -1;
	}
	if(point.y < 0 || sheet->size.height <= point.y){
		return -1;
	}
	*((ColorValue*)((char*)sheet->backBitmap + SCREEN_COLOR_BYTE * (point.x + point.y * sheet->size.width))) = getColorValue(color);
	return 0;
}

int clearScreen(ImageSheet *sheet, Color color){
	Point point;
	for(point.x = 0; point.x < sheet->size.width; point.x++){
		for(point.y = 0; point.y < sheet->size.height; point.y++){
			if(drawScreenPoint(sheet, point, color) < 0)
				return -1;
		}
	}
	return 0;
}

void transferBackBuffer(ImageSheet *sheet){
	char* tmp;
	tmp = sheet->bitmap;
	sheet->bitmap = sheet->backBitmap;
	sheet->backBitmap = tmp;
}
int drawChar(ImageSheet *sheet, char value, Point location, Color color){
	Point point;
	int countInByte = 0, byteIndex = 0;
	
	pFont = (void*)DEFAULT_FONT_ADDRESS;
	CharFont *charFont;
	charFont = &((*pFont)[(int)value]);
	
	for(point.y = location.y; point.y < DEFAULT_FONT_HEIGHT + location.y; point.y++){
		for(point.x = location.x; point.x < DEFAULT_FONT_WIDTH + location.x; point.x++){
			int mask = 1;
			mask = mask << countInByte;
			if(((*charFont)[byteIndex] & mask) != 0){
				if(drawScreenPoint(sheet, point, color) < 0)
					return -1;
			}
			countInByte++;
			if(countInByte >= 8){
				countInByte = 0;
				byteIndex ++;
			}
		}
	}
	return 0;
}


int drawString(ImageSheet *sheet, char value[], Point location, Color color){
	int i;
	Point defaultPoint;
	defaultPoint.x = location.x;
	defaultPoint.y = location.y;
	for(i = 0; value[i] != '\0'; i++){
		if(value[i] == 0x0d){
			location.x = defaultPoint.x;
			continue;
		}
		if(value[i] == 0x0a){
			location.y += DEFAULT_FONT_HEIGHT;
			continue;
		}
		drawChar(sheet, value[i], location, color);
		location.x += DEFAULT_FONT_WIDTH;
	}
	return 0;
}
void stringRightShift(char *value, int count){
	int i;
	for(i = count - 2; i >= 0; i--){
		value[i + 1] = value[i];
	}
	
	value[0] = ' ';
}


void putInt2(int a, char aa[]){
	while(1){
		int digit = a % 10;
		aa[0] = digit + '0';
		a /= 10;
		if(a == 0){
			break;
		}else{
			stringRightShift(aa, 100);
		}
	}
	
	Point point;
	point.x = point.y = 0;
}
void putInt(ImageSheet *sheet, int a, Point location, Color color){
	char aa[100];
	int i = 0;
	for(i = 0; i < 100; i++){
		aa[i] = '\0';
	}
	if(a < 0){
		aa[0] = '-';
		putInt2(-a, aa + 1);
		drawString(sheet, aa, location, color);
		return;
	}else{
		putInt2(a, aa);
		drawString(sheet, aa, location, color);
		return;
	}
}

ADD_LIST_FUNCTION(_addImageSheet, ImageSheetListCell, ImageSheet*);
void addImageSheet(ImageSheet *origin, ImageSheet *addingSheet){
	//while(aaaaa > 0);
	aaaaa++;
	_addImageSheet(origin->sheetList, addingSheet);
	aaaaa--;
}


REMOVE_LIST_FUNCTION(_deleteImageSheet, ImageSheetListCell, ImageSheet*);
int deleteImageSheet(ImageSheet *origin, ImageSheet *deleteSheet){
	int result;
	//while(aaaaa > 0);
	aaaaa++;
	result = _deleteImageSheet(origin->sheetList, deleteSheet);
	aaaaa--;
	return result;
}

int moveTopImageSheet(ImageSheet *origin, ImageSheet *imageSheet){
	if(deleteImageSheet(origin, imageSheet)){
		addImageSheet(origin, imageSheet);
		return 1;
	}
	return 0;
}

void initImageSheet(ImageSheet *sheet, Point location, Size size){
	sheet->location = location;
	sheet->size = size;
	
	sheet->bitmap = allocMemoryTypeSize(char, size.width * size.height * SCREEN_COLOR_BYTE);
	sheet->backBitmap = allocMemoryTypeSize(char, size.width * size.height * SCREEN_COLOR_BYTE);
	//ダミーセルの作成
	sheet->sheetList = allocMemoryType(ImageSheetListCell);
	sheet->sheetList->next = NULL;
}
