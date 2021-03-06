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
	addiu	$2,$2,2 	reg 2 = reg 2 + 2
	bne	$2,$5,$L2		if reg 2 != reg 5 (=32) go to L2
	addiu	$3,$3,4		

	lui	$6,%hi(B)						#load high part of B into reg 6
	lui	$2,%hi(B+4)					#load high part of B+4
	lui	$5,%hi(B+64)				#load high part of B+64
	sw	$0,%lo(B)($6)				#store 0 in B (B high + offset B low)
	addiu	$2,$2,%lo(B+4)		#complete the load of B+4 in reg 2
	addiu	$5,$5,%lo(B+64)		#complete the load of B+64 into reg 5
$L3:
	lw	$3,0($4)					#load the value at $4 (A)
	addiu	$2,$2,4					#$2 += 4 (volgende element van B)	
	addiu	$3,$3,95				#waarde van A element 0 + 95
	sw	$3,-4($2)					#schrijf A element naar B[element]
	bne	$2,$5,$L3					#branch not equal: als $2 niet $5 voer volgende uit en ga naar L3 top
	addiu	$4,$4,4					#add 4 to reg 4

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
