/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.h - RISC-V instruction decoder.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __INST_DECODER_H__
#define __INST_DECODER_H__

#include <stdexcept>
#include <cstdint>
#include "reg-file.h" //added

/*add enums and constants necessary for your instruction decoder */
enum AluControl { INT, FLOAT, DIV, NOP};
enum instructionName { ADDW, AUIPC};

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
  instructionName name;

  uint8_t opcode;
  uint8_t rd;
  uint8_t funct3;
  uint8_t rs1;
  uint8_t rs2;
  uint8_t funct7;
   
  uint32_t imm;
  
  AluControl ctrl;
  /* TODO: fill with data instruction data: opcode, register numbers, etc... */
};

std::ostream &operator<<(std::ostream &os, const DecodedInstruction &decoded);


/* InstructionDecoder component to be used by class Processor */
class InstructionDecoder
{
  public:
    void                decodeInstruction(const uint32_t instruction);

    DecodedInstruction decoded;

    DecodedInstruction  getDecodedInstruction(void) const;	
		AluControl					getAluCtrl(void) const;
    uint8_t             getAdressA() {return decoded.rs1;}
    uint8_t             getAdressB() {return decoded.rs2;}
    uint8_t             getAdressReturn() {return decoded.rd;}
    instructionName     getInstructionName() {return decoded.name;}
    
    /* TODO: add methods that the processor class can use to obtain
     * the necessary data from the decoded instruction.
     */
  private:
    void decodeUtype(const uint32_t instruction);
		void decodeRtype(const uint32_t instruction);
};


#endif /* __INST_DECODER_H__ */
