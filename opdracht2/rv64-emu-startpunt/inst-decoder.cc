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

#include <bitset>

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

  //std::cout << "mask: "<< std::bitset<32>(mask) << "\n";
  //std::cout << "selection: "<< std::bitset<32>(selection)<< "\n";

  selection = selection & mask;     //take only the lowest bits
  //std::cout <<"result: "<<std::bitset<32>((uint8_t)selection)<< "\n";

  return selection;
}

/* decode an Rtype instruction */
void
InstructionDecoder::decodeRtype(const uint32_t instruction)
{
  decoded.rd = (uint8_t)selectBits(7, 11, instruction);			//target register
  decoded.funct3 = (uint8_t)selectBits(12, 14, instruction);
  decoded.rs1 = (uint8_t)selectBits(15, 19, instruction);
  decoded.rs2 = (uint8_t)selectBits(20, 24, instruction);
  decoded.funct7 = (uint8_t)selectBits(25, 31, instruction);

  std::cout<<"funct3: "<<std::bitset<3>(decoded.funct3)
           <<" funct7: "<<std::bitset<7>(decoded.funct7)<<"\n";
}

/* decode an Itype instruction */
void
InstructionDecoder::decodeItype(const uint32_t instruction)
{
	decoded.rd = (uint8_t)selectBits(7, 11, instruction);			//target register
  decoded.funct3 = (uint8_t)selectBits(12, 14, instruction);
  decoded.rs1 = (uint8_t)selectBits(15, 19, instruction);
  decoded.imm = (uint8_t)selectBits(20, 31, instruction);

  std::cout<<"funct3: "<<std::bitset<3>(decoded.funct3);
}

/* decode an Utype instruction */
void
InstructionDecoder::decodeUtype(const uint32_t instruction)
{
  decoded.rd = (uint8_t)selectBits(7, 11, instruction);			//target register
  decoded.imm = (uint8_t)selectBits(12, 31, instruction);

  std::cout<<"rd: "<<std::bitset<4>(decoded.rd)
           <<" imm: "<<std::bitset<20>(decoded.imm)<<"\n";
}

/* decode an UJtype instruction */
void
InstructionDecoder::decodeUJtype(const uint32_t instruction)
{ //TODO check imm notation
  decoded.rd = (uint8_t)selectBits(7, 11, instruction);			//target register
  decoded.imm = (uint8_t)selectBits(12, 31, instruction);

  std::cout<<"rd: "<<std::bitset<4>(decoded.rd)
           <<" imm: "<<std::bitset<20>(decoded.imm)<<"\n";
}

/* Decodes a single instruction. The decoded instruction should be
 * stored in the class member "decoded" of type DecodedInstruction.
 */
void
InstructionDecoder::decodeInstruction(const uint32_t instruction)
{
  int func3;
  
  decoded.opcode = (uint8_t)selectBits(0,6, instruction);
  std::cout << "decoded opcode: "<< std::bitset<7>(decoded.opcode) << "\n";
  //decode the different types
  switch (decoded.opcode){
    case 0b0010011://R-type or I-type
      func3 = selectBits(12,14,instruction);
			switch(func3){
				case 0b000:
					decodeItype(instruction);
					decoded.name = ADDI;
				case 0b001:
					break;
				case 0b101:
					break;
				default:
					std::cout<<"unknown func3: "<<func3<<"\n";
					break;
			}
			//decodeRtype(instruction);
      //if (decoded.funct3 == 0){
      //  if (decoded.funct7 == 0){
      //  }
      
      break;
    case 0b0111011://R-type
      decodeRtype(instruction);
      if(decoded.funct7 == 0){//ADDW
        decoded.name = ADDW;
      }

      break;
   // case 0b0011011://R-type
   //   func3 = selectBits(12,14, instruction);
   //   if (func3 != 000){

   //   decodeRtype(instruction);
   //   }
   //   else{
   //     //TODO
   //   }
   //   break;
    case 0b0010111://U-type, AUIPC
      decodeUtype(instruction);
      decoded.imm = decoded.imm << 12;
      decoded.name = AUIPC;
      std::cout<<"decoded imm: "<<std::bitset<32>(decoded.imm);
			break;
    case 0b0110111://U-type, LUI
   		decodeUtype(instruction);
			decoded.imm = decoded.imm << 12;
			decoded.name = LUI;
			break;
		case ob11o1111://UJ-type, JAL
			break;
    default:
      std::cout << " ";
      std::cout << "unsupported opcode\n";    
      break;
  }                
}

DecodedInstruction
InstructionDecoder::getDecodedInstruction(void) const
{
  return decoded;
}

AluControl
InstructionDecoder::getAluCtrl(void) const
{
	return decoded.ctrl;
}


/* TODO: add implementations of member functions of InstructionDecoder */
