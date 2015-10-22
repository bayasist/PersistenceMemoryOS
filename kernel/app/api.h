#include "apiData.h"






void api_print(HANDLE windowHandle,char*, Point);
void api_draw(HANDLE windowHandle);
void api_addEvent(WaitEventData* eventData, HANDLE *handle);
HANDLE api_peekEventQueue(GetEventData* eventData);
HANDLE api_waitEventQueue(GetEventData* eventData);
void api_clear(HANDLE windowHandle);
void api_createWindow(Point location, Size size, HANDLE* handle);