#include "main.h"

void cpu_init(struct CPU *cpu, const uint32_t mem_size) {
    cpu->mem_size = mem_size;
    cpu->mem = calloc(cpu->mem_size, sizeof(uint32_t));
    if (!cpu->mem) ERR("mem init failed\n");
    cpu->pc = 0;
    cpu->sp = cpu->mem_size - 1;
    cpu->regs[7] = cpu->sp;
    cpu->halted = false;
    cpu->flags = (struct Flags){0};
    enable_interrupts(cpu);
}
void open_file(const char* file_name, struct CPU *cpu) {
    FILE *f = fopen(file_name, "rb");
    if (!f) ERR("cant open file\n");
    for (size_t i = 0; i < cpu->mem_size; ++i) {
        unsigned char b[4];
        size_t n = fread(b, 1, 4, f);
        if (n == 0) break;
        if (n != 4) ERR("bad program size (not multiple of 4)\n");
        cpu->mem[i] = (uint32_t)b[0] | ((uint32_t)b[1] << 8) | ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24);
    }
    fclose(f);
}

int cpu_step(struct CPU *cpu){
    if(cpu->halted) return 0;

    const uint32_t insn=fetch(cpu);
    const uint32_t op=OPC(insn);
    const uint32_t rd=RD(insn);
    const uint32_t rs=RS(insn);

    switch(op){
        case OP_NOP: nop(cpu); return 1;
        case OP_ADD: cpu->regs[rd]=add(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_SUB: cpu->regs[rd]=sub(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_INC: cpu->regs[rd]=inc(cpu->regs[rd],&cpu->flags); return 1;
        case OP_DEC: cpu->regs[rd]=dec(cpu->regs[rd],&cpu->flags); return 1;
        case OP_MUL: cpu->regs[rd]=mul(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_DIV: cpu->regs[rd]=divi(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_MOD: cpu->regs[rd]=mod(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_AND: cpu->regs[rd]=and(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_NAND: cpu->regs[rd]=nand(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_NOR: cpu->regs[rd]=nor(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_OR: cpu->regs[rd]=or(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_XOR: cpu->regs[rd]=xor(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_NOT: cpu->regs[rd]=not(cpu->regs[rd],&cpu->flags); return 1;
        case OP_CMP: (void)cmp(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_SHL: cpu->regs[rd]=shl(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_SHR: cpu->regs[rd]=shr(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_SAR: cpu->regs[rd]=sar(cpu->regs[rd],cpu->regs[rs],&cpu->flags); return 1;
        case OP_ADDI: cpu->regs[rd]=add(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_SUBI: cpu->regs[rd]=sub(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_MULI: cpu->regs[rd]=mul(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_DIVI: cpu->regs[rd]=divi(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_MODI: cpu->regs[rd]=mod(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_ANDI: cpu->regs[rd]=and(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_NANDI: cpu->regs[rd]=nand(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_NORI: cpu->regs[rd]=nor(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_ORI: cpu->regs[rd]=or(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_XORI: cpu->regs[rd]=xor(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_CMPI: (void)cmp(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_SHLI: cpu->regs[rd]=shl(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_SHRI: cpu->regs[rd]=shr(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_SARI: cpu->regs[rd]=sar(cpu->regs[rd],fetch(cpu),&cpu->flags); return 2;
        case OP_JMP: jmp(cpu,fetch(cpu)); return 2;
        case OP_JZ: jz(cpu,fetch(cpu)); return 2;
        case OP_JNZ: jnz(cpu,fetch(cpu)); return 2;
        case OP_JN: jn(cpu,fetch(cpu)); return 2;
        case OP_CALL: call(cpu,fetch(cpu)); return 3;
        case OP_RET: ret(cpu); return 3;
        case OP_MOV: mov(cpu,rd,rs); return 1;
        case OP_LI: li(cpu,rd,fetch(cpu)); return 2;
        case OP_READ: (void)read_(cpu,rd,rs,fetch(cpu)); return 2;
        case OP_WRITE: (void)write_(cpu,rd,rs,fetch(cpu)); return 2;
        case OP_PUSH: (void)push(cpu,rs); return 2;
        case OP_POP: (void)pop(cpu,rd); return 2;
        case OP_SWAP_R: swap_r(cpu,rd,rs); return 1;
        case OP_SWAP_M: swap_m(cpu,rd,rs); return 2;
        case OP_INT: call_interrupt(cpu,(enum INTERRUPTS)fetch(cpu)); return 2;
        case OP_DIS_INT: disable_interrupts(cpu); return 1;
        case OP_ENA_INT: enable_interrupts(cpu); return 1;
        case OP_HLT: cpu->halted = true; return 0;
        default: ERR("ILLEGAL OPCODE\n");
    }
}

int main(int argc, char **argv) {
    //still not complete because no assembler
    struct CPU cpu = {0};
    if (argc < 2) { fprintf(stderr,"usage: %s program.bin\n",argv[0]); return -1; }
    cpu_init(&cpu, 0x6969);
    open_file(argv[1], &cpu);
    while (!cpu.halted) cpu_step(&cpu);
    free(cpu.mem);
    return 0;
    return 0;
}
