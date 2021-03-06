#include "window.h"
#include "common.h"
#include "screen.h"
#include "memory.h"
WindowListCell *windowList;
extern ImageSheet *windowImageSheet;
Window *focusedWindow;
ADD_LIST_FUNCTION(_addWindowList, WindowListCell, Window*)
void addWindowList(Window *window){
	_addWindowList(windowList, window);
}

REMOVE_LIST_FUNCTION(_deleteWindowList, WindowListCell, Window*);
int deleteWindowList(Window *deleteWindow){
	return _deleteWindowList(windowList, deleteWindow);
}

/*
//TODO ↓後でマクロに登録しておく
int deleteWindowList(Window *deleteWindow){
	WindowListCell *p, *select, *selectBefore;
	select = NULL;
	for(p = windowList; p->next != NULL; p = p->next){
		if(p->next->value == deleteWindow){
			select = p->next;
			selectBefore = p;
			break;
		}
	}
	if(select == NULL){
		return 0;
	}
	selectBefore->next = select->next;
	freeMemory(select, sizeof(WindowListCell));
	return 1;
}
*/

void initWindows(ImageSheet *mainImageSheet){
	Size size;
	Point location;
	location.x = location.y = 0;
	size.width = SCREEN_WIDTH;
	size.height = SCREEN_HEIGHT;
	
	windowImageSheet = allocMemoryType(ImageSheet);
	initImageSheet(windowImageSheet, location, size);
	addImageSheet(mainImageSheet, windowImageSheet);
	clearScreen(windowImageSheet, COLOR_TRANSPARENT);
	transferBackBuffer(windowImageSheet);
	focusedWindow = NULL;
	
	windowList = allocMemoryType(WindowListCell);
	windowList->next = NULL;
}
extern int test_aaaaa;
void initWindow(Window **window, Point location, Size size){
	*window = allocMemoryType(Window);
	(*window)->outerSheet = allocMemoryType(ImageSheet);
	(*window)->innerSheet = allocMemoryType(ImageSheet);
	initImageSheet((*window)->outerSheet, location, size);
	location.x = WINDOW_BORDER_SIZE;
	location.y = WINDOW_TITLE_BAR_SIZE;
	size.width -= WINDOW_BORDER_SIZE * 2;
	size.height -= WINDOW_BORDER_SIZE + WINDOW_TITLE_BAR_SIZE;
	initImageSheet((*window)->innerSheet, location, size);
	
	addImageSheet(windowImageSheet, (*window)->outerSheet);
	addImageSheet((*window)->outerSheet, (*window)->innerSheet);
	clearScreen((*window)->outerSheet, COLOR_GRAY);
	clearScreen((*window)->innerSheet, COLOR_WHITE);
	transferBackBuffer((*window)->outerSheet);
	transferBackBuffer((*window)->innerSheet);
	addWindowList((*window));
}


int moveTopWindow(Window *window){
	if(deleteWindowList(window)){
		addWindowList(window);
		return 1;
	}
	return 0;
}
void clearWindow(Window *window, Color color){
	clearScreen(window->innerSheet, color);
}

void getWindowOuterRectangle(Window *window, Point *point, Size *size){
	*point = window->outerSheet->location;
	point->x += windowImageSheet->location.x;
	point->y += windowImageSheet->location.y;
	*size = window->outerSheet->size;
}
void getWindowInnerRectangle(Window *window, Point *point, Size *size){
	*point = window->outerSheet->location;
	point->x += windowImageSheet->location.x;
	point->y += windowImageSheet->location.y;
	point->x += window->innerSheet->location.x;
	point->y += window->innerSheet->location.y;
	*size = window->innerSheet->size;
}


Window* getHitWindowInner(Point point){
	Window *result = getHitWindow(point);
	Point winInnerPoint;
	Size winInnerSize;
	if(result == NULL){
		return NULL;
	}
	getWindowInnerRectangle(result, &winInnerPoint, &winInnerSize);
	if(isHitRectanglePoint(winInnerPoint, winInnerSize, point)){
		return result;
	}
	return NULL;
}


Window* getHitWindow(Point point){
	WindowListCell *p;
	Window *result = NULL;
	for(p = windowList; p->next != NULL; p = p->next){
		Point winPoint;
		Size winSize;
		getWindowOuterRectangle(p->next->value, &winPoint, &winSize);
		if(isHitRectanglePoint(winPoint, winSize, point)){
			result = p->next->value;
		}
	}
	return result;
}


void focusWindow(Window *window){
	if(focusedWindow != NULL){
		clearScreen(focusedWindow->outerSheet, COLOR_GRAY);
	}
	if(window == NULL){
		focusedWindow = NULL;
		return;
	}
	focusedWindow = window;
	if(moveTopImageSheet(windowImageSheet, window->outerSheet)){
		moveTopWindow(window);
	}
	clearScreen(window->outerSheet, COLOR_DARK_GRAY);
}

Window* getFocusedWindow(void){
	return focusedWindow;
}
