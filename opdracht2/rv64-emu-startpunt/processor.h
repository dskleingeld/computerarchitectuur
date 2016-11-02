/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    processor.h - Processor class tying all components together.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

#include "arch.h"

#include "elf-file.h"
#include "inst-decoder.h"
#include "alu.h"
#include "memory-bus.h"
#include "sys-control.h"

class Processor
{
  public:
    Processor(ELFFile &program);

    /* Command-line register initialization */
    void initRegister(RegNumber regnum, RegValue value);

    /* Instruction execution steps */
    void run(void);

    void instructionFetch(void);
    bool instructionDecode(void);
    void execute(void);
    void memory(void);
    void writeBack(void);

    /* Helpers for handling loads and stores. */
    uint64_t performLoad(uint8_t size, bool signExtend, RegValue value);
    void     performStore(uint8_t size, RegValue value, MemAddress addr);

    /* Debugging and statistics */
    void dumpRegisters(void) const;
    void dumpStatistics(void) const;

  private:
    /* Statistics */
    int nCycles;
    int nInstructions;

    /* Components making up the system */
    MemAddress PC;
    uint32_t instruction;
    RegValue result;

    InstructionDecoder decoder;
    RegisterFile regfile;
    ALU alu;

    MemoryBus bus;
    std::shared_ptr<SysControl> control;
};

#endif /* __PROCESSOR_H__ */
