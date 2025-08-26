#guys this was written by chatgpt because i am too lazy to write an assmbler
import os, sys
OPC = {
 "NOP":0,"ADD":1,"ADDI":2,"SUB":3,"SUBI":4,"INC":5,"DEC":6,
 "MUL":7,"MULI":8,"DIV":9,"DIVI":10,"MOD":11,"MODI":12,
 "AND":13,"ANDI":14,"NAND":15,"NANDI":16,"NOR":17,"NORI":18,
 "OR":19,"ORI":20,"XOR":21,"XORI":22,"NOT":23,"CMP":24,"CMPI":25,
 "SHL":26,"SHLI":27,"SHR":28,"SHRI":29,"SAR":30,"SARI":31,
 "JMP":32,"JZ":33,"JNZ":34,"JN":35,"CALL":36,"RET":37,
 "MOV":38,"MOVK":39,"RMOVK":40,"LI":41,"READ":42,"WRITE":43,
 "PUSH":44,"POP":45,"SWAP_R":46,"SWAP_M":47,"INT":48,"DIS_INT":49,"ENA_INT":50,"HLT":51
}
SZ = {
 "NOP":1,"ADD":1,"SUB":1,"INC":1,"DEC":1,"MUL":1,"DIV":1,"MOD":1,
 "AND":1,"NAND":1,"NOR":1,"OR":1,"XOR":1,"NOT":1,"CMP":1,
 "SHL":1,"SHR":1,"SAR":1,"MOV":1,"MOVK":1,"RMOVK":1,"SWAP_R":1,"DIS_INT":1,"ENA_INT":1,"HLT":1,"RET":1,
 "ADDI":2,"SUBI":2,"MULI":2,"DIVI":2,"MODI":2,"ANDI":2,"NANDI":2,"NORI":2,"ORI":2,"XORI":2,
 "CMPI":2,"SHLI":2,"SHRI":2,"SARI":2,"JMP":2,"JZ":2,"JNZ":2,"JN":2,"CALL":2,
 "LI":2,"READ":2,"WRITE":2,"INT":2,"SWAP_M":2,"PUSH":2,"POP":2
}
def assemble(lines):
    import re, struct
    if isinstance(lines,str): lines=lines.splitlines()
    trim=lambda s:s.split("//",1)[0].strip()
    split_ops=lambda s:[t.strip() for t in s.split(",") if t.strip()]
    is_label=lambda s:re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*",s or "") is not None
    REG={f"R{i}":i for i in range(16)}
    LBL,DEF,prog={}, {},[]
    pc=0
    def parse_imm(tok):
        t=tok.strip()
        if t.startswith("#"): t=t[1:]
        if not t: return None
        try: return int(t,0)
        except: return None
    for ln,raw in enumerate(lines,1):
        s=trim(raw)
        if not s: continue
        if s.startswith("#define"):
            parts=s.split()
            if len(parts)!=3: raise ValueError(f"line {ln}: bad #define")
            name,val=parts[1].upper(),parts[2]
            vi=parse_imm(val)
            if vi is None: raise ValueError(f"line {ln}: bad #define value")
            DEF[name]=vi&0xFFFFFFFF
            continue
        while True:
            mlabel=re.match(r'^([A-Za-z_][A-Za-z0-9_]*)\s*:\s*(.*)$',s)
            if not mlabel: break
            name,rest=mlabel.group(1),mlabel.group(2)
            if not is_label(name): raise ValueError(f"line {ln}: bad label '{name}'")
            if name in LBL: raise ValueError(f"line {ln}: duplicate label '{name}'")
            LBL[name]=pc
            s=rest.strip()
            if not s: break
        if not s: continue
        m=re.match(r"^([A-Za-z_][A-Za-z0-9_]*)\s*(.*)$",s)
        if not m: raise ValueError(f"line {ln}: cannot parse '{s}'")
        mnem,rest=m.group(1).upper(),m.group(2).strip()
        ops=split_ops(rest) if rest else []
        if mnem=="DD": prog.append((pc,ln,"DD",ops)); pc+=4*len(ops); continue
        if mnem not in OPC: raise ValueError(f"line {ln}: unknown '{mnem}'")
        if mnem not in SZ: raise ValueError(f"line {ln}: no size rule for {mnem}")
        prog.append((pc,ln,mnem,ops)); pc+=SZ[mnem]*4
    def get_reg(tok,ln):
        t=tok.upper()
        if t not in REG: raise ValueError(f"line {ln}: bad reg '{tok}'"); 
        return REG[t]
    def val32(tok,ln):
        t=tok.strip(); U=t.upper()
        if U in DEF: return DEF[U]
        if is_label(t):
            if t not in LBL: raise ValueError(f"line {ln}: unknown label '{t}'")
            return LBL[t]&0xFFFFFFFF
        v=parse_imm(t)
        if v is None: raise ValueError(f"line {ln}: bad imm '{tok}'")
        return v&0xFFFFFFFF
    enc_hdr=lambda op,rd,rs:struct.pack("<I",(((op&0x3F)<<10)|((rd&0xF)<<6)|((rs&0xF)<<2))&0xFFFF)
    u32=lambda x:struct.pack("<I",x&0xFFFFFFFF)
    out=bytearray()
    for _,ln,M,ops in prog:
        if M=="DD":
            if not ops: raise ValueError(f"line {ln}: dd needs values")
            for t in ops: out+=u32(val32(t,ln))
            continue
        rd=rs=0;extra=None
        if M in {"NOP","RET","HLT","DIS_INT","ENA_INT"}:
            if ops: raise ValueError(f"line {ln}: {M} takes none")
        elif M in {"INC","DEC","NOT","POP","RMOVK"}:
            if len(ops)!=1: raise ValueError(f"line {ln}: {M} rd")
            rd=get_reg(ops[0],ln)
        elif M in {"PUSH","MOVK"}:
            if len(ops)!=1: raise ValueError(f"line {ln}: {M} rs")
            rs=get_reg(ops[0],ln)
        elif M in {"MOV","SWAP_R","SWAP_M","ADD","SUB","MUL","DIV","MOD","AND","NAND","NOR","OR","XOR","CMP","SHL","SHR","SAR"}:
            if len(ops)!=2: raise ValueError(f"line {ln}: {M} rd,rs")
            rd,rs=get_reg(ops[0],ln),get_reg(ops[1],ln)
        elif M in {"ADDI","SUBI","MULI","DIVI","MODI","ANDI","NANDI","NORI","ORI","XORI","CMPI","SHLI","SHRI","SARI","LI"}:
            if len(ops)!=2: raise ValueError(f"line {ln}: {M} rd,imm")
            rd=get_reg(ops[0],ln);extra=val32(ops[1],ln)
        elif M in {"READ","WRITE"}:
            if len(ops)!=3: raise ValueError(f"line {ln}: {M} rd,rs,imm")
            rd,rs=get_reg(ops[0],ln),get_reg(ops[1],ln);extra=val32(ops[2],ln)
        elif M in {"JMP","JZ","JNZ","JN","CALL","INT"}:
            if len(ops)!=1: raise ValueError(f"line {ln}: {M} imm")
            extra=val32(ops[0],ln)
        else: raise ValueError(f"line {ln}: unsupported {M}")
        out+=enc_hdr(OPC[M],rd,rs)
        if SZ[M]==2: out+=u32(extra if extra is not None else 0)
    return bytes(out)

if len(sys.argv) != 2:
    print(f"Usage: python3 {sys.argv[0]} filename")
    sys.exit(1)

name = sys.argv[1]
with open(name, "r") as f:
    bin_data = assemble(f.read())
    bin_name = os.path.splitext(name)[0] + ".bin"
    with open(bin_name, "wb") as f: f.write(bin_data)
