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
}

/* decode an Itype instruction */
void
InstructionDecoder::decodeItype(const uint32_t instruction)
{
	decoded.rd = (uint8_t)selectBits(7, 11, instruction);			//target register
  decoded.funct3 = (uint8_t)selectBits(12, 14, instruction);
  decoded.rs1 = (uint8_t)selectBits(15, 19, instruction);
  decoded.imm = (uint8_t)selectBits(20, 31, instruction);
}

/* decode an Utype instruction */
void
InstructionDecoder::decodeUtype(const uint32_t instruction)
{
  decoded.rd = (uint8_t)selectBits(7, 11, instruction);			//target register
  decoded.imm = (uint8_t)selectBits(12, 31, instruction);
	decoded.imm = decoded.imm << 12;

  std::cout<<"rd: "<<std::bitset<4>(decoded.rd)
           <<" imm: "<<std::bitset<20>(decoded.imm)<<"\n";
}

/* decode an UJtype instruction */
void
InstructionDecoder::decodeUJtype(const uint32_t instruction)
{ //TODO check imm notation
  decoded.rd = (uint8_t)selectBits(7, 11, instruction);			//target register
  decoded.imm = (uint8_t)selectBits(12, 31, instruction);

  decoded.imm = (decoded.imm & 0b10000000000000000000) |
                (decoded.imm & 0b1111111111) << 10     |
                (decoded.imm & 0b10000000000) << 11    |
                (decoded.imm & 0b1111111000000000000);

  std::cout<<"Jal imm: "<<decoded.imm<<"\n";
}

void
InstructionDecoder::decodeStype(const uint32_t instruction)
{
  int immA;
  int immB;

  immA = (uint8_t)selectBits(7, 11, instruction);
  decoded.funct3 = (uint8_t)selectBits(12, 14, instruction);
  decoded.rs1 = (uint8_t)selectBits(15, 19, instruction);
  decoded.rs2 = (uint8_t)selectBits(20, 24, instruction);
  immB = (uint8_t)selectBits(25, 31, instruction);

  decoded.imm = (immA & 0b11111) |
                ((immB & 0b00000111111) << 5);
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
					break;
				default:
					std::cout<<"unknown func3: "<<func3<<"\n";
					break;
			}
      break;
    case 0b0111011://R-type
      decodeRtype(instruction);
      if(decoded.funct7 == 0)//ADDW
        decoded.name = ADDW;
      break;
    case 0b0010111://U-type, AUIPC
      decodeUtype(instruction);
      decoded.imm = decoded.imm << 12;
      decoded.name = AUIPC;
      std::cout<<"decoded imm: "<<std::bitset<32>(decoded.imm);
			break;
    case 0b0110111://U-type, LUI
   		decodeUtype(instruction);
			decoded.name = LUI;
			break;
		case 0b1101111://UJ-type, JAL
		  decodeUJtype(instruction);
		  decoded.name = JAL;
			break;
		case 0b0100011://S-type
		  decodeStype(instruction);
		  switch(decoded.funct3){
		    case 0b010://SW
		      decoded.name = SW;
		      break;
        case 0b011://SD
          decoded.name = SD;
          break;
		  }
      std::cout<<"func3: "<<+decoded.funct3<<"\n";
		  break;
    default:
      std::cout << " ";
      std::cout << "unsupported opcode\n";
      break;
  }
}

std::string
InstructionDecoder::getDecodedInstruction(void) const
{
  std::string name;
  switch(decoded.name){
    case ADDW:
      name = "ADDW";
      break;
    case ADDI:
      name = "ADDI";
      break;
    case AUIPC:
      name = "AUIPC";
      break;
    case LUI:
      name = "LUI";
      break;
    case JAL:
      name = "JAL";
      break;
    case SW:
      name = "SW";
      break;
  }
  return name;
}

AluControl
InstructionDecoder::getAluCtrl(void) const
{
	return decoded.ctrl;
}


/* TODO: add implementations of member functions of InstructionDecoder */
