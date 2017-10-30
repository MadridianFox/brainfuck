#include <stdlib.h>
#include "brainfuck.h"

int main(int argc, char* argv[]) {
    struct StatementHandler *sth_array = calloc(HANDLERS_COUNT, sizeof(struct StatementHandler));

    sth_array[0].handler = hPlus;
    sth_array[0].symbol = '+';
    sth_array[0].loop_type = NO_LOOP;

    sth_array[1].handler = hMinus;
    sth_array[1].symbol = '-';
    sth_array[1].loop_type = NO_LOOP;

    sth_array[2].handler = hNext;
    sth_array[2].symbol = '>';
    sth_array[2].loop_type = NO_LOOP;

    sth_array[3].handler = hPrev;
    sth_array[3].symbol = '<';
    sth_array[3].loop_type = NO_LOOP;

    sth_array[4].handler = hIn;
    sth_array[4].symbol = ',';
    sth_array[4].loop_type = NO_LOOP;

    sth_array[5].handler = hOut;
    sth_array[5].symbol = '.';
    sth_array[5].loop_type = NO_LOOP;

    sth_array[6].handler = hLoopStart;
    sth_array[6].symbol = '[';
    sth_array[6].loop_type = LOOP_START;

    sth_array[7].handler = hLoopEnd;
    sth_array[7].symbol = ']';
    sth_array[7].loop_type = LOOP_END;

    struct Machine* machine = new_Machine((size_t)30000);
    struct Operator* program = readProgram("++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.", sth_array);
    setProgram(machine,program);
    while(canDoStep(machine)){
        stepProgramm(machine);
    }

    delete_Machine(machine);
    delete_Operator(program);
    free(sth_array);

    return 0;
}