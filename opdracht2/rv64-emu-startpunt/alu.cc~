/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#include "alu.h"


void
ALU::execute(void)
{
  /* TODO: add code to perform operation set on the ALU */
  switch (ctrl){
  	case INT:
  		result = A+B;
  		break;
		case FLOAT:
  		result = A+B;
			break;
		case DIV:
  		result = A/B;
			break;
		case NOP:
			break;
		default:
			break;
	}		
}
