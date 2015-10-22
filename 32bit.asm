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
	CLI				; ���荞�݋֎~
	
	CALL	A20wait
	MOV	AL,0xd1
	OUT	0x64,AL
	CALL	A20wait
	MOV	AL,0xdf			; enable A20
	OUT	0x60,AL
	CALL	A20wait
	MOV	SI,	ENTER_32BITS_MODE_STRING
	CALL	PRINT_MESSAGE
	STI				; ���荞�ݗL��
	RET

A20wait:				; �E�F�C�g�����֐�
	IN	AL, 0x64		; �X�e�[�^�X���W�X�^�ǂݍ���
	TEST	AL, 0x2			; ���M�����R�}���h������������������
					; �V�X�e���t���O�Ń`�F�b�N
	JNZ	A20wait			; �������Ă��Ȃ��ꍇ�̓��[�v
	RET

A20wait2:				; �A�E�g�v�b�g�|�[�g�̒l���������܂��܂ŃE�F�C�g�����֐�
	IN	AL, 0x64		; �X�e�[�^�X���W�X�^�ǂݍ���
	TEST	AL, 0x1			; �l���������܂ꂽ���C���v�b�g�o�b�t�@�t�����`�F�b�N
	JZ	A20wait2		; �������܂�Ă��Ȃ��ꍇ�̓��[�v
	
	
	
	
ENTER_P_MODE:
	CLI
	MOV	EAX,	CR0
	;without paging 
	OR	EAX,	0x00000001
	MOV	CR0, EAX
	JMP	CODE_DESC:P_MODE_START
[BITS 32]
P_MODE_START:					; �v���e�N�e�B�b�h���[�h�J�n
	MOV	AX,	DATA_DESC		; �Z�O�����g�Z���N�^��������
	MOV	SS,	AX
	MOV	ES,	AX
	MOV	FS,	AX
	MOV	GS,	AX
	MOV	DS,	AX

;	MOV	ESP,	900000h
	MOV	ESP,	0x7c00
	
	MOV	EBX,	IMAGE_PMODE_BASE	; �J�[�l���̐擪�A�h���X��EBX�ɓ����
	MOV	EBP,	EBX			; ������EBP�ɓ����

	XOR	EBX,	EBX
	CLI					; ���荞�݋֎~
	CALL	EBP				; �J�[�l���̃G���g�����֐��R�[�����܂�
	ADD	ESP,	4
ERROR_LOOP:
	HLT
	JMP	ERROR_LOOP