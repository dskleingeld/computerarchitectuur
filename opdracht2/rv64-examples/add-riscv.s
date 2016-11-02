	.text
        .align 4
	.globl	_start
	.type	_start, @function
_start:
	addw	x1,x1,x2
	addw	x1,x1,x3
	addw	x1,x1,x4
	addw	x1,x1,x5
	addw	x1,x1,x2
	addw	x1,x1,x3
	addw	x1,x1,x4
	addw	x1,x1,x5
	.size	_start, .-_start
