	.globl main
main:
.LFB0:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	$1, %ebx
	movl	%ebx, -4(%rbp)
	movl	$6, %edx
	movl	%edx, -8(%rbp)
L2:
	movl	-4(%rbp), %edi
	movl	-8(%rbp), %r8d
	cmpl	%r8d, %edi
	setl	%al
	movzbl	%al, %eax
	movl	%eax, %r9d
	test	%r9d,%r9d
	jne	L0
	jmp	L1
L0:
	movl	-4(%rbp), %r10d
	movl	$1, %r11d
	movl	%r10d, %r12d
	addl	%r11d, %r12d
	movl	%r12d, -4(%rbp)
	jmp L2
L1:
	nop
	movl	-4(%rbp), %r14d
	movl	%r14d, %eax
	popq	%rbp
	ret
.LFE0:
	.section .note.GNU-stack,"",@progbits
