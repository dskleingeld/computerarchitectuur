/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    main.cc - Command line parsing, program start.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#include "arch.h"

#include <iostream>
#include <vector>
#include <regex>

#include <getopt.h>
#include <cstdlib>

#include "elf-file.h"
#include "processor.h"

static void
showHelp(const char *progName)
{
  std::cerr << progName << " [-r reginit] <programFilename>" << std::endl;
  std::cerr <<
R"HERE(
    Where 'reginit' is a register initializer in the form
    rX=Y with X a register number and Y the initializer value.
)HERE";
}

using RegisterInit = std::pair<RegNumber, RegValue>;

static bool
parseRegisterInit(std::string option,
                  std::vector<RegisterInit> &initializers)
{
  std::regex init_regex("r([0-9]{1,2})=([0-9]+)");
  std::smatch match;

  if (std::regex_match(option, match, init_regex))
    {
      int regnum = atoi(match[1].str().c_str());

      /* FIXME: should perhaps not be verified here, but during the
       * actual initialization of the register file.
       */
      if (regnum < 1 || regnum > (NumRegs - 1))
        {
          std::cerr << "Error: Invalid register number " << regnum << std::endl;
          return false;
        }

      initializers.push_back(std::make_pair(regnum,
                                            atoi(match[2].str().c_str())));
      return true;
    }

  return false;
}

static int
run(const char *programFilename,
    const std::vector<RegisterInit> &initializers)
{
  ELFFile program;

  if (! program.load(programFilename))
    {
      std::cerr << "Couldn't load program: not an (valid/supported) ELF file"
                << std::endl;
      return 1;
    }

  Processor p(program);

  for (auto &initializer : initializers)
    p.initRegister(initializer.first, initializer.second);

  p.run();

  return 0;
}

int
main(int argc, char **argv)
{
  char c;
  std::vector<RegisterInit> initializers;

  const char *progName = argv[0];

  while ((c = getopt(argc, argv, "r:h")) != -1)
    {
      switch (c)
        {
          case 'r':
            if (! parseRegisterInit(optarg, initializers))
              {
                std::cerr << "Error: Malformed register initialization specifier "
                          << optarg << std::endl;
                return 1;
              }
            break;

          case 'h':
          default:
            showHelp(progName);
            return 0;
            break;
        }
    }

  argc -= optind;
  argv += optind;

  if (argc < 1)
    {
      std::cerr << "Error: No executable specified." << std::endl << std::endl;
      showHelp(progName);
      return 1;
    }

  char *programFilename = argv[0];

  return run(programFilename, initializers);
}
