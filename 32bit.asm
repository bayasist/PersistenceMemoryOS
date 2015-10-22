[BITS 16]
%include	"gdt.asm"
%define		CODE_DESC	0x8
%define		DATA_DESC	0x10
%define		IMAGE_PMODE_BASE 0x000100000
ENTER_32BITS_MODE_STRING DB "32Bit Mode", 0x0a , 0x00
ENTER_32BITS_MODE:
	CALL	SETTING_GDT
	CALL	Enable_A20
	JMP	ENTER_P_MODE
	HLT

Enable_A20:
	CLI				; 割り込み禁止
	
	CALL	A20wait
	MOV	AL,0xd1
	OUT	0x64,AL
	CALL	A20wait
	MOV	AL,0xdf			; enable A20
	OUT	0x60,AL
	CALL	A20wait
	MOV	SI,	ENTER_32BITS_MODE_STRING
	CALL	PRINT_MESSAGE
	STI				; 割り込み有効
	RET

A20wait:				; ウェイト処理関数
	IN	AL, 0x64		; ステータスレジスタ読み込み
	TEST	AL, 0x2			; 送信したコマンド処理が完了したかを
					; システムフラグでチェック
	JNZ	A20wait			; 完了していない場合はループ
	RET

A20wait2:				; アウトプットポートの値が書き込まれるまでウェイト処理関数
	IN	AL, 0x64		; ステータスレジスタ読み込み
	TEST	AL, 0x1			; 値が書き込まれたかインプットバッファフルをチェック
	JZ	A20wait2		; 書き込まれていない場合はループ
	
	
	
	
ENTER_P_MODE:
	CLI
	MOV	EAX,	CR0
	;without paging 
	OR	EAX,	0x00000001
	MOV	CR0, EAX
	JMP	CODE_DESC:P_MODE_START
[BITS 32]
P_MODE_START:					; プロテクティッドモード開始
	MOV	AX,	DATA_DESC		; セグメントセレクタを初期化
	MOV	SS,	AX
	MOV	ES,	AX
	MOV	FS,	AX
	MOV	GS,	AX
	MOV	DS,	AX

;	MOV	ESP,	900000h
	MOV	ESP,	0x7c00
	
	MOV	EBX,	IMAGE_PMODE_BASE	; カーネルの先頭アドレスをEBXに入れる
	MOV	EBP,	EBX			; そしてEBPに入れる

	XOR	EBX,	EBX
	CLI					; 割り込み禁止
	CALL	EBP				; カーネルのエントリを関数コールします
	ADD	ESP,	4
ERROR_LOOP:
	HLT
	JMP	ERROR_LOOP