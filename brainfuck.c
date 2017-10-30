//
// Created by madri on 25.10.2017.
//
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include "brainfuck.h"

struct Machine *new_Machine(size_t size){
    struct Machine* machine = (struct Machine*) malloc(sizeof(struct Machine));
    machine->_tape = calloc(size, sizeof(char));
    machine->_size = size;
    machine->_position = 0;
    return machine;
}

void delete_Machine(struct Machine * machine) {
    free(machine->_tape);
    free(machine);
}

void clearState(struct Machine* machine){
    machine->_current_stmt = 0;
    machine->_position = 0;
    for(size_t i = 0; i < machine->_size; i++){
        machine->_tape[i] = 0;
    }
}

void setProgram(struct Machine* machine, struct Operator* program){
    clearState(machine);
    machine->_program = program;
    machine->_current_stmt = program;
}

int canDoStep(struct Machine *machine){
    return machine->_current_stmt != 0;
}

/*
struct StatementHandler* new_StatementHandler(char symbol, struct Operator* (*handler)(struct Machine*), enum loop type){
    struct StatementHandler* sth = (struct StatementHandler*) malloc(sizeof(struct StatementHandler));
    sth->handler = handler;
    sth->symbol = symbol;
    sth->loop_type = type;
    return sth;
}

void delete_StatementHandler(struct StatementHandler *sth){
    free(sth);
}
*/

struct Operator* new_Operator(struct StatementHandler* sth){
    struct Operator* op = (struct Operator*) malloc(sizeof(struct Operator));
    op->sth = sth;
    return op;
}

void delete_Operator(struct Operator* op){
    struct Operator* next;
    while(op){
        next = op->next;
        free(op);
        op = next;
    }
}

struct Operator *readProgram(char *program, struct StatementHandler *handlers) {
    size_t op_count = strlen(program);
    char op_symbol;
    struct Operator *head = 0, *current = 0, *prev = 0;
    struct LoopStack* loopStack = 0;

    for(int op_index=0; op_index < op_count; op_index++){
        op_symbol = program[op_index];
        for(int handler_index=0; handler_index < HANDLERS_COUNT; handler_index++){
            if(op_symbol == handlers[handler_index].symbol){
                current = new_Operator(&handlers[handler_index]);
                if(handlers[handler_index].loop_type == LOOP_START){
                    pushLoop(&loopStack, current);
                }else if(handlers[handler_index].loop_type == LOOP_END){
                    struct Operator *loop_start = popLoop(&loopStack);
                    if(loop_start != 0){
                        loop_start->loop = current;
                        current->loop = loop_start;
                    }
                }
                if(head == 0){
                    head = current;
                    prev = current;
                }else{
                    prev->next = current;
                    prev = current;
                }
            }
        }
    }
    return head;
}

void pushLoop(struct LoopStack** stack, struct Operator* op){
    struct LoopStack* stackNode = (struct LoopStack*) malloc(sizeof(struct LoopStack));
    stackNode->next = *stack;
    stackNode->op = op;
    *stack = stackNode;
}

struct Operator* popLoop(struct LoopStack** stack){
    struct LoopStack* stackHead = 0;
    struct Operator* op = 0;
    if(*stack != 0){
        stackHead = *stack;
        op = stackHead->op;
        *stack = (*stack)->next;
        free(stackHead);
    }
    return op;
}

void stepProgramm(struct Machine* machine){
    if(machine->_current_stmt != 0){
        machine->_current_stmt = machine->_current_stmt->sth->handler(machine);
    }
}

// default operation handlers

struct Operator* hPlus(struct Machine* machine){
    machine->_tape[machine->_position]++;
    return machine->_current_stmt->next;
}

struct Operator* hMinus(struct Machine* machine){
    machine->_tape[machine->_position]--;
    return machine->_current_stmt->next;
}

struct Operator* hNext(struct Machine* machine){
    size_t new_position = machine->_position + 1;
    if(new_position >= machine->_size){
        machine->_position = 0;
    }else{
        machine->_position = new_position;
    }
    return machine->_current_stmt->next;
}

struct Operator* hPrev(struct Machine* machine){
    if(machine->_position == 0){
        machine->_position = machine->_size - 1;
    }else{
        machine->_position--;
    }
    return machine->_current_stmt->next;
}

struct Operator* hIn(struct Machine* machine){
    machine->_tape[machine->_position] = (unsigned char) getchar();
    return machine->_current_stmt->next;
}

struct Operator* hOut(struct Machine* machine){
    putchar(machine->_tape[machine->_position]);
    return machine->_current_stmt->next;
}

struct Operator* hLoopStart(struct Machine* machine){
    if(machine->_tape[machine->_position] > 0){
        return machine->_current_stmt->next;
    }else{
        return machine->_current_stmt->loop->next;
    }
}

struct Operator* hLoopEnd(struct Machine* machine){
    if(machine->_tape[machine->_position] > 0){
        return machine->_current_stmt->loop->next;
    }else{
        return machine->_current_stmt->next;
    }
}

