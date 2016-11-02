/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    elf-file.h - ELF file parsing class.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __ELF_FILE_H__
#define __ELF_FILE_H__

#include "memory-interface.h"

#include <vector>
#include <memory>
#include <string>

/* The ELFFile class loads a program from an ELF file by creating memories
 * for every section that needs to be loaded. During construction of
 * the Processor class, these memories are added to the memory bus
 * of the system.
 */
class ELFFile
{
  public:
    ELFFile();
    ~ELFFile();

    bool bad(void) const { return isBad; }

    bool load(const std::string &filename);
    void unload(void);

    std::vector<std::shared_ptr<MemoryInterface>> createMemories(void);
    uint64_t getEntrypoint(void) const;

  private:
    bool isBad;

    int fd;
    size_t programSize;
    void *mapAddr;


    bool isELF(void) const;
    bool isTarget(const uint8_t elf_class,
                  const uint8_t endianness,
                  const uint8_t machine) const;
};

#endif /* __ELF_FILE_H__ */
