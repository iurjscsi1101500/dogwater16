#include "main.h"

inline void nop(const struct CPU *cpu) {(void)cpu;}

inline void jz(struct CPU *cpu, const uint32_t target_pc) {
    if (cpu->flags.Zero) {cpu->pc = target_pc;}
}
inline void jnz(struct CPU *cpu, const uint32_t target_pc) {
    if (!cpu->flags.Zero) {cpu->pc = target_pc;}
}
inline void jmp(struct CPU *cpu, const uint32_t target_pc) {
    cpu->pc = target_pc;
}
inline void jn(struct CPU *cpu, const uint32_t target_pc) {
    if (cpu->flags.Negative) {cpu->pc = target_pc;}
}
inline void call(struct CPU *cpu, const uint32_t target_pc) {
    cpu->regs[LR] = cpu->pc;
    cpu->pc = target_pc;
}
inline void ret(struct CPU *cpu) {
    cpu->pc = cpu->regs[LR];
}
inline void blo(struct CPU *cpu, const uint32_t target_pc, const uint32_t rd, const uint32_t rs) {
    if (cpu->regs[rd] < cpu->regs[rs])
         cpu->pc = target_pc;
}
