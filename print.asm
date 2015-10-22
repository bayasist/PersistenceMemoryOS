[BITS 16]
;
;文字列表示
;事前にSIレジスタに表示したい文字列のアドレスを代入しておく
;%ifndef __Print_INC_INCLUDED__
;%define __Print_INC_INCLUDED__

PRINT_MESSAGE:
	PUSH	AX
	PUSH	BX
StartDispMsg:
	LODSB
	OR	AL,	AL
	JZ	.DONE	
	MOV	AH,	0x0E
	MOV	BH,	0x00
	MOV	BL,	0x07
	INT	0x10
	JMP	StartDispMsg
.DONE:
	POP	BX
	POP	AX
	RET

;%endif

