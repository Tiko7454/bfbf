bf:
    just compile_via_int bf.bf
    mv a.out bf

compile INPUT: bf
    ./bf < {{INPUT}} > a.nasm
    nasm a.nasm -felf32 -o a.o
    ld a.o -melf_i386 -o a.out

compile_via_int INPUT: interpreter
    ./interpreter bf.bf < {{INPUT}} > a.nasm
    nasm a.nasm -felf32 -o a.o
    ld a.o -melf_i386 -o a.out

interpret BF_FILE: interpreter
    ./interpreter {{BF_FILE}}

interpreter:
    gcc interpreter.c -o interpreter

