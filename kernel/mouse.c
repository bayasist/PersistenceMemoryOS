#include "mouse.h"
#include "screen.h"
#include "common.h"
#include "memory.h"
#include "window.h"
#include "int.h"
int mouseEventCount;
int mouseBuf[4];
Point mousePoint;
MouseButtons mouseButtons;

extern ImageSheet *mouseImageSheet;
void initMouse(ImageSheet *mainImageSheet){
	Size size;
	mousePoint.x = mousePoint.y = 10;
	size.width = 5;
	size.height = 5;
	
	mouseImageSheet = allocMemoryType(ImageSheet);
	initImageSheet(mouseImageSheet, mousePoint, size);
	addImageSheet(mainImageSheet, mouseImageSheet);
	clearScreen(mouseImageSheet, COLOR_RED);
	
	mouseEventCount = 0;
}

extern ImageSheet *mainImageSheet;
extern ImageSheet *windowImageSheet;
Point moveStartWindowLocation;
Window *movingWindow;
Point moveStartMouseLocation;
void moveMouse(int moveX, int moveY){
	Point po;
	po.x = po.y = 0;
	mousePoint.x += moveX;
	mousePoint.y += moveY;
	if(mousePoint.x < 0){
		mousePoint.x = 0;
	}
	if(mousePoint.x >= SCREEN_WIDTH){
		mousePoint.x = SCREEN_WIDTH - 1;
	}
	if(mousePoint.y < 0){
		mousePoint.y = 0;
	}
	if(mousePoint.y >= SCREEN_HEIGHT){
		mousePoint.y = SCREEN_HEIGHT - 1;
	}
	mouseImageSheet->location = mousePoint;
	
	
	if(getFocusedWindow() != NULL){
		mouseMoveEventTask(getFocusedWindow(), mousePoint, mouseButtons);
	}
	
	if(movingWindow != NULL){
		Point newWindowLocation;
		newWindowLocation.x = moveStartWindowLocation.x + (mousePoint.x - moveStartMouseLocation.x);
		newWindowLocation.y = moveStartWindowLocation.y + (mousePoint.y - moveStartMouseLocation.y);
		movingWindow->outerSheet->location = newWindowLocation;
	}
}


void mouseUp(MOUSE_BUTTON_TYPE type){
	if(type == MOUSE_BUTTON_TYPE_LEFT){
		clearScreen(mouseImageSheet, COLOR_RED);
		transferBackBuffer(mouseImageSheet);
	}
	
	//MouseUpイベントを投げる
	if(getFocusedWindow() != NULL && getHitWindowInner(mousePoint) == getFocusedWindow()){
		mouseUpEventTask(getFocusedWindow(), mousePoint, mouseButtons);
	}
	
	if(movingWindow != NULL){
		Point newWindowLocation;
		newWindowLocation.x = moveStartWindowLocation.x + (mousePoint.x - moveStartMouseLocation.x);
		newWindowLocation.y = moveStartWindowLocation.y + (mousePoint.y - moveStartMouseLocation.y);
		movingWindow->outerSheet->location = newWindowLocation;
		movingWindow = NULL;
	}
}




void mouseDown(MOUSE_BUTTON_TYPE type){
	Window *selectingWindow;
	if(type == MOUSE_BUTTON_TYPE_LEFT){
		clearScreen(mouseImageSheet, COLOR_BLUE);
		transferBackBuffer(mouseImageSheet);
	}
	
	//ウィンドウの上をクリックしていたら、ウィンドウにフォーカスを当てる。
	if((selectingWindow = getHitWindow(mousePoint)) != NULL){
		focusWindow(selectingWindow);
	}else{
		focusWindow(NULL);
	}
	
	//MouseUpイベントを投げる
	if(getFocusedWindow() != NULL && getHitWindowInner(mousePoint) == getFocusedWindow()){
		mouseDownEventTask(getFocusedWindow(), mousePoint, mouseButtons);
	}
	
	//Windowを動かす
	if(getFocusedWindow() != NULL && getHitWindow(mousePoint) == getFocusedWindow() && getHitWindowInner(mousePoint) != getFocusedWindow()){
		movingWindow = getFocusedWindow();
		moveStartWindowLocation = movingWindow->outerSheet->location;
		moveStartMouseLocation = mousePoint;
	} else {
		movingWindow = NULL;
	}
	
}




void mouseButtonClick(int value){
	MouseButtons newMouseButtons;
	newMouseButtons.left = ((value & MOUSE_BUTTON_TYPE_LEFT) == MOUSE_BUTTON_TYPE_LEFT);
	newMouseButtons.right = ((value & MOUSE_BUTTON_TYPE_RIGHT) == MOUSE_BUTTON_TYPE_RIGHT);
	newMouseButtons.center = ((value & MOUSE_BUTTON_TYPE_CENTER) == MOUSE_BUTTON_TYPE_CENTER);
	
	if(newMouseButtons.left != mouseButtons.left && newMouseButtons.left){
		mouseDown(MOUSE_BUTTON_TYPE_LEFT);
	} else if(newMouseButtons.left != mouseButtons.left && !newMouseButtons.left){
		mouseUp(MOUSE_BUTTON_TYPE_LEFT);
	}
	if(newMouseButtons.right != mouseButtons.right && newMouseButtons.right){
		mouseDown(MOUSE_BUTTON_TYPE_RIGHT);
	} else if(newMouseButtons.right != mouseButtons.right && !newMouseButtons.right){
		mouseUp(MOUSE_BUTTON_TYPE_RIGHT);
	}
	
	if(newMouseButtons.center != mouseButtons.center && newMouseButtons.center){
		mouseDown(MOUSE_BUTTON_TYPE_CENTER);
	} else if(newMouseButtons.center != mouseButtons.center && !newMouseButtons.center){
		mouseUp(MOUSE_BUTTON_TYPE_CENTER);
	}
	
	
	mouseButtons = newMouseButtons;
}

void mouseMain(int data){
	int mouseMoveX, mouseMoveY;
	if(mouseEventCount == 1){
		if((data & 0xc8) != 0x08) {
			return;
		}
	}
	
	
	mouseBuf[mouseEventCount - 1] = data;
	
	if(mouseEventCount == 3){
		mouseMoveX = mouseBuf[1];
		mouseMoveY = mouseBuf[2];
		if((mouseBuf[0] & 0x10) != 0){
			mouseMoveX |= 0xffffff00;
		}
		if((mouseBuf[0] & 0x20) != 0){
			mouseMoveY |= 0xffffff00;
		}
		mouseMoveY *= -1;
		
		mouseButtonClick(mouseBuf[0] & 0x07);
		if(mouseMoveX != 0 || mouseMoveY != 0){
			moveMouse(mouseMoveX, mouseMoveY);
		}
		
	}
	
	
	if(0 <= mouseEventCount && mouseEventCount <= 2){
		mouseEventCount++;
	}else if(mouseEventCount == 3){
		mouseEventCount = 1;
	}
	
}
