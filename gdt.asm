[BITS 16]
; GDT��ݒ肵�܂�
SETTING_GDT:
		CLI				; ���荞�݂��֎~���܂�
		PUSHA				; AX�ACX�ADX�ABX�ASP�ABP�ASI�ADI���W�X�^��PUSH���܂�
		LGDT	[gdt_toc]
		STI				; ���荞�݂�L���ɂ��܂�
		POPA				; DI�ASI�ABP�ASP�ABX�ADX�ACX�AAX��POP���܂�
		RET
		