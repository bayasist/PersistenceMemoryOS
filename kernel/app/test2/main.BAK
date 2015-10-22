#include "../api.h"
#include <stdio.h>

void BayaMain(void){
	Point location;
	Size size;
	HANDLE handle;
	long long time = 0;
	char aaaa[10] = "%d";
	char bbbb[100] = "";
	
	
	location.x = 0;
	location.y = 150;
	size.width = 200;
	size.height = 50;
	api_createWindow(location, size, &handle);
	
	while(1){
		Point location;
		location.x = location.y = 0;
		sprintf(bbbb, aaaa, time);
		api_clear(handle);
		api_print(handle, bbbb, location);
		api_draw(handle);
		
		if(time > 100000000){
			time = 0;
		}else{
			time++;
		}
	}
	
}


