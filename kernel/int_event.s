	.file	"common.c"
	.text
	.globl	__call_from_app
	.type	__call_from_app, @function
__call_from_app:
.LFB0:
	.cfi_startproc
	push	%es
	push	%ds
	pusha
	movl	%esp,	%eax
	pushl	%eax
	mov	%ss,	%ax
	mov	%ax,	%ds
	mov	%ax,	%es
	call	keyboardEvent
	popl	%eax
	popa
	pop	%ds
	pop	%es
	iret
	.cfi_endproc
.LFE0:
	.size	__call_from_app, .-__call_from_app
	.globl	_asm_keyboard_event
	.type	_asm_keyboard_event, @function
_asm_keyboard_event:
.LFB1:
	.cfi_startproc
	push	%es
	push	%ds
	pusha
	movl	%esp,	%eax
	pushl	%eax
	mov	%ss,	%ax
	mov	%ax,	%ds
	mov	%ax,	%es
	call	keyboardEvent
	popl	%eax
	popa
	pop	%ds
	pop	%es
	iret
	.cfi_endproc
.LFE1:
	.size	_asm_keyboard_event, .-_asm_keyboard_event
	.globl	_asm_mouse_event
	.type	_asm_mouse_event, @function
_asm_mouse_event:
.LFB2:
	.cfi_startproc
	push	%es
	push	%ds
	pusha
	movl	%esp,	%eax
	pushl	%eax
	mov	%ss,	%ax
	mov	%ax,	%ds
	mov	%ax,	%es
	call	mouseEvent
	popl	%eax
	popa
	pop	%ds
	pop	%es
	iret
	.cfi_endproc
.LFE2:
	.size	_asm_mouse_event, .-_asm_mouse_event
	.globl	_asm_timer_event
	.type	_asm_timer_event, @function
_asm_timer_event:
.LFB3:
	.cfi_startproc
	push	%es
	push	%ds
	pusha
	movl	%esp,	%eax
	pushl	%eax
	mov	%ss,	%ax
	mov	%ax,	%ds
	mov	%ax,	%es
	call	timerEvent
	popl	%eax
	popa
	pop	%ds
	pop	%es
	iret
	.cfi_endproc
.LFE3:
	.size	_asm_timer_event, .-_asm_timer_event
	.ident	"GCC: (GNU) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
