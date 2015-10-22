#include "common.h"
#ifndef IS_DEFINED_SCREEN
#define IS_DEFINED_SCREEN 1
//#define SCREEN_WIDTH	320
//#define SCREEN_HEIGHT	200
#define SCREEN_WIDTH (*((short*)(0x9004)))
#define SCREEN_HEIGHT (*((short*)(0x9008)))
#define VM_START (*((char**)(0x9000)))
#define VRAM_START_MEMORY_ADDRESS 0x9000
#define SCREEN_COLOR_BYTE	1
#define IS_PALET_MODE		1

#define DEFAULT_FONT_WIDTH 8
#define DEFAULT_FONT_HEIGHT 16
#define DEFAULT_FONT_ADDRESS 0x200000
#define DEFAULT_FONT_COUNT 256


typedef enum{
	COLOR_BLACK			= 0,
	COLOR_BLUE			= 4,
	COLOR_GREEN			= 20,
	COLOR_GRAY			= 62,
	COLOR_DARK_GRAY 	= 31,
	COLOR_LIGHT_GRAY	= 93,
	COLOR_RED			= 100,
	COLOR_WHITE			= 124,
	COLOR_TRANSPARENT 	= 255
} Color;

#ifdef	IS_PALET_MODE
typedef unsigned char ColorValue;
#endif
#ifndef	IS_PALET_MODE
typedef struct{
	unsigned char r, g, b;
}ColorValue;
#endif

typedef char CharFont[DEFAULT_FONT_WIDTH * DEFAULT_FONT_HEIGHT / 8];
typedef CharFont Font[DEFAULT_FONT_COUNT];

ColorValue getColorValue(Color color);

struct ImageSheetListCell;
typedef struct{
	Point location;
	Size size;
	void *bitmap;
	void *backBitmap;
	struct ImageSheetListCell *sheetList;
} ImageSheet;

/*
typedef struct ImageSheetListCell{
	struct ImageSheetListCell *next;
	ImageSheet *sheet;
} ImageSheetListCell;
*/

LIST_DEFINE(ImageSheetListCell, ImageSheet*);


int writeScreenPoint(Point point, Color color);

int clearScreen(ImageSheet*,Color color);
int drawChar(ImageSheet*,char value, Point location, Color color);
int drawString(ImageSheet*,char value[], Point location, Color color);
void putInt(ImageSheet*,int a, Point location, Color color);
void initScreen(void);
void draw(void);
void initImageSheet(ImageSheet *sheet, Point location, Size size);
void addImageSheet(ImageSheet *origin, ImageSheet *addingSheet);

int moveTopImageSheet(ImageSheet*, ImageSheet*);

void transferBackBuffer(ImageSheet *sheet);
#endif