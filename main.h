
#ifndef DOGWATER_16_MAIN_H
#define DOGWATER_16_MAIN_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define ERR(x) do { fprintf(stderr, "%s", x); exit(-1); } while(0)
#define MAX 0xFFFF
#define INSTR_LEN 2
#define LR 0x7
#define CHECK_MEM(x, cpu) ((x) >= (cpu)->mem_size)
#define OPC(w) (((w) >> 10) & 0x3F)
#define RD(w)  (((w) >> 6)  & 0x0F)
#define RS(w)  (((w) >> 2)  & 0x0F)

enum INTERRUPTS {
    KEYBOARD_READ,
    KEYBOARD_WRITE,
    NMI, //this will shut down cpu
    MEMORY_FAULT,
    WAIT_MS
};
enum OPCODE {
    OP_NOP,
    OP_ADD, OP_ADDI, OP_SUB, OP_SUBI, OP_INC, OP_DEC,
    OP_MUL, OP_MULI, OP_DIV, OP_DIVI, OP_MOD, OP_MODI,
    OP_AND, OP_ANDI, OP_NAND, OP_NANDI, OP_NOR, OP_NORI, OP_OR, OP_ORI, OP_XOR, OP_XORI,
    OP_NOT, OP_CMP, OP_CMPI,
    OP_SHL, OP_SHLI, OP_SHR, OP_SHRI, OP_SAR, OP_SARI,
    OP_JMP, OP_JZ, OP_JNZ, OP_JN, OP_CALL, OP_RET,
    OP_MOV, OP_LI, OP_READ, OP_WRITE, OP_PUSH, OP_POP, OP_SWAP_R, OP_SWAP_M,
    OP_INT, OP_DIS_INT, OP_ENA_INT,
    OP_HLT
};

struct Flags {
    bool Zero, Negative;
};
struct CPU{
    uint16_t pc;
    uint16_t sp;
    uint16_t regs[16];   // reg[7] used for stack call/ret, reg[2] used by interrupts
    uint16_t keyboard_reg;
    struct Flags flags;
    uint16_t *mem;
    size_t mem_size;
    bool halted;
    bool interrupts_enabled;
};

void call_interrupt(struct CPU *cpu, const enum INTERRUPTS interrupt);
static inline void set_flags(struct Flags *flags, const uint16_t result) {
    flags->Zero = !result;
    flags->Negative = (bool) (result >> 15) & 0x1;
}
static inline uint16_t fetch(struct CPU *cpu) {
    if (CHECK_MEM(cpu->pc >> 1, cpu)) {
        cpu->regs[2]= cpu->pc;
        call_interrupt(cpu, MEMORY_FAULT);
    }
    cpu->pc += INSTR_LEN;
    return cpu->mem[(cpu->pc - INSTR_LEN) >> 1];
}
static inline void disable_interrupts(struct CPU *cpu) {
    cpu->interrupts_enabled = false;
}
static inline void enable_interrupts(struct CPU *cpu) {
    cpu->interrupts_enabled = true;
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
uint16_t shr (const uint16_t a, const uint16_t n, struct Flags *flags);
uint16_t sar (const uint16_t a, const uint16_t n, struct Flags *flags);

void nop(const struct CPU *cpu);
void jmp(struct CPU *cpu, const uint16_t target_pc);
void jz(struct CPU *cpu, const uint16_t target_pc);
void jnz(struct CPU *cpu, const uint16_t target_pc);
void jn(struct CPU *cpu, const uint16_t target_pc);
void call(struct CPU *cpu, const uint16_t target_pc);
void ret(struct CPU *cpu);

void mov(struct CPU *cpu, const uint16_t r_dest, const uint16_t r_src);
void li(struct CPU *cpu, const uint16_t r_dest, const uint16_t r_val);
//do this otherwise gcc throws
uint16_t read_(struct CPU *cpu, const uint16_t reg ,const uint16_t m_src, const uint16_t offset);
uint16_t write_(struct CPU *cpu, const uint16_t mem ,const uint16_t r_src, const uint16_t offset);
uint16_t push(struct CPU *cpu, const uint16_t r_src);
uint16_t pop(struct CPU *cpu, const uint16_t r_dest);
void swap_r(struct CPU *cpu, const uint16_t r_dest, const uint16_t r_src);
void swap_m(struct CPU *cpu, const uint16_t m_dest, const uint16_t m_src);


#endif //DOGWATER_16_MAIN_H
