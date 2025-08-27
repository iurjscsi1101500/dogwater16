
#ifndef DOGWATER_16_MAIN_H
#define DOGWATER_16_MAIN_H
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define ERR(x) do { fprintf(stderr, "%s", x); exit(-1); } while(0)
#define MAX 0xFFFFFFFFu
#define INSTR_LEN 4
#define LR 0x7
#define CHECK_MEM(x, cpu) ((x) >= (cpu)->mem_size)
#define OPC(w) (((w) >> 10) & 0x3F)
#define RD(w)  (((w) >> 6)  & 0x0F)
#define RS(w)  (((w) >> 2)  & 0x0F)
#define TMP 3
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
    OP_MOV, OP_MOVK, OP_RMOVK, OP_LI, OP_READ, OP_WRITE, OP_PUSH, OP_POP, OP_SWAP_R, OP_SWAP_M,
    OP_INT, OP_DIS_INT, OP_ENA_INT,
    OP_HLT, OP_LEA, OP_SLTU, OP_BLO, OP_READB, OP_READH, OP_WRITEB, OP_WRITEH, OP_READR, OP_WRITER, OP_READBR, OP_READHR, OP_WRITEBR, OP_WRITEHR, OP_LEAR
};

struct Flags {
    bool Zero, Negative;
};
struct CPU{
    uint32_t pc;
    uint32_t sp;
    uint32_t regs[16];   // reg[7] used for stack call/ret, reg[2] used by interrupts
    uint32_t keyboard_reg;
    struct Flags flags;
    uint32_t *mem;
    size_t mem_size;
    bool halted;
    bool interrupts_enabled;
};

void call_interrupt(struct CPU *cpu, const enum INTERRUPTS interrupt);
static inline void set_flags(struct Flags *flags, const uint32_t result) {
    flags->Zero = (result == 0);
    flags->Negative = (bool) ((result >> 31) & 0x1);
}
static inline uint32_t fetch(struct CPU *cpu) {
    if (CHECK_MEM(cpu->pc >> 2, cpu)) {
        cpu->regs[2] = cpu->pc;
        call_interrupt(cpu, MEMORY_FAULT);
    }
    cpu->pc += INSTR_LEN;
    return cpu->mem[(cpu->pc - INSTR_LEN) >> 2];
}
static inline void disable_interrupts(struct CPU *cpu) {
    cpu->interrupts_enabled = false;
}
static inline void enable_interrupts(struct CPU *cpu) {
    cpu->interrupts_enabled = true;
}
uint32_t add (const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t sub (const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t inc (const uint32_t a, struct Flags *flags);
uint32_t dec (const uint32_t a, struct Flags *flags);
uint32_t mul (const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t divi(const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t mod (const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t and (const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t nand(const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t nor (const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t or  (const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t xor (const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t not (const uint32_t a, struct Flags *flags);
uint32_t cmp (const uint32_t a, const uint32_t b, struct Flags *flags);
uint32_t shl (const uint32_t a, const uint32_t n, struct Flags *flags);
uint32_t shr (const uint32_t a, const uint32_t n, struct Flags *flags);
uint32_t sar (const uint32_t a, const uint32_t n, struct Flags *flags);
uint32_t sltu (const uint32_t a, const uint32_t b, struct Flags *flags);

void nop(const struct CPU *cpu);
void jmp(struct CPU *cpu, const uint32_t target_pc);
void jz(struct CPU *cpu, const uint32_t target_pc);
void jnz(struct CPU *cpu, const uint32_t target_pc);
void jn(struct CPU *cpu, const uint32_t target_pc);
void call(struct CPU *cpu, const uint32_t target_pc);
void ret(struct CPU *cpu);
void blo(struct CPU *cpu, const uint32_t target_pc, const uint32_t rd, const uint32_t rs);

void mov(struct CPU *cpu, const uint32_t r_dest, const uint32_t r_src);
void li(struct CPU *cpu, const uint32_t r_dest, const uint32_t r_val);
//do this otherwise gcc throws
uint32_t read_(struct CPU *cpu, const uint32_t reg ,const uint32_t m_src, const uint32_t offset);
uint32_t write_(struct CPU *cpu, const uint32_t mem ,const uint32_t r_src, const uint32_t offset);
uint32_t readb(struct CPU *cpu, const uint32_t r_dest, uint32_t base, const uint32_t off);
uint32_t readh(struct CPU *cpu, const uint32_t r_dest, uint32_t base, const uint32_t off);
void writeb(struct CPU *cpu, uint32_t base, const uint32_t r_src, uint32_t off);
void writeh(struct CPU *cpu, uint32_t base, const uint32_t r_src, uint32_t off);
uint32_t push(struct CPU *cpu, const uint32_t r_src);
uint32_t pop(struct CPU *cpu, const uint32_t r_dest);
void swap_r(struct CPU *cpu, const uint32_t r_dest, const uint32_t r_src);
void swap_m(struct CPU *cpu, const uint32_t m_dest, const uint32_t m_src);


#endif //DOGWATER_16_MAIN_H
