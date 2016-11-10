/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __ALU_H__
#define __ALU_H__

#include "inst-decoder.h"

#include <map>

/* The ALU component performs the specified operation on operands A and B,
 * placing the result in result. The operation is specified through
 * opcode and/or function code.
 */
class ALU
{
  public:
    void setA(RegValue A) { this->A = A; }
    void setB(RegValue B) { this->B = B; }

    RegValue getResult() const { return result; }

    /* TODO: add methods to set opcode, function code and other information
     * necessary to identify the operation to execute on the ALU.
     */
		AluControl ctrl;		
		
    void execute();

  private:
    RegValue A;
    RegValue B;
    RegValue result;
};

#endif /* __ALU_H__ */
