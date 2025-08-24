#include "main.h"

inline void mov(struct CPU *cpu, const uint16_t r_dest, const uint16_t r_src) {
    cpu->regs[r_dest] = cpu->regs[r_src];
    set_flags(&cpu->flags, cpu->regs[r_dest]);
}

inline void li(struct CPU *cpu, const uint16_t r_dest, const uint16_t r_val) {
    cpu->regs[r_dest] = r_val;
    set_flags(&cpu->flags, cpu->regs[r_dest]);
}
inline uint16_t read(struct CPU *cpu,const uint16_t reg ,const uint16_t m_src, const uint16_t offset) {
    cpu->regs[reg] = cpu->mem[m_src + offset];
    set_flags(&cpu->flags, cpu->regs[reg]);
    return cpu->regs[reg];
}
inline uint16_t write(const struct CPU *cpu,const uint16_t mem ,const uint16_t r_src, const uint16_t offset) {
    cpu->mem[mem + offset] = cpu->regs[r_src];
    return cpu->mem[mem + offset];
}
inline uint16_t push(struct CPU *cpu, const uint16_t r_src) {
    cpu->sp--;
    cpu->mem[cpu->sp] = cpu->regs[r_src & LR];
    return cpu->mem[cpu->sp];
}
inline uint16_t pop(struct CPU *cpu, const uint16_t r_dest) {
    cpu->regs[r_dest & LR] = cpu->mem[cpu->sp];
    cpu->sp--;
    return cpu->regs[r_dest & LR];
}