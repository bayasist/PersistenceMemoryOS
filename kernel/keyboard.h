
#ifndef IS_DEFINED_KEYBOARD
#define IS_DEFINED_KEYBOARD 1

typedef enum{
	KEY_CODE_ESC			 = 0x1,
	KEY_CODE_1				 = 0x2,
	KEY_CODE_2				 = 0x3,
	KEY_CODE_3				 = 0x4,
	KEY_CODE_4				 = 0x5,
	KEY_CODE_5				 = 0x6,
	KEY_CODE_6				 = 0x7,
	KEY_CODE_7				 = 0x8,
	KEY_CODE_8				 = 0x9,
	KEY_CODE_9				 = 0xA,
	KEY_CODE_0				 = 0xB,
	KEY_CODE_HYPHEN			 = 0xC,
	KEY_CODE_CIRCUMFLEX		 = 0xD,
	KEY_CODE_BAK			 = 0xE,
	KEY_CODE_TAB			 = 0xF,
	KEY_CODE_Q				 = 0x10,
	KEY_CODE_W				 = 0x11,
	KEY_CODE_E				 = 0x12,
	KEY_CODE_R				 = 0x13,
	KEY_CODE_T				 = 0x14,
	KEY_CODE_Y				 = 0x15,
	KEY_CODE_U				 = 0x16,
	KEY_CODE_I				 = 0x17,
	KEY_CODE_O				 = 0x18,
	KEY_CODE_P				 = 0x19,
	KEY_CODE_ATMARK			 = 0x1A,
	KEY_CODE_BRANCKET_START	 = 0x1B,
	KEY_CODE_ENTER			 = 0x1C,
	KEY_CODE_CTRL_LEFT		 = 0x1D,
	KEY_CODE_A				 = 0x1e,
	KEY_CODE_S				 = 0x1f,
	KEY_CODE_D				 = 0x20,
	KEY_CODE_F				 = 0x21,
	KEY_CODE_G				 = 0x22,
	KEY_CODE_H				 = 0x23,
	KEY_CODE_J				 = 0x24,
	KEY_CODE_K				 = 0x25,
	KEY_CODE_L				 = 0x26,
	KEY_CODE_SEMICOLON		 = 0x27,
	KEY_CODE_COLON			 = 0x28,
	KEY_CODE_E_J			 = 0x29,
	KEY_CODE_SHIFT_LEFT	 	 = 0x2A,
	KEY_CODE_BRANCKET_END	 = 0x2B,
	KEY_CODE_Z				 = 0x2C,
	KEY_CODE_X				 = 0x2D,
	KEY_CODE_C				 = 0x2E,
	KEY_CODE_V				 = 0x2F,
	KEY_CODE_B				 = 0x30,
	KEY_CODE_N				 = 0x31,
	KEY_CODE_M				 = 0x32,
	KEY_CODE_COMMA			 = 0x33,
	KEY_CODE_DOT			 = 0x34,
	KEY_CODE_SLASH			 = 0x35,
	KEY_CODE_SHIFT_RIGHT	 = 0x36,
	KEY_CODE_ASTERISK_TENKEY = 0x37,
	KEY_CODE_ALT_LEFT		 = 0x38,
	KEY_CODE_SPACE			 = 0x39,
	KEY_CODE_CAPS_LOCK		 = 0x3A,
	KEY_CODE_F1				 = 0x3B,
	KEY_CODE_F2				 = 0x3C,
	KEY_CODE_F3				 = 0x3D,
	KEY_CODE_F4				 = 0x3E,
	KEY_CODE_F5				 = 0x3F,
	KEY_CODE_F6				 = 0x40,
	KEY_CODE_F7				 = 0x41,
	KEY_CODE_F8				 = 0x42,
	KEY_CODE_F9				 = 0x43,
	KEY_CODE_F10			 = 0x44,
	KEY_CODE_NUM_LOCK		 = 0x45,
	KEY_CODE_SCROLL_LOCK	 = 0x46,
	KEY_CODE_7_TENKEY		 = 0x47,
	KEY_CODE_8_TENKEY		 = 0x48,
	KEY_CODE_9_TENKEY		 = 0x49,
	KEY_CODE_HYPHEN_TENKEY	 = 0x4A,
	KEY_CODE_4_TENKEY		 = 0x4B,
	KEY_CODE_5_TENKEY		 = 0x4C,
	KEY_CODE_6_TENKEY		 = 0x4D,
	KEY_CODE_PLAS_TENKEY	 = 0x4E,
	KEY_CODE_1_TENKEY		 = 0x4F,
	KEY_CODE_2_TENKEY		 = 0x50,
	KEY_CODE_3_TENKEY		 = 0x51,
	KEY_CODE_0_TENKEY		 = 0x52,
	KEY_CODE_DOT_TENKEY		 = 0x53,
	KEY_CODE_SYSREQ_TENKEY	 = 0x54,
	KEY_CODE_F11			 = 0x57,
	KEY_CODE_F12			 = 0x58,
	KEY_CODE_UNDER_SCORE	 = 0x73,
	KEY_CODE_BACK_SLASH		 = 0x7D,
} KeyCode;
void keyboardMain(int);

#endif