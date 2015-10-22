[BITS 16]
; GDTを設定します
SETTING_GDT:
		CLI				; 割り込みを禁止します
		PUSHA				; AX、CX、DX、BX、SP、BP、SI、DIレジスタをPUSHします
		LGDT	[gdt_toc]
		STI				; 割り込みを有効にします
		POPA				; DI、SI、BP、SP、BX、DX、CX、AXにPOPします
		RET
		