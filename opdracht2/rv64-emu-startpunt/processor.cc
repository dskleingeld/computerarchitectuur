/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    processor.h - Processor class tying all components together.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#include "processor.h"
#include "inst-decoder.h"
#include "serial.h"

/* Dumps disassembled instructions if enabled. */
/*#ifdef INSTR_DUMP*/

#include <iostream>
#include <iomanip>

Processor::Processor(ELFFile &program)
  : nCycles(0), nInstructions(0),
    PC(program.getEntrypoint()), bus(program.createMemories()),
  control(new SysControl(0x270))
{
  bus.addClient(std::shared_ptr<MemoryInterface>(new Serial(0x200)));
  bus.addClient(control);
}

/* This method is used to initialize registers using values
 * passed as command-line argument.
 */
void
Processor::initRegister(RegNumber regnum, RegValue value)
{
  regfile.writeRegister(regnum, value);
}

/* Processor main loop. Each iteration should execute an instruction.
 * One step in executing and instruction takes 1 clock cycle.
 */
void
Processor::run(void)
{
  while (! control->shouldHalt())
    {
      try
        {
          ++nCycles;
          instructionFetch();

          ++nCycles;
          bool jumped = instructionDecode();
          if (jumped)
            continue;

          ++nCycles;
          execute();

          ++nCycles;
          memory();

          ++nCycles;
          writeBack();
        }
      catch (std::exception &e)
        {
          /* Catch exceptions such as IllegalInstruction and InvalidAddress */
          std::cerr << "ABNORMAL PROGRAM TERMINATION; PC = "
                    << std::hex << PC << std::dec << std::endl;
          std::cerr << "Reason: " << e.what() << std::endl;
          break;
        }
    }

  dumpRegisters();
  dumpStatistics();
}

void
Processor::instructionFetch(void)
{
  instruction = bus.readWord(PC);
  std::cout << "PC: "<< std::hex << PC <<", ";
  PC += 4;
}

/* Returns whether jump has already occurred during this step and further
 * steps can be skipped.
 */
bool
Processor::instructionDecode(void) {
  RegValue A;
  RegValue B;
  instructionName name;
  instructionType type;

  decoder.decodeInstruction(instruction);
#ifdef INSTR_DUMP
  std::cerr << "instruction: "<<decoder.getDecodedInstruction() << std::endl;
#endif /* INSTR_DUMP */

  type = decoder.getInstructionType();
  switch(type){
    case R:
      A = regfile.readRegister(decoder.getAdressA());
      B = regfile.readRegister(decoder.getAdressB());
      alu.setA(A);
      alu.setB(B);
      alu.ctrl = decoder.getAluCtrl();
  }

  name = decoder.getInstructionName();
  switch(name){
    case ADD:
    case ADDW:
      //get A and B
      A = regfile.readRegister(decoder.getAdressA());
      B = regfile.readRegister(decoder.getAdressB());
      alu.ctrl = decoder.getAluCtrl();
      alu.setA(A);
      alu.setB(B);
      break;
    case AUIPC:
      A = (RegValue)PC;
      B = decoder.decoded.imm;
      alu.ctrl = decoder.getAluCtrl();
      alu.setA(A);
      alu.setB(B);
      break;
		case LUI:
			A = 0;
			B = decoder.decoded.imm;
      alu.ctrl = decoder.getAluCtrl();
			alu.setA(A);
			alu.setB(B);
			break;
		case ADDI:
			A = 0;
			B = decoder.decoded.imm;
      alu.ctrl = decoder.getAluCtrl();
			alu.setA(A);
			alu.setB(B);
			break;
		case JAL:
			A = PC;
			B = decoder.decoded.imm;
      alu.ctrl = decoder.getAluCtrl();
			alu.setA(A);
			alu.setB(B);
			return true;
			break;
		case SW:
			A = PC;
			B = decoder.decoded.imm;
      alu.ctrl = decoder.getAluCtrl();
			alu.setA(A);
			alu.setB(B);
			return true;
			break;
  }
  return false;
}

void
Processor::execute(void)
{
  alu.execute();
}

uint64_t
Processor::performLoad(uint8_t size, bool signExtend, MemAddress addr)
{
  uint64_t value;
  /* TODO: implement the different load operations */
  switch(size){
  	case 1:
  		value = (uint64_t)bus.readByte(addr);
  		break;
		case 2:
  		value = (uint64_t)bus.readHalfWord(addr);
  		break;
		case 4:
  		value = (uint64_t)bus.readWord(addr);
  		break;
		case 8:
  		value = (uint64_t)bus.readDoubleWord(addr);
  		break;
		default:
      value = 0;
      break;
  }
  return value;
}

void
Processor::performStore(uint8_t size, RegValue value, MemAddress addr)
{
  /* TODO: implement the different store operations */
  switch(size){
  	case 1:
  		bus.writeByte(addr, (uint8_t)value);
  		break;
		case 2:
  		bus.writeHalfWord(addr, (uint16_t)value);
  		break;
		case 4:
  		bus.writeWord(addr, (uint32_t)value);
  		break;
		case 8:
  		bus.writeDoubleWord(addr, (uint64_t)value);
  		break;
		default:
			break;
  }
}

void
Processor::memory(void)
{
  /* TODO: implement, recall that the current ALU result can be
   * obtained using alu.getResult(). For memory-operations, this
   * ALU result is the effective memory address.
   */

  if(decoder.decoded.name == SW){
  	MemAddress addr = alu.getResult();
  	RegValue value = regfile.readRegister(decoder.decoded.rs2);
  	performStore(4, value, addr);
  }
}

void
Processor::writeBack(void)
{
  /* TODO: implement, recall that the current ALU result can be
   * obtained using alu.getResult() */
  if(decoder.decoded.name != SW){
  	uint8_t returnAddr = decoder.getAdressReturn();
  	RegValue result = alu.getResult();
		regfile.writeRegister(returnAddr, result);
	}
	if(decoder.decoded.name == JAL){
		uint8_t returnAddr = decoder.getAdressReturn();
		regfile.writeRegister(returnAddr, PC);
		PC = alu.getResult();
	}
}

void
Processor::dumpRegisters(void) const
{
  auto storeFlags(std::cerr.flags());

  for (int i = 0; i < 32 / 2; ++i)
    {
      std::cerr << "R" << std::setw(2) << std::setfill('0') << i << " "
                << std::setw(18)
                << std::hex << std::showbase
                << regfile.readRegister(i)
                << "\t";
      std::cerr.setf(storeFlags);
      std::cerr << "R" << std::setw(2) << (i + 32/2) << " "
                << std::setw(18)
                << std::hex << std::showbase
                << regfile.readRegister(i + 32/2)
                << std::endl;
      std::cerr.setf(storeFlags);
    }
}

void
Processor::dumpStatistics(void) const
{
  std::cerr << nCycles << " clock cycles, "
            << nInstructions << " instructions executed." << std::endl
            << "CPI: " << ((float)nCycles / nInstructions) << std::endl;
}
