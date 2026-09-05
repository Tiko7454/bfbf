# bfbf

bfbf is a BF-to-x86 Linux compiler written in BF.

The compiler translates BF source files into 32-bit x86 assembly (NASM syntax).

Input and output are handled using the Linux `read` and `write` system calls.

## Requirements

* Linux
* NASM
* GNU ld with 32-bit x86 support

## Usage

The complete build process is defined in the Justfile.

To build the compiler:

```bash
just bf
```

To compile a BF program:

```bash
just compile a.bf
```

Or compile manually:

```bash
./bf < a.bf > a.asm
nasm -f elf32 a.asm -o a.o
ld -m elf_i386 a.o -o a.out
```

Run the resulting program:

```bash
./a.out
```
