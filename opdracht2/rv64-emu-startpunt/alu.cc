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
  //std::cout<< "ctrl: "<<ctrl << "\n";
  //std::cout<< "INT: "<<INT << "\n";
  switch (ctrl){
  	case opADD:
  		result = A+B;
		  std::cout<<"A: "<<A<<" B: "<<B<<"\n";
  		std::cout <<"result: "<<result << "\n";
  		break;
		case opSUB:
  		result = A-B;
			break;
		case opSHIFT:
  		result = A/B;
			break;
		case opNOP:
			break;
		default:
			break;
	}
  A = B = 0;
}
