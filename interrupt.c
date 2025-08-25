#include "main.h"

inline void call_interrupt(struct CPU *cpu, const enum INTERRUPTS interrupt) {
    switch (interrupt) {
    case KEYBOARD_READ: {
        scanf("%c", &cpu->regs[2]);
    } break;

    case KEYBOARD_WRITE: {
        printf("%c", (char) cpu->regs[2]);
    } break;

    case NMI: exit(0);

    case MEMORY_FAULT: {
        ERR("MEMORY FAULT AT %d", cpu->regs[2])
    } break;

    case WAIT_MS: {
        usleep(cpu->regs[2] * 1000);
    } break;

    }
    return;
}