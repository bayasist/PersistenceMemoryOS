[BITS 16]
BOOT2:
	JMP BOOT2_PROGRAM_START
	TEST_STRING DB "HELLO WORLD" , 0x00
%include "print.asm"
BOOT2_PROGRAM_START:
		MOV		SI,TEST_STRING
		
BOOT2_PROGRAM_START_LOOP:
		MOV		AL,[SI]
		ADD		SI,1			; SI��1�𑫂�
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			; �ꕶ���\���t�@���N�V����
		MOV		BX,15			; �J���[�R�[�h
		INT		0x10			; �r�f�IBIOS�Ăяo��
		JMP		BOOT2_PROGRAM_START_LOOP
fin:
		HLT						; ��������܂�CPU���~������
		JMP		fin				; �������[�v
;	MOV SI, TEST_STRING
;	CALL PRINT_MESSAGE
END_LOOP:
	HLT
	JMP END_LOOP