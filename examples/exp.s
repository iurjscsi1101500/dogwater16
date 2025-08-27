#define PRINT_INT 0x1

msg: dd 0x00000000
     dd 0x00000000
     dd 0x00000000

start:
    // 32-bit writes; constants are little-endian so memory bytes are h e l l | o   w o | r l d \n
    LI    R0, msg
    LI    R1, 0x6C6C6568      // "hell" for little-endian
    WRITEB R0, R1, 0
    LI    R1, 0x6F77206F      // "o wo"
    WRITEB R0, R1, 4
    LI    R1, 0x0A646C72      // "rld\n"
    WRITEB R0, R1, 8

    // print 4 32-bit
    LI    R0, msg
    LI    R2, 4
loop:
    READBR  R1, R0, R8
    MOVK   R1
    INT    PRINT_INT
    INC    R8
    DEC    R2
    JNZ    loop

    HLT

