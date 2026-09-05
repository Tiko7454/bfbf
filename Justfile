bf:
    just compile_via_int bf.bf
    mv a.out bf

compile INPUT: bf
    ./bf < {{INPUT}} > a.asm
    just nasm

compile_via_int INPUT: interpreter
    ./interpreter bf.bf < {{INPUT}} > a.asm
    just nasm

interpret BF_FILE: interpreter
    ./interpreter {{BF_FILE}}

interpreter:
    gcc interpreter.c -o interpreter

nasm:
    nasm a.asm -felf32 -o a.o
    ld a.o -melf_i386 -o a.out
