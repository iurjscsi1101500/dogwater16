#include "main.h"

inline void call_interrupt(struct CPU *cpu, const enum INTERRUPTS interrupt) {
    if (!cpu->interrupts_enabled) {
        ERR("Tried to use interrupt when interrupt was disabled\n");
    }
    switch (interrupt) {
    case KEYBOARD_READ: {
        scanf("%u", &cpu->keyboard_reg);
    } break;

    case KEYBOARD_WRITE: {
        printf("%c%c%c%c", (char)(cpu->keyboard_reg & 0xFF), (char)(cpu->keyboard_reg >> 8), (char)(cpu->keyboard_reg >> 16), (char)(cpu->keyboard_reg >> 24));
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
