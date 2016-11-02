/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    reg-file.h - Register file.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __REG_FILE__
#define __REG_FILE__

#include "arch.h"

#include <array>

/* For now hard-coded for a single zero-register and
 * (NumRegs - 1) general-purpose registers.
 */
class RegisterFile
{
  public:
    RegisterFile()
    {
      /* Zero initialize all registers */
      registers.fill(0);
    }

    RegValue readRegister(const uint8_t regnum) const
    {
      if (regnum == 0)
        return 0;
      return registers[regnum - 1];
    }

    void writeRegister(const uint8_t regnum,
                       RegValue      value)
    {
      if (regnum == 0)
        return;
      registers[regnum - 1] = value;
    }

  private:
    std::array<RegValue, NumRegs - 1> registers;
};

#endif /* __REG_FILE_H__ */
