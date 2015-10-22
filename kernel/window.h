#include "common.h"
#include "screen.h"

#ifndef IS_DEFINED_WINDOW
#define IS_DEFINED_WINDOW 1
#define WINDOW_BORDER_SIZE 1
#define WINDOW_TITLE_BAR_SIZE 19


typedef struct{
	ImageSheet *outerSheet;
	ImageSheet *innerSheet;
} Window;


LIST_DEFINE(WindowListCell, Window*)
void initWindow(Window **window, Point location, Size size);

void clearWindow(Window *window, Color color);
#define convertWindowHandle(HANDLE) ((Window*)HANDLE) 
void initWindows(ImageSheet *mainImageSheet);
Window* getHitWindow(Point point);
void focusWindow(Window *window);
Window* getHitWindowInner(Point);
Window* getFocusedWindow(void);
#endif