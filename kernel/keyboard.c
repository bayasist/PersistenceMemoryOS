#include "keyboard.h"
#include "common.h"
#include "int.h"



char keyboardState[0x80];
char getKeyChar(KeyCode key){
	char result;
	switch(key){
	case KEY_CODE_A:
		result = 'a';
		break;
	case KEY_CODE_B:
		result = 'b';
		break;
	case KEY_CODE_C:
		result = 'c';
		break;
	case KEY_CODE_D:
		result = 'd';
		break;
	case KEY_CODE_E:
		result = 'e';
		break;
	case KEY_CODE_F:
		result = 'f';
		break;
	case KEY_CODE_G:
		result = 'g';
		break;
	case KEY_CODE_H:
		result = 'h';
		break;
	case KEY_CODE_I:
		result = 'i';
		break;
	case KEY_CODE_J:
		result = 'j';
		break;
	case KEY_CODE_K:
		result = 'k';
		break;
	case KEY_CODE_L:
		result = 'l';
		break;
	case KEY_CODE_M:
		result = 'm';
		break;
	case KEY_CODE_N:
		result = 'n';
		break;
	case KEY_CODE_O:
		result = 'o';
		break;
	case KEY_CODE_P:
		result = 'p';
		break;
	case KEY_CODE_Q:
		result = 'q';
		break;
	case KEY_CODE_R:
		result = 'r';
		break;
	case KEY_CODE_S:
		result = 's';
		break;
	case KEY_CODE_T:
		result = 't';
		break;
	case KEY_CODE_U:
		result = 'u';
		break;
	case KEY_CODE_V:
		result = 'v';
		break;
	case KEY_CODE_W:
		result = 'w';
		break;
	case KEY_CODE_X:
		result = 'x';
		break;
	case KEY_CODE_Y:
		result = 'y';
		break;
	case KEY_CODE_Z:
		result = 'z';
		break;
	case KEY_CODE_0:
	case KEY_CODE_0_TENKEY:
		result = '0';
		break;
	case KEY_CODE_1:
	case KEY_CODE_1_TENKEY:
		result = '1';
		break;
	case KEY_CODE_2:
	case KEY_CODE_2_TENKEY:
		result = '2';
		break;
	case KEY_CODE_3:
	case KEY_CODE_3_TENKEY:
		result = '3';
		break;
	case KEY_CODE_4:
	case KEY_CODE_4_TENKEY:
		result = '4';
		break;
	case KEY_CODE_5:
	case KEY_CODE_5_TENKEY:
		result = '5';
		break;
	case KEY_CODE_6:
	case KEY_CODE_6_TENKEY:
		result = '6';
		break;
	case KEY_CODE_7:
	case KEY_CODE_7_TENKEY:
		result = '7';
		break;
	case KEY_CODE_8:
	case KEY_CODE_8_TENKEY:
		result = '8';
		break;
	case KEY_CODE_9:
	case KEY_CODE_9_TENKEY:
		result = '9';
		break;
	case KEY_CODE_HYPHEN:
	case KEY_CODE_HYPHEN_TENKEY:
		result = '-';
		break;
	case KEY_CODE_CIRCUMFLEX:
		result = '^';
		break;
	case KEY_CODE_TAB:
		result = '\t';
		break;
	case KEY_CODE_ATMARK:
		result = '@';
		break;
	case KEY_CODE_BRANCKET_START:
		result = '[';
		break;
	case KEY_CODE_BRANCKET_END:
		result = ']';
		break;
	case KEY_CODE_ENTER:
		result = '\n';
		break;
	case KEY_CODE_SEMICOLON:
		result = ';';
		break;
	case KEY_CODE_COLON:
		result = ':';
		break;
	case KEY_CODE_COMMA:
		result = ',';
		break;
	case KEY_CODE_DOT:
	case KEY_CODE_DOT_TENKEY:
		result = '.';
		break;
	case KEY_CODE_SLASH:
		result = '/';
		break;
	case KEY_CODE_ASTERISK_TENKEY:
		result = '*';
		break;
	case KEY_CODE_SPACE:
		result = ' ';
		break;
	case KEY_CODE_PLAS_TENKEY:
		result = '+';
		break;
	case KEY_CODE_UNDER_SCORE:
		result = '_';
		break;
	case KEY_CODE_BACK_SLASH:
		result = '\\';
		break;
	default:
		result = '\0';
		break;
	}
	
	return result;
}

void keyup(KeyCode key){
	keyboardState[key] = 0;
	if(getFocusedWindow() != NULL){
		keyupEventTask(getFocusedWindow(), key, getKeyChar(key));
	}
}

void keydown(KeyCode key){
	keyboardState[key] = 1;
	if(getFocusedWindow() != NULL){
		keydownEventTask(getFocusedWindow(), key, getKeyChar(key));
	}
	
	
}


void keyboardMain(int data){
	if((data & 0x80) == 0){
		keydown(data & 0x7f);
	} else {
		keyup(data & 0x7f);
	}
}



