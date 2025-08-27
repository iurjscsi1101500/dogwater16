#include "main.h"

inline void mov(struct CPU *cpu, const uint32_t r_dest, const uint32_t r_src) {
    cpu->regs[r_dest] = cpu->regs[r_src];
    set_flags(&cpu->flags, cpu->regs[r_dest]);
}

inline void li(struct CPU *cpu, const uint32_t r_dest, const uint32_t r_val) {
    cpu->regs[r_dest] = r_val;
    set_flags(&cpu->flags, cpu->regs[r_dest]);
}
inline uint32_t read_(struct CPU *cpu, const uint32_t reg ,const uint32_t m_src, const uint32_t offset) {
    if (CHECK_MEM(m_src + offset, cpu)) {
        cpu->regs[2] = m_src + offset;
        call_interrupt(cpu, MEMORY_FAULT);
    }
    cpu->regs[reg] = cpu->mem[m_src + offset];
    set_flags(&cpu->flags, cpu->regs[reg]);
    return cpu->regs[reg];
}
inline uint32_t write_(struct CPU *cpu, const uint32_t mem ,const uint32_t r_src, const uint32_t offset) {
    if (CHECK_MEM(mem + offset, cpu)) {
        cpu->regs[2] = mem + offset;
        call_interrupt(cpu, MEMORY_FAULT);
    }
    cpu->mem[mem + offset] = cpu->regs[r_src];
    return cpu->mem[mem + offset];
}
inline uint32_t readb(struct CPU *cpu, const uint32_t r_dest, uint32_t base, const uint32_t off) {
    uint32_t v = (read_(cpu, TMP, base, off) & 0xFFu) | ((read_(cpu, TMP, base, off + 1u) & 0xFFu) << 8) | ((read_(cpu, TMP, base, off + 2u) & 0xFFu) << 16) | ((read_(cpu, TMP, base, off + 3u) & 0xFFu) << 24);
    cpu->regs[r_dest] = v; set_flags(&cpu->flags, v); return v;
}

inline void writeb(struct CPU *cpu, uint32_t base, const uint32_t r_src, uint32_t off) {
    uint32_t v = cpu->regs[r_src];
    cpu->regs[TMP] = v & 0xFFu; (void)write_(cpu, base, TMP, off);
    cpu->regs[TMP] = (v >> 8) & 0xFFu; (void)write_(cpu, base, TMP, off + 1u);
    cpu->regs[TMP] = (v >> 16) & 0xFFu; (void)write_(cpu, base, TMP, off + 2u);
    cpu->regs[TMP] = (v >> 24) & 0xFFu; (void)write_(cpu, base, TMP, off + 3u);
}

inline uint32_t readh(struct CPU *cpu, const uint32_t r_dest, uint32_t base, const uint32_t off) {
    uint32_t v = (read_(cpu, TMP, base, off) & 0xFFu) | ((read_(cpu, TMP, base, off + 1u) & 0xFFu) << 8);
    cpu->regs[r_dest] = v; set_flags(&cpu->flags, v); return v;
}

inline void writeh(struct CPU *cpu, uint32_t base, const uint32_t r_src, uint32_t off) {
    uint32_t v = cpu->regs[r_src];
    cpu->regs[TMP] = v & 0xFFu; (void)write_(cpu, base, TMP, off);
    cpu->regs[TMP] = (v >> 8) & 0xFFu; (void)write_(cpu, base, TMP, off + 1u);
}
inline uint32_t push(struct CPU *cpu, const uint32_t r_src) {
    cpu->mem[--cpu->sp] = cpu->regs[r_src & LR];
    return cpu->mem[cpu->sp];
}
inline uint32_t pop(struct CPU *cpu, const uint32_t r_dest) {
    cpu->regs[r_dest & LR] = cpu->mem[cpu->sp++];
    return cpu->regs[r_dest & LR];
}
inline void swap_r(struct CPU *cpu, const uint32_t r_dest, const uint32_t r_src) {
    const uint32_t temp = cpu->regs[r_src];
    cpu->regs[r_src] = cpu->regs[r_dest];
    cpu->regs[r_dest] = temp;
}
inline void swap_m(struct CPU *cpu, const uint32_t m_dest, const uint32_t m_src) {
    if (CHECK_MEM(m_src, cpu) || CHECK_MEM(m_dest, cpu)) {
        cpu->regs[2] = m_src;
        call_interrupt(cpu, MEMORY_FAULT);
    }
    const uint32_t temp = cpu->mem[m_src];
    cpu->mem[m_src] = cpu->mem[m_dest];
    cpu->mem[m_dest] = temp;
}
