/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#include "inst-decoder.h"

#include <functional>
#include <map>
#include <iostream>
#include <sstream>


/* TODO: add code to format a "DecodedInstruction" */


/* Output operator to print DecodedInstructions. */
std::ostream &
operator<<(std::ostream &os, const DecodedInstruction &decoded)
{
  /* TODO: write a textual representation of the decoded instruction
   * in "decoded" to the output stream "os".
   */

  return os;
}
