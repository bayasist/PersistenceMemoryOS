[BITS 16]

gdt_toc:			; GDT�ƃT�C�Y�ƃA�h���X���i�[���Ă���A�h���X���x��
		DW 8*4		; GDT�̃T�C�Y
		DD _gdt		; GDT�̃A�h���X
_gdt:					; ���̃��x����gdt_toc�ɓo�^����
		; Null descriptor
		DW 0x0000		; �S���l��0��Null�f�B�X�N���v�^�����܂�
		DW 0x0000		; Intel�@CPU�̎d�l�Ō��܂��Ă��܂�
		DW 0x0000		
		DW 0x0000		
		
		; Code descriptor				
		DB 0xFF			; �R�[�h�Z�O�����g�̃T�C�Y�iSegment Limit�j�����܂�
		DB 0xFF			; �T�C�Y��0xFFFFF�����܂�
		DW 0x0000		; �R�[�h�Z�O�����g�̃x�[�X�A�h���X�����܂��B0x00000000�����܂�
		DB 0x00			; Base Address Mid
		DB 10011010b		; Type�AS�ADPL�AP�̒l����܂�
		DB 11001111b		; Segment Limit Hi�AAVL�A0�AD/B�AG�̒l�����܂�
		DB 0			; Base Address Hi

		; Data descriptor
		DB 0xFF			; �f�[�^�Z�O�����g�̃T�C�Y�iSegment Limit�j�����܂�
		DB 0xFF			; �T�C�Y��0xFFFFF�����܂�
		DW 0x0000		; �f�[�^�Z�O�����g�̃x�[�X�A�h���X�����܂��B0x00000000�����܂�
		DB 0x00			; Base Address Mid
		DB 10010010b		; Type�AS�ADPL�AP�̒l����܂�
		DB 11001111b		; Segment Limit Hi�AAVL�A0�AD/B�AG�̒l�����܂�
		DB 0			; Base Address Hi	
		
		; TEMPORARY
		DW 0x0000	
		DW 0x0000		
		DW 0x0000		
		DW 0x0000		
