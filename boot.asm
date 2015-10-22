[BITS 16]
;	VBEMODE	EQU	0x0118	;1024×760×24bit
	VBEMODE	EQU	0x0105	;1024×760×8bit
	
	
	JMP BOOT_PROGRAM_START
	START_STRING DB "START HELLO OS2" , 0x0d, 0x0a, 0x00
BOOT_PROGRAM_START:
; Initialize Data Segment
	XOR	AX, AX
	MOV	DS, AX
	MOV	ES, AX
	MOV	FS, AX
	MOV	GS, AX

	XOR	BX, BX
	XOR	CX, CX
	XOR	DX, DX

; Initialize Stack Segment and Stack Poiniter
	
	MOV	AX, 0x0000		; スタックポインタを0x0009FFFCに設定する
	MOV	SS, AX
	MOV	SP, 0x7c00
	
	MOV	SI,START_STRING
	CALL	PRINT_MESSAGE
; 画面モードを設定



	MOV	AX, 	0x9000
	MOV	ES,	AX
	MOV	DI,	0
	MOV	AX,	0x4f00
	INT	0x10
	CMP	AX,	0x004f
	JNE	scrn320
	
	MOV	AX,	[ES:DI + 4]
	CMP	AX,	0x0200
	JB	scrn320
	
	MOV	CX, VBEMODE
	MOV	AX,	0x4f01
	INT	0x10
	CMP	AX, 0x004f
	JNE	scrn320
	
	CMP	BYTE[ES:DI + 0x19], 8
	JNE	scrn320
	CMP	BYTE[ES:DI + 0x1b],	4
	JNE	scrn320
	MOV	AX,	[ES:DI + 0x00]
	AND	AX,	0x0080
	JZ	scrn320
	
	MOV	BX,	VBEMODE + 0x4000
	MOV	AX,	0x4f02
	INT	0x10
	MOV	EAX,	[ES:DI + 0x28]
	MOV	[0x9000],	EAX
	MOV	AX,	[ES:DI + 0x12]
	MOV	[0x9004],	AX
	MOV	AX,	[ES:DI + 0x14]
	MOV	[0x9008],	AX
	JMP	scrn320_end
scrn320:
	MOV	AL,	0x13			; VGAグラフィックス、320x200x8bitカラー
	MOV	AH,	0x00
	INT	0x10
	MOV	EAX,	0xa0000
	MOV	[0x9000],	EAX
	MOV	AX,	320
	MOV	[0x9004],	AX
	MOV	AX,	200
	MOV	[0x9008],	AX
scrn320_end:
	
		
	CALL	ENTER_32BITS_MODE
	
	HLT
%include	"print.asm"
%include	"32bit.asm"