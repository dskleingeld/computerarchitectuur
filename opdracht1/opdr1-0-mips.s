	.file	1 "opdr1.c"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	fp=32
	.module	nooddspreg
	.local	A
	.comm	A,64,4
	.local	B
	.comm	B,64,4
	.text
	.align	2
	.globl	main
	.set	nomips16
	.set	nomicromips
	.ent	main
	.type	main, @function
main:
	.frame	$fp,32,$31	# vars= 8, regs= 2/0, args= 16, gp= 0
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32 #move stack pointer 32 bits back (reserve 32 bits of memory)
	sw	$31,28($sp)   #store the content of reg 31 into the last 4 bytes of the stack
	sw	$fp,24($sp)   #store the frame pointer in bit 24 to 28 of the stack 
	move	$fp,$sp     #set frame pointer to the current stack pointer
	sw	$0,16($fp)    #set bits 16 to 20 to 0000 
	b	$L2             #execute the code block at $L2
	nop               #do nothing for one cycle #TODO why is there a nop here?

$L3:
	lw	$2,16($fp)  #load bits 16-20 from the stack into reg 2 (set $2 to 0)
	nop             
	sll	$3,$2,1     #shift values from $2 1 to the left and introduce new zero at the rightmost pos.
	                #store this in $3
	lui	$2,%hi(A)   #load the high values of A (= int array) into the higher 16 bits of reg 2 
	lw	$4,16($fp)  #set $4 to bits 16 to 20
	nop             
	sll	$4,$4,2     #shift $4 left and introduce a zero right
	addiu	$2,$2,%lo(A)  #add the lower part of A to $2 (which already contains the upper part of A)
	addu	$2,$4,$2      #$2 = $4 + $2
	sw	$3,0($2)        #store $3 into the stack at $2 #TODO whats the zero for? not +0 right?
	lw	$2,16($fp)      #load into $2 bits 16-20 from the stack
	nop
	addiu	$2,$2,1       #add one to $2
	sw	$2,16($fp)      #store the new $2 stack at bits 16-20
	
$L2:
	lw	$2,16($fp)    #set register 2 to the zeros we put in this part of the frame at line 28
	nop               #do nothing
	slt	$2,$2,16      #set $2 to one if $2 < 16 (waarom dit doen dit $2 is altijd 0000 op dit punt?)
	bne	$2,$0,$L3     #goto L3 if $2 != $0 (=0000 as: "Register $0 always holds 0") 
	nop

	li	$2,95	        # 0x5f  #load 95 into R2
	sw	$2,20($fp)            #store R2 at bits 20-24

  #TODO WAT the hell gebeurt hier? gaan we nu B als adress gebruiken ofzo?
	lui	$2,%hi(B)             
	sw	$0,%lo(B)($2)

	li	$2,1	        # 0x1 #set R2 to 1
	sw	$2,16($fp)
	b	$L4
	nop

$L5:
	lw	$2,16($fp)
	nop
	addiu	$3,$2,-1
	lui	$2,%hi(A)
	sll	$3,$3,2
	addiu	$2,$2,%lo(A)
	addu	$2,$3,$2
	lw	$3,0($2)
	lw	$2,20($fp)
	nop
	addu	$3,$3,$2
	lui	$2,%hi(B)
	lw	$4,16($fp)
	nop
	sll	$4,$4,2
	addiu	$2,$2,%lo(B)
	addu	$2,$4,$2
	sw	$3,0($2)
	lw	$2,16($fp)
	nop
	addiu	$2,$2,1
	sw	$2,16($fp)
$L4:
	lw	$2,16($fp)
	nop
	slt	$2,$2,16
	bne	$2,$0,$L5
	nop

	lui	$2,%hi(B)
	addiu	$2,$2,%lo(B)
	lw	$2,60($2)
	nop
	move	$4,$2
	jal	print_roman
	nop

	move	$2,$0
	move	$sp,$fp
	lw	$31,28($sp)
	lw	$fp,24($sp)
	addiu	$sp,$sp,32
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	main
	.size	main, .-main
	.ident	"GCC: (GNU) 5.2.0"
