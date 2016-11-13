/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#include "inst-decoder.h"
#include <iostream>
#include <functional>
#include <map>

/* TODO: add your instruction decoder */

/*
 * Class InstructionDecoder -- helper class for getting specific
 * information from the decoded instruction.
 */

/* select bits including start and stop and return */
uint32_t 
selectBits(uint8_t start, uint8_t stop, const uint32_t instruction){
//
  
  uint32_t selection; 
  int mask;
  
  selection = instruction >> start; //shift bits we want to the lowest position
  //create a mask by shifting ~0 (is all 1) the number of bits we want to the 
  //left and then inverting so we get all 0 everywhere except the part we want to keep
  mask = ~(~0 << (stop-start+1));      //(~ is the bitwise NOT operator)
  selection = selection & mask;     //take only the lowest bits

  return selection;
}

/* decode an Rtype instruction */
void
decodeRtype(const uint32_t instruction, DecodedInstruction decoded)
{
  decoded.rd = selectBits(7, 11, instruction);
  decoded.funct3 = selectBits(12, 14, instruction);
  decoded.rs1 = selectBits(15, 19, instruction);
  decoded.rs2 = selectBits(20, 24, instruction);
  decoded.funct7 = selectBits(25, 31, instruction);
}


/* Decodes a single instruction. The decoded instruction should be
 * stored in the class member "decoded" of type DecodedInstruction.
 */
void
InstructionDecoder::decodeInstruction(const uint32_t instruction)
{
  /* TODO: decode "instruction" and write the result to the variable
   * "decoded" (of type DecodedInstruction).
   */
  int func3;
  DecodedInstruction decoded;
  
  decoded.opcode = selectBits(0,6, instruction);
  std::cout << +decoded.opcode;
  
  //decode the different types
  switch (decoded.opcode){
    case 0b0010011://R-type
      decodeRtype(instruction, decoded);
      if (decoded.funct3 == 0){
        if (decoded.funct7 == 0){//ADDW
          decoded.ctrl = INT;
        }
      }
      break;
    case 0b0111011://R-type
      decodeRtype(instruction, decoded);
      break;
    case 0b0011011://R-type
      func3 = selectBits(12,14, instruction);
      if (func3 != 000){
        decodeRtype(instruction, decoded);
      }
      else{
        //TODO
      }
      break;
    default:
      std::cout << " ";
//      std::cout << "unsupported opcode\n";    
      break;
  }                
}

DecodedInstruction
InstructionDecoder::getDecodedInstruction(void) const
{
  return decoded;
}

uint8_t 
InstructionDecoder::getAdressA(void) const
{
  return decoded.rs1;
}

uint8_t 
InstructionDecoder::getAdressA(void) const
{
  return decoded.rs2;
}
/* TODO: add implementations of member functions of InstructionDecoder */
