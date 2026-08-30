build: interpreter
    true

interpreter:
    gcc interpreter.c -o interpreter

int BF_FILE: interpreter
    ./interpreter {{BF_FILE}}
