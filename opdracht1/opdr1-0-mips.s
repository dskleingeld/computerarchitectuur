	.file	1 "opdr1.c"
	.section .mdebug.abi32
	.previous                          ####################################
	.nan	legacy                       # PLEASE LOOK IN REPORT FOR HIGHER #
	.module	fp=32                      # LEVEL EXPLENATION, SORRY 				#  
	.module	nooddspreg                 ####################################
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
	addiu	$sp,$sp,-32 #reserveer 32 bit geheugen in de stack voor variablen
	sw	$31,28($sp)   #store the content of reg 31 into the last 4 bytes of the stack
	sw	$fp,24($sp)   #store the frame pointer in byte 24 to 28 of the stack 
	move	$fp,$sp     #set frame pointer to the current stack pointer
	sw	$0,16($fp)    #set byte 16 to 20 to 0 #set i = 0
	b	$L2             #execute the code block at $L2
	nop               #do nothing for one cycle*

$L3:
	lw	$2,16($fp)  #load bytes 16-20 from the stack into reg 2
	nop             
	sll	$3,$2,1     #shift values from $2 1 to the left and introduce new zero at the rightmost pos.
	                #store this in $3
	lui	$2,%hi(A)   #load the high values of A (= int array) into the higher 16 bytes of reg 2 
	lw	$4,16($fp)  #set $4 to byte 16 to 20
	nop             
	sll	$4,$4,2     		#shift $4 left and introduce a zero right. left shift = *2^n in this case n=2
	addiu	$2,$2,%lo(A)  #add the lower part of A to $2 (which already contains the upper part of A)
	addu	$2,$4,$2      #$2 = $4 + $2
	sw	$3,0($2)        #store $3 into the stack at $2 #TODO whats the zero for? not +0 right?
	lw	$2,16($fp)      #load into $2 bytes 16-20 from the stack
	nop
	addiu	$2,$2,1       #add one to $2
	sw	$2,16($fp)      #store the new $2 stack at bytes 16-20
	
$L2:
	lw	$2,16($fp)    #set register 2 to the zeros we put in this part of the frame at line 28
	nop               #do nothing
	slt	$2,$2,16      #set $2 to one if $2 < 16; sla in register 2 true of false (1 of 0) als i kleiner of niet dan 16
	bne	$2,$0,$L3     #goto L3 if $2 != $0 (=0000 as: "Register $0 always holds 0")
	nop

	li	$2,95	        #0x5f  #load 95 into R2; R2 = 95 (is C) en sla C op in bytes 20 tot 24
	sw	$2,20($fp)    #store R2 at bit 20 for a length of 4 bytes

	lui	$2,%hi(B)     #stop t higher part van B's adress in R2; B[0] = 0     
	sw	$0,%lo(B)($2) #store zero at the higher part of B+the lower part of B

	li	$2,1	        # 0x1 #set R2 to 1; set i to 1
	sw	$2,16($fp)
	b	$L4
	nop

$L5:
	lw	$2,16($fp)			# $2 = i
	nop
	addiu	$3,$2,-1			# $3 = i - 1
	lui	$2,%hi(A)				# laad het hoge deel van A in $2
	sll	$3,$3,2					# $3 = $3*4 ======> $3 = (i-1)*4
	addiu	$2,$2,%lo(A)	# laad ook het lage deel in $2 
	addu	$2,$3,$2			# add $3 to $2 en sla op in $2 ====> (i-1)*4+adress_A
	lw	$3,0($2)				# $3 = A[i-1]
	lw	$2,20($fp)			# $2 = 95, load C (=95) in $2
	nop
	addu	$3,$3,$2			# $3 = $3+$2 = A[i-1]+95
	lui	$2,%hi(B)				# load high pat of B into $2
	lw	$4,16($fp)			# $4 = i
	nop
	sll	$4,$4,2					# $4 = i*4
	addiu	$2,$2,%lo(B)  # complete load of adress of B into $2
	addu	$2,$4,$2			# add i*4 to addess of B and store in $2
	sw	$3,0($2)				# 
	lw	$2,16($fp)
	nop
	addiu	$2,$2,1
	sw	$2,16($fp)
$L4:
	lw	$2,16($fp)
	nop
	slt	$2,$2,16 		#als $2 > 16 set $2 to 1 (true)
	bne	$2,$0,$L5		#"Branch on not equal", als $2 != 0 ga naar L5 
	nop

	lui	$2,%hi(B)				# load upper part of B into upper part of reg 2
	addiu	$2,$2,%lo(B)	# add lower part of B to reg 2
	lw	$2,60($2)				# load byte 60 tm 64 van geheugenadress in reg 2
	nop
	move	$4,$2					# copieer de waarde in reg 2 naar reg 4
	jal	print_roman			
	nop

	move	$2,$0
	move	$sp,$fp				# set the stack pointer to the frame pointer
	lw	$31,28($sp)			# load the variable at 28 bytes in the stack into reg 31
	lw	$fp,24($sp)			# change the frame pointer to byte 24 of the stack pointer
	addiu	$sp,$sp,32		# move the stack pointer 32 bytes forward
	j	$31								# jump to the code in register 31 
	nop

	.set	macro
	.set	reorder
	.end	main
	.size	main, .-main
	.ident	"GCC: (GNU) 5.2.0"
	
	
*after a jump or branch one more instruction is executed
