#include "screen.h"

#ifndef IS_DEFINED_MOUSE
#define IS_DEFINED_MOUSE 1
void moveMouse(int, int);
void initMouse(ImageSheet*);
void mouseMain(int);
typedef struct{
	int left:1;
	int center:1;
	int right:1;
} MouseButtons;

typedef enum{
	MOUSE_BUTTON_TYPE_LEFT   = 0x01,
	MOUSE_BUTTON_TYPE_RIGHT  = 0x02,
	MOUSE_BUTTON_TYPE_CENTER = 0x04
} MOUSE_BUTTON_TYPE;
#endif