#include <stdlib.h>
#include <stdio.h>
#include "brainfuck/brainfuck.h"

char* readFile(char* filename){
    FILE *file = fopen(filename, "r");
    if(file == 0){
        printf("Can't open file %s", filename);
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    size_t size = (size_t) ftell(file);
    rewind(file);
    char* buffer = malloc(sizeof(char) * size);
    size_t readed = fread(buffer, 1, size, file);
    if(readed != size){
        printf("Error while reading file: %li/%li bytes readed", readed, size);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    return buffer;
}

int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("Usage: %s filename", argv[0]);
        exit(EXIT_FAILURE);
    }
    char * program_string = readFile(argv[1]);
    struct StatementHandler *sth_array = makeSthArray();
    struct Machine* machine = new_Machine((size_t)30000);
    struct Operator* program = readProgram(program_string, sth_array);
    free(program_string);
    setProgram(machine,program);
    while(canDoStep(machine)){
        stepProgramm(machine);
    }

    delete_Machine(machine);
    delete_Operator(program);
    free(sth_array);

    return 0;
}