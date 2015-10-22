	.file	"asp.s"
	.text
	.globl	_asm_api
	.type	_asm_api, @function
_asm_api:
.LFB0:
	.cfi_startproc
	STI
	push	%es
	push	%ds
	pusha
	movl	%esp,	%eax
	pushl	%eax
	mov	%ss,	%ax
	mov	%ax,	%ds
	mov	%ax,	%es
	pushl	%ebx
	call	CallAPI
	popl	%ebx
	popl	%eax
	popa
	pop	%ds
	pop	%es
	iret
	.cfi_endproc
.LFE0:
	.size	_asm_api, .-_asm_api
	
	.ident	"GCC: (GNU) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
