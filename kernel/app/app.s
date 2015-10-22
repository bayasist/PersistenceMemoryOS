	.file	"app.s"
	.text
	.globl	_asm_taskswitch
	.type	_asm_taskswitch, @function
_asm_taskswitch:
.LFB0:
	.cfi_startproc
	ljmp	4(%esp)
	ret
	.cfi_endproc
.LFE0:
	.size	_asm_taskswitch, .-_asm_taskswitch
	.globl	_asm_load_tr
	.type	_asm_load_tr, @function
_asm_load_tr:
.LFB1:
	.cfi_startproc
	ltr	4(%esp)
	ret
	.cfi_endproc
.LFE1:
	.size	_asm_load_tr, .-_asm_load_tr
	.globl	_asm_launch_task
	.type	_asm_launch_task, @function
_asm_launch_task:
.LFB2:
	.cfi_startproc
	pusha
	movl	36(%esp),	%eax
	movl	40(%esp),	%ecx
	movl	44(%esp),	%edx
	movl	48(%esp),	%ebx
	movl	52(%esp),	%ebp
	movl	%esp,	(%ebp)
	movw	%ss,	4(%ebp)
	movw	%bx,	%es
	movw	%bx,	%ds
	movw	%bx,	%fs
	movw	%bx,	%gs
	or	$3,	%ecx
	or	$3,	%ebx
	pushl	%ebx
	pushl	%edx
	pushl	%ecx
	pushl	%eax
	lret
	.cfi_endproc
.LFE2:
	.size	_asm_launch_task, .-_asm_launch_task
	.ident	"GCC: (GNU) 4.8.2"
	.section	.note.GNU-stack,"",@progbits
