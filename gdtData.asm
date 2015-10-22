[BITS 16]

gdt_toc:			; GDTとサイズとアドレスを格納しているアドレスラベル
		DW 8*4		; GDTのサイズ
		DD _gdt		; GDTのアドレス
_gdt:					; このラベルをgdt_tocに登録する
		; Null descriptor
		DW 0x0000		; 全部値が0のNullディスクリプタを作ります
		DW 0x0000		; Intel　CPUの仕様で決まっています
		DW 0x0000		
		DW 0x0000		
		
		; Code descriptor				
		DB 0xFF			; コードセグメントのサイズ（Segment Limit）を入れます
		DB 0xFF			; サイズは0xFFFFFを入れます
		DW 0x0000		; コードセグメントのベースアドレスを入れます。0x00000000を入れます
		DB 0x00			; Base Address Mid
		DB 10011010b		; Type、S、DPL、Pの値入れます
		DB 11001111b		; Segment Limit Hi、AVL、0、D/B、Gの値を入れます
		DB 0			; Base Address Hi

		; Data descriptor
		DB 0xFF			; データセグメントのサイズ（Segment Limit）を入れます
		DB 0xFF			; サイズは0xFFFFFを入れます
		DW 0x0000		; データセグメントのベースアドレスを入れます。0x00000000を入れます
		DB 0x00			; Base Address Mid
		DB 10010010b		; Type、S、DPL、Pの値入れます
		DB 11001111b		; Segment Limit Hi、AVL、0、D/B、Gの値を入れます
		DB 0			; Base Address Hi	
		
		; TEMPORARY
		DW 0x0000	
		DW 0x0000		
		DW 0x0000		
		DW 0x0000		
