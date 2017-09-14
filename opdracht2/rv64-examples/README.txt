To build "add-riscv.s":

  ./riscv-cc -o add-riscv.bin add-riscv.s

The C programs must be compiled together with the start-up code
minicrt-riscv.s. To build "hello.c"

  ./riscv-cc -o hello.bin hello.c minicrt-riscv.s

And to build all other programs:

  ./riscv-cc -o $(program).bin $(program).c roman.c minicrt-riscv.s





gcc crosscompiler: /usr/local/bin/riscv/bin/riscv64-unknown-linux-gnu-gcc

