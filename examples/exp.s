#define PRINT_INT 0x1
#define CH_X      0x58
#define CH_NL     0x0A

// this is a comment
//this is a variable
//name          val (needed)
ch_from_mem: dd 0x2E

start:
    LI    R0, ch_from_mem
    LI    R1, CH_X
    WRITE R0, R1, 0

    CALL  print_var

    LI    R0, CH_NL
    MOVK  R0
    INT   PRINT_INT
    HLT

print_var:
    LI    R0, ch_from_mem
    READ  R1, R0, 0
    MOVK  R1
    INT   PRINT_INT
    RET

