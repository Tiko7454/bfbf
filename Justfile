build: interpreter
    true

interpreter:
    gcc interpreter.c -o interpreter

compile_via_int INPUT: interpreter
    ./interpreter bf.bf < {{INPUT}} > a.nasm
    nasm a.nasm -felf32 -o a.o
    ld a.o -melf_i386 -o a.out

int BF_FILE: interpreter
    ./interpreter {{BF_FILE}}
