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
  std::cout << "programm counter: "; +PC;
  PC += 4;
  
}

/* Returns whether jump has already occurred during this step and further
 * steps can be skipped.
 */
bool
Processor::instructionDecode(void)
{
  decoder.decodeInstruction(instruction);
#ifdef INSTR_DUMP
  std::cerr << decoder.getDecodedInstruction() << std::endl;
#endif /* INSTR_DUMP */

	//set alu control signal
	alu.ctrl = decoder.getAluCtrl();
  
  /* TODO: implement remaining logic to set up the ALU (set A, B and
   * other necessary control signals).
   */

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
  /* TODO: implement the different load operations */

  return 0;
}

void
Processor::performStore(uint8_t size, RegValue value, MemAddress addr)
{
  /* TODO: implement the different store operations */
}

void
Processor::memory(void)
{
  /* TODO: implement, recall that the current ALU result can be
   * obtained using alu.getResult(). For memory-operations, this
   * ALU result is the effective memory address.
   */
}

void
Processor::writeBack(void)
{
  /* TODO: implement, recall that the current ALU result can be
   * pbtained using alu.getResult() */
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
