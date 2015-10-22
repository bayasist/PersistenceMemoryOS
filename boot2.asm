[BITS 16]
BOOT2:
	JMP BOOT2_PROGRAM_START
	TEST_STRING DB "HELLO WORLD" , 0x00
%include "print.asm"
BOOT2_PROGRAM_START:
		MOV		SI,TEST_STRING
		
BOOT2_PROGRAM_START_LOOP:
		MOV		AL,[SI]
		ADD		SI,1			; SIに1を足す
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; 一文字表示ファンクション
		MOV		BX,15			; カラーコード
		INT		0x10			; ビデオBIOS呼び出し
		JMP		BOOT2_PROGRAM_START_LOOP
fin:
		HLT						; 何かあるまでCPUを停止させる
		JMP		fin				; 無限ループ
;	MOV SI, TEST_STRING
;	CALL PRINT_MESSAGE
END_LOOP:
	HLT
	JMP END_LOOP