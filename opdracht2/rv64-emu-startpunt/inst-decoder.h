/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.h - RISC-V instruction decoder.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __INST_DECODER_H__
#define __INST_DECODER_H__

#include "reg-file.h"

#include <stdexcept>
#include <cstdint>

/*add enums and constants necessary for your instruction decoder */
enum aluControl { INT, FLOAT, DIV };


/* Exception that should be thrown when an illegal instruction
 * is encountered.
 */
class IllegalInstruction : public std::runtime_error
{
  public:
    explicit IllegalInstruction(const std::string &what)
      : std::runtime_error(what)
    { }

    explicit IllegalInstruction(const char *what)
      : std::runtime_error(what)
    { }
};

/* Structure to keep together all data for a single decoded instruction.
 */
struct DecodedInstruction
{
  uint8_t opcode;
  uint8_t rd;
  uint8_t funct3;
  uint8_t rs1;
  uint8_t rs2;
  uint8_t funct7;
   
  uint32_t imm;
  
  aluControl ctrl;
  /* TODO: fill with data instruction data: opcode, register numbers, etc... */
};

std::ostream &operator<<(std::ostream &os, const DecodedInstruction &decoded);


/* InstructionDecoder component to be used by class Processor */
class InstructionDecoder
{
  public:
    void                decodeInstruction(const uint32_t instruction);

    DecodedInstruction  getDecodedInstruction(void) const;

    /* TODO: add methods that the processor class can use to obtain
     * the necessary data from the decoded instruction.
     */

  private:
    DecodedInstruction decoded(void) const;
    uint8_t getAdressA(void) const;
    uint8_t getAdressB(void) const;
};

/* List of different opcodes as constants */
const uint8_t OPCODE_ADDW = 0b0111011;

#endif /* __INST_DECODER_H__ */
