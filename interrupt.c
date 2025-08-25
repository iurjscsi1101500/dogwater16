#include "main.h"

inline void call_interrupt(struct CPU *cpu, const enum INTERRUPTS interrupt) {
    switch (interrupt) {
    case KEYBOARD_READ: {
        scanf("%hu", &cpu->regs[2]);
    } break;

    case KEYBOARD_WRITE: {
        printf("%c", (char) cpu->regs[2]);
    } break;

    case NMI: exit(0);

    case MEMORY_FAULT: {
        fprintf(stderr, "MEMORY FAULT AT %u", cpu->regs[2]);
        exit(-1);
    } break;

    case WAIT_MS: {
        usleep(cpu->regs[2] * 1000);
    } break;

    }
    return;
}
