	.file	"phase4.c"
	.text
	.globl	callLoop
	.type	callLoop, @function
callLoop:
.LFB0:
	.cfi_startproc
	movl	$0, result(%rip)
	ret
	.cfi_endproc
.LFE0:
	.size	callLoop, .-callLoop
	.globl	callSecond
	.type	callSecond, @function
callSecond:
.LFB1:
	.cfi_startproc
	rep ret
	.cfi_endproc
.LFE1:
	.size	callSecond, .-callSecond
	.globl	funct4
	.type	funct4, @function
funct4:
.LFB2:
	.cfi_startproc
	movl	%edi, rbx(%rip)
	testl	%edi, %edi
	jg	.L4
	movl	$0, result(%rip)
.L7:
	movl	result(%rip), %eax
	ret
.L4:
	movl	%esi, rbp(%rip)
	movl	%esi, result(%rip)
	cmpl	$1, %edi
	je	.L7
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	funct4
	movl	rbp(%rip), %esi
	movl	%esi, %eax
	addl	result(%rip), %eax
	movl	%eax, r12d(%rip)
	movl	rbx(%rip), %eax
	leal	-2(%rax), %edi
	call	funct4
	movl	r12d(%rip), %eax
	addl	%eax, result(%rip)
	movl	result(%rip), %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2:
	.size	funct4, .-funct4
	.globl	main
	.type	main, @function
main:
.LFB3:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$2, %esi
	movl	$6, %edi
	call	funct4
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE3:
	.size	main, .-main
	.comm	r12d,4,4
	.comm	rbp,4,4
	.comm	rbx,4,4
	.comm	result,4,4
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-11)"
	.section	.note.GNU-stack,"",@progbits
