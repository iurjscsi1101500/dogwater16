#include "main.h"

inline void nop(const struct CPU *cpu) {(void)cpu;}

inline void jz(struct CPU *cpu, const uint16_t target_pc) {
    if (cpu->flags.Zero) {cpu->pc = target_pc;}
}
inline void jnz(struct CPU *cpu, const uint16_t target_pc) {
    if (!cpu->flags.Zero) {cpu->pc = target_pc;}
}
inline void jmp(struct CPU *cpu, const uint16_t target_pc) {
    cpu->pc = target_pc;
}
inline void jn(struct CPU *cpu, const uint16_t target_pc) {
    if (cpu->flags.Negative) {cpu->pc = target_pc;}
}
inline void call(struct CPU *cpu, const uint16_t target_pc) {
    cpu->regs[LR] = (cpu->pc + INSTR_LEN);
    cpu->pc = target_pc;
}
inline void ret(struct CPU *cpu) {
    cpu->pc = cpu->regs[LR];
}