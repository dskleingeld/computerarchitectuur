	movq    %rdi, -24(%rbp) # Pointer naar begin van een array (movq: move quad)
	movq    %rsi, -32(%rbp) # Pointer naar begin van een ANDERE array
	movl    %edx, -36(%rbp)	#	copy 4 bytes from edx to bytes -36 to -32 in the stack
	movl    $0, -8(%rbp)		# set bytes -8 to -4 to 0
	movl    $0, -4(%rbp)		#	set bytes -4 to 0  to o
	jmp     .L2		#continue at L2
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
	movl    -4(%rbp), %eax	#set the register %eax to stack byes -4 to 0
	cmpl    -36(%rbp), %eax	#
	jl      .L3
	movl    -8(%rbp), %eax
	popq    %rbp
	
move sourse destination (left to right)
	
assembly instructions are generally suffixed with the letters "b", "s", "w", "l", "q" or "t" to determine what size operand is being manipulated.

    b = byte (8 bit)
    s = short (16 bit integer) or single (32-bit floating point)
    w = word (16 bit) (two bytes here ipv 4 in mips)
    l = long (32 bit integer or 64-bit floating point)
    q = quad (64 bit) (aka 8 bytes)
    t = ten bytes (80-bit floating point)

