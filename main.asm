[BITS 16]
	ORG 0x7c00
	TIMES 0x7e00 - ($ - $$) DB 0x00
%include "boot.asm"
%include "gdtData.asm"
	