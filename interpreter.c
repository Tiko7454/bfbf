#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define PRE_BF_ERROR 1
#define BF_ERROR 2

#define MEM_SIZE 65536
#define CHUNK 256
#define MAX_INSTUCTION_COUNT 1000000

#define run_once                                  \
    do {                                          \
        static int ran = 0;                       \
        assert(ran == 0 && "ran more than once"); \
        ran++;                                    \
    } while (0)

void print_memory(const unsigned char* memory, size_t from, size_t upto) {
    for (const unsigned char* m = memory - from; m != memory + upto; m++) {
        int content = *m;
        if (m == memory) {
            fprintf(stderr, "\033[32m[%3d]\033[0m", content);
        } else {
            fprintf(stderr, "[%3d]", content);
        }
    }
    fputs("\n", stderr);
}

const size_t *create_jump_table(char* instructions) {
    run_once;
    static size_t jump_table[MAX_INSTUCTION_COUNT] = {0};
    int stack[MAX_INSTUCTION_COUNT] = {0};
    size_t top = 0;
    for (int i = 0; i < MAX_INSTUCTION_COUNT; i++) {
        if (instructions[i] == '[') {
            stack[top++] = i;
        } else if (instructions[i] == ']') {
            if (top == 0) {
                top = -1;
                break;
            }
            int start_instruction = stack[--top];
            jump_table[start_instruction] = i;
            jump_table[i] = start_instruction;
        }
    }
    if (top != 0) {
        fputs("[] are not balanced\n", stderr);
        exit(BF_ERROR);
    }
    return jump_table;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fputs("arg count doesn't match the expected\n", stderr);
        return PRE_BF_ERROR;
    }
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        fputs("cannot open file\n", stderr);
        return PRE_BF_ERROR;
    }

    char instructions[MAX_INSTUCTION_COUNT] = {0};
    {
        size_t ret = CHUNK;
        for (int i = 0; ret == CHUNK; i++) {
            ret = fread(instructions + i * CHUNK, 1, CHUNK, file);
        }
    }
    fclose(file);

    const size_t* jump_table = create_jump_table(instructions);

    unsigned char memory[MEM_SIZE] = {0};
    size_t head = 0;
    size_t ip = 0;
    int do_debug = instructions[ip] == '`';
    while (1) {
        char ch = instructions[ip];
        if (ch == 0) {
            break;
        }
        if (do_debug) {
            fprintf(stderr, "now doing %c on %lu\n", instructions[ip], ip);
        }
        switch (ch) {
            case '+':
                memory[head]++;
                break;
            case '-':
                memory[head]--;
                break;
            case '<':
                if (head-- == 0) {
                    fputs("memory underflow\n", stderr);
                    return BF_ERROR;
                }
                break;
            case '>':
                if (++head == MEM_SIZE) {
                    fputs("memory overflow\n", stderr);
                    return BF_ERROR;
                }
                break;
            case '[':
                if (memory[head] == 0) {
                    ip = jump_table[ip];
                }
                break;
            case ']':
                ip = jump_table[ip] - 1;
                break;
            case ',':
                memory[head] = getchar();
                break;
            case '.':
                putchar(memory[head]);
                break;
            case '?':
                fprintf(stderr, "%lu\n", head);
                break;
            case '!':
                print_memory(memory+head, 0, 16);
                break;
            case '@':
                print_memory(memory+head, 7, 8);
                break;
            case '\'':
                fprintf(stderr, "%d\n", (int)memory[head]);
                break;
            default:
                break;
        }
        ip++;
    }
}
