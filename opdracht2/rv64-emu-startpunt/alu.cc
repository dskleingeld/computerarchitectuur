/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#include "alu.h"
#include "inst-decoder.h"
#include <iostream>

void
ALU::execute(void)
{
  /* TODO: add code to perform operation set on the ALU */
  std::cout<< "ctrl: "<<ctrl << "\n";
  std::cout<< "INT: "<<INT << "\n";
  switch (ctrl){
  	case INT:
  		result = A+B;
  		std::cout << result << "\n";
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
