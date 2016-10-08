	.file	1 "opdr1.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	fp=32
	.module	nooddspreg
	.section .text.startup,"ax",@progbits
	.align	2
	.globl	main
	.set	nomips16
	.set	nomicromips
	.ent	main
	.type	main, @function
main:
	.frame	$sp,24,$31	# vars= 0, regs= 1/0, args= 16, gp= 0
	.mask	0x80000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	lui	$4,%hi(A)
	addiu	$4,$4,%lo(A)
	move	$3,$4
	move	$2,$0
	li	$5,32		# 0x20
$L2:
	sw	$2,0($3)
	addiu	$2,$2,2
	bne	$2,$5,$L2
	addiu	$3,$3,4

	lui	$6,%hi(B)
	lui	$2,%hi(B+4)
	lui	$5,%hi(B+64)
	sw	$0,%lo(B)($6)
	addiu	$2,$2,%lo(B+4)
	addiu	$5,$5,%lo(B+64)
$L3:
	lw	$3,0($4)
	addiu	$2,$2,4
	addiu	$3,$3,95
	sw	$3,-4($2)
	bne	$2,$5,$L3
	addiu	$4,$4,4

	addiu	$6,$6,%lo(B)
	lw	$4,60($6)
	addiu	$sp,$sp,-24
	sw	$31,20($sp)
	jal	print_roman
	nop

	lw	$31,20($sp)
	move	$2,$0
	j	$31
	addiu	$sp,$sp,24

	.set	macro
	.set	reorder
	.end	main
	.size	main, .-main
	.local	B
	.comm	B,64,4
	.local	A
	.comm	A,64,4
	.ident	"GCC: (GNU) 5.2.0"
