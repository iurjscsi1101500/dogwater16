#include "main.h"

inline void call_interrupt(struct CPU *cpu, const enum INTERRUPTS interrupt) {
    if (!cpu->interrupts_enabled) {
        ERR("Tried to use interrupt when interrupt was disabled\n");
    }
    switch (interrupt) {
    case KEYBOARD_READ: {
        scanf("%hu", &cpu->keyboard_reg);
    } break;

    case KEYBOARD_WRITE: {
        printf("%c", (char) cpu->keyboard_reg);
    } break;

    case NMI: exit(0);

    case MEMORY_FAULT: {
        fprintf(stderr, "MEMORY FAULT AT %u\n", cpu->regs[2]);
        exit(-1);
    } break;

    case WAIT_MS: {
        usleep(cpu->regs[2] * 1000);
    } break;

    default: ERR("UNKNOWN INTERRUPT\n");
    }
    return;
}
