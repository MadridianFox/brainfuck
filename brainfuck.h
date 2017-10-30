//
// Created by madri on 25.10.2017.
//

#ifndef BRAINFUCK_BRAINFUCK_H
#define BRAINFUCK_BRAINFUCK_H

#define HANDLERS_COUNT 8

enum loop {NO_LOOP, LOOP_START, LOOP_END};

struct Machine {
    size_t _position;
    size_t _size;
    unsigned char* _tape;
    struct Operator* _program;
    struct Operator* _current_stmt;
};

struct Machine* new_Machine(size_t);
void delete_Machine(struct Machine *);
void clearState(struct Machine* machine);
void setProgram(struct Machine* machine, struct Operator* program);
int canDoStep(struct Machine *machine);

struct StatementHandler{
    struct Operator* (*handler)(struct Machine*);
    char symbol;
    enum loop loop_type;
};

/*
struct StatementHandler* new_StatementHandler(char, struct Operator* (*)(struct Machine*), enum loop);
void delete_StatementHandler(struct StatementHandler*);
*/
struct Operator{
    struct StatementHandler* sth;
    struct Operator* next;
    struct Operator* loop;
};

struct Operator* new_Operator(struct StatementHandler*);
void delete_Operator(struct Operator*);


struct Operator* readProgram(char* program, struct StatementHandler* handlers);

struct LoopStack{
    struct Operator* op;
    struct LoopStack* next;
};

void pushLoop(struct LoopStack** stack, struct Operator* op);
struct Operator* popLoop(struct LoopStack** stack);

void stepProgramm(struct Machine* machine);

// default operation handlers

struct Operator* hPlus(struct Machine* machine);
struct Operator* hMinus(struct Machine* machine);
struct Operator* hNext(struct Machine* machine);
struct Operator* hPrev(struct Machine* machine);
struct Operator* hIn(struct Machine* machine);
struct Operator* hOut(struct Machine* machine);
struct Operator* hLoopStart(struct Machine* machine);
struct Operator* hLoopEnd(struct Machine* machine);

#endif //BRAINFUCK_BRAINFUCK_H
