
#ifndef DOGWATER_16_MAIN_H
#define DOGWATER_16_MAIN_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define ERR(fmt, ...) fprintf(stderr, fmt, __VA_ARGS__); exit(-1);

#define MAX 0xFFFF
#define INSTR_LEN 2

enum INTERRUPTS {
    KEYBOARD_READ,
    KEYBOARD_WRITE,
    NMI, //this will shut down cpu
    MEMORY_FAULT,
    WAIT_MS
};
struct Flags {
    bool Zero, Negative;
};
struct CPU{
    uint16_t pc;
    uint16_t sp;
    uint16_t regs[16];   // reg[7] used for stack call/ret, reg[2] used by interrupts
    struct Flags flags;
    uint16_t *mem;
    size_t mem_size;
    bool halted;
};

static inline void set_flags(struct Flags *flags, const uint16_t result) {
    flags->Zero = !result;
    flags->Negative = (bool) (result >> 15) & 0x1;
}
static inline uint16_t fetch(const struct CPU *cpu, const uint16_t addr) {
    return cpu->mem[(addr >> 1) % cpu->mem_size];;
}
uint16_t add (const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t sub (const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t inc (const uint16_t a, struct Flags *flags);
uint16_t dec (const uint16_t a, struct Flags *flags);
uint16_t mul (const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t divi(const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t mod (const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t and (const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t nand(const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t nor (const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t or  (const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t xor (const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t not (const uint16_t a, struct Flags *flags);
uint16_t cmp (const uint16_t a, const uint16_t b, struct Flags *flags);
uint16_t shl (const uint16_t a, const uint16_t n, struct Flags *flags);
uint16_t sar (const uint16_t a, const uint16_t n, struct Flags *flags);

void nop(const struct CPU *cpu);
void jz(struct CPU *cpu, const uint16_t target_pc);
void jnz(struct CPU *cpu, const uint16_t target_pc);
void jn(struct CPU *cpu, const uint16_t target_pc);
void call(struct CPU *cpu, const uint16_t target_pc);
void ret(struct CPU *cpu);

void mov(struct CPU *cpu, const uint16_t r_dest, const uint16_t r_src);
void li(struct CPU *cpu, const uint16_t r_dest, const uint16_t r_val);
uint16_t read(struct CPU *cpu, const uint16_t reg ,const uint16_t m_src, const uint16_t offset);
uint16_t write(const struct CPU *cpu, const uint16_t mem ,const uint16_t r_src, const uint16_t offset);
uint16_t push(struct CPU *cpu, const uint16_t r_src);
uint16_t pop(struct CPU *cpu, const uint16_t r_dest);
void swap_r(struct CPU *cpu, const uint16_t r_dest, const uint16_t r_src);
void swap_m(struct CPU *cpu, const uint16_t m_dest, const uint16_t m_src);

void call_interrupt(struct CPU *cpu, const enum INTERRUPTS interrupt);
#define CHECK_MEM(x, cpu) ((x) >= (cpu)->mem_size)


#endif //DOGWATER_16_MAIN_H