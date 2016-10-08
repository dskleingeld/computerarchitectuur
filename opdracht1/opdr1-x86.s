	movq    %rdi, -24(%rbp) # Pointer naar begin van een array
	movq    %rsi, -32(%rbp) # Pointer naar begin van een ANDERE array
	movl    %edx, -36(%rbp)
	movl    $0, -8(%rbp)
	movl    $0, -4(%rbp)
	jmp     .L2
.L3:
	movl    -4(%rbp), %eax
	1
	cltq
	salq    $2, %rax
	addq    -24(%rbp), %rax
	movl    (%rax), %edx
	movl    -4(%rbp), %eax
	cltq
	salq    $2, %rax
	addq    -32(%rbp), %rax
	movl    (%rax), %eax
	imull   %edx, %eax
	addl    %eax, -8(%rbp)
	addl    $1, -4(%rbp)
.L2:
	movl    -4(%rbp), %eax
	cmpl    -36(%rbp), %eax
	jl      .L3
	movl    -8(%rbp), %eax
	popq    %rbp
