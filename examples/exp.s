#define PRINT_INT 0x1

msg: dd 0x00000000
     dd 0x00000000
     dd 0x00000000

start:
    // 32-bit writes (little-endian words)
    LI    R0, msg
    LI    R1, 0x6C6C6568      // "hell"
    WRITEB R0, R1, 0
    LI    R1, 0x6F77206F      // "o wo"
    WRITEB R0, R1, 4
    LI    R1, 0x0A646C72      // "rld\n"
    WRITEB R0, R1, 8

    // word 0
    LI    R0, msg
    READB  R1, R0, 0
    READB  R2, R0, 1
    SHLI   R2, 8
    OR     R1, R2
    READB  R2, R0, 2
    SHLI   R2, 16
    OR     R1, R2
    READB  R2, R0, 3
    SHLI   R2, 24
    OR     R1, R2
    MOVK   R1
    INT    PRINT_INT

    // word 1
    READB  R1, R0, 4
    READB  R2, R0, 5
    SHLI   R2, 8
    OR     R1, R2
    READB  R2, R0, 6
    SHLI   R2, 16
    OR     R1, R2
    READB  R2, R0, 7
    SHLI   R2, 24
    OR     R1, R2
    MOVK   R1
    INT    PRINT_INT

    // word 2
    READB  R1, R0, 8
    READB  R2, R0, 9
    SHLI   R2, 8
    OR     R1, R2
    READB  R2, R0, 10
    SHLI   R2, 16
    OR     R1, R2
    READB  R2, R0, 11
    SHLI   R2, 24
    OR     R1, R2
    MOVK   R1
    INT    PRINT_INT

    HLT

