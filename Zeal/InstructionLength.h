/* (C) Copyright 2012-2014 Semantic Designs, Inc.
   You may freely use this code provided you retain this copyright message
*/

#include <Windows.h>
namespace Zeal
{
    typedef unsigned int natural;

    static natural InstructionLength(BYTE* pc)
    { // returns length of instruction at PC
        natural length = 0;
        natural opcode, opcode2;
        natural modrm;
        natural sib;
        BYTE* p = pc;

        while (true)
        {  // scan across prefix bytes
            opcode = *p++;
            switch (opcode)
            {
            case 0x64: case 0x65: // FS: GS: prefixes
            case 0x36: // SS: prefix
            case 0x66: case 0x67: // operand size overrides
            case 0xF0: case 0xF2: // LOCK, REPNE prefixes
                length++;
                break;
            case 0x2E: // CS: prefix, used as HNT prefix on jumps
            case 0x3E: // DS: prefix, used as HT prefix on jumps
                length++;
                // goto process relative jmp // tighter check possible here
                break;
            default:
                goto process_instruction_body;
            }
        }

    process_instruction_body:
        switch (opcode) // switch on main opcode
        {
            // ONE BYTE OPCODE, move to next opcode without remark
        case 0x27: case 0x2F:
        case 0x37: case 0x3F:
        case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
        case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
        case 0x50: case 0x51: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57:
        case 0x58: case 0x59: case 0x5A: case 0x5B: case 0x5C: case 0x5D: case 0x5E: case 0x5F:
        case 0x90: // nop
        case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97: // xchg
        case 0x98: case 0x99:
        case 0x9C: case 0x9D: case 0x9E: case 0x9F:
        case 0xA4: case 0xA5: case 0xA6: case 0xA7: case 0xAA: case 0xAB: // string operators
        case 0xAC: case 0xAD: case 0xAE: case 0xAF:
            /* case 0xC3: // RET handled elsewhere */
        case 0xC9:
        case 0xCC: // int3
        case 0xF5: case 0xF8: case 0xF9: case 0xFC: case 0xFD:
            return length + 1; // include opcode

        case 0xC3: // RET
            if (*p++ != 0xCC)
                return length + 1;
            if (*p++ != 0xCC)
                return length + 2;
            if (*p++ == 0xCC
                && *p++ == 0xCC)
                return length + 5;
            goto error;

            // TWO BYTE INSTRUCTION
        case 0x04: case 0x0C: case 0x14: case 0x1C: case 0x24: case 0x2C: case 0x34: case 0x3C:
        case 0x6A:
        case 0xB0: case 0xB1: case 0xB2: case 0xB3: case 0xB4: case 0xB5: case 0xB6: case 0xB7:
        case 0xC2:
            return length + 2;

            // TWO BYTE RELATIVE BRANCH
        case 0x70: case 0x71: case 0x72: case 0x73: case 0x74: case 0x75: case 0x76: case 0x77:
        case 0x78: case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D: case 0x7E: case 0x7F:
        case 0xE0: case 0xE1: case 0xE2: case 0xE3: case 0xEB:
            return length + 2;

            // THREE BYTE INSTRUCTION (NONE!)

        // FIVE BYTE INSTRUCTION:
        case 0x05: case 0x0D: case 0x15: case 0x1D:
        case 0x25: case 0x2D: case 0x35: case 0x3D:
        case 0x68:
        case 0xA9:
        case 0xB8: case 0xB9: case 0xBA: case 0xBB: case 0xBC: case 0xBD: case 0xBE: case 0xBF:
            return length + 5;

            // FIVE BYTE RELATIVE CALL
        case 0xE8:
            return length + 5;

            // FIVE BYTE RELATIVE BRANCH
        case 0xE9:
            if (p[4] == 0xCC)
                return length + 6; // <jmp near ptr ...  int 3>
            return length + 5; // plain <jmp near ptr>

            // FIVE BYTE DIRECT ADDRESS
        case 0xA1: case 0xA2: case 0xA3: // MOV AL,AX,EAX moffset...
            return length + 5;
            break;

            // ModR/M with no immediate operand
        case 0x00: case 0x01: case 0x02: case 0x03: case 0x08: case 0x09: case 0x0A: case 0x0B:
        case 0x10: case 0x11: case 0x12: case 0x13: case 0x18: case 0x19: case 0x1A: case 0x1B:
        case 0x20: case 0x21: case 0x22: case 0x23: case 0x28: case 0x29: case 0x2A: case 0x2B:
        case 0x30: case 0x31: case 0x32: case 0x33: case 0x38: case 0x39: case 0x3A: case 0x3B:
        case 0x84: case 0x85: case 0x86: case 0x87: case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8D: case 0x8F:
        case 0xD1: case 0xD2: case 0xD3:
        case 0xFE: case 0xFF: // misinterprets JMP far and CALL far, not worth fixing
            length++; // count opcode
            goto modrm;

            // ModR/M with immediate 8 bit value
        case 0x80: case 0x82: case 0x83:
        case 0xC0: case 0xC1:
        case 0xC6:  // with r=0?
            length += 2; // count opcode and immediate byte
            goto modrm;

            // ModR/M with immediate 32 bit value
        case 0x81:
        case 0xC7:  // with r=0?
            length += 5; // count opcode and immediate byte
            goto modrm;

        case 0x9B: // FSTSW AX = 9B DF E0
            if (*p++ == 0xDF)
            {
                if (*p++ == 0xE0)
                    return length + 3;
                goto error;
            }
            else {
                goto error;
            }

        case 0xD9: // various FP instructions
            modrm = *p++;
            length++; //  account for FP prefix
            switch (modrm)
            {
            case 0xC9: case 0xD0:
            case 0xE0: case 0xE1: case 0xE4: case 0xE5:
            case 0xE8: case 0xE9: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEE:
            case 0xF8: case 0xF9: case 0xFA: case 0xFB: case 0xFC: case 0xFD: case 0xFE: case 0xFF:
                return length + 1;
            default:  // r bits matter if not one of the above specific opcodes
                switch ((modrm & 0x38) >> 3)
                {
                case 0: goto modrm_fetched;  // fld
                case 1: return length + 1; // fxch
                case 2: goto modrm_fetched; // fst
                case 3: goto modrm_fetched; // fstp
                case 4: goto modrm_fetched; // fldenv
                case 5: goto modrm_fetched; // fldcw
                case 6: goto modrm_fetched; // fnstenv
                case 7: goto modrm_fetched; // fnstcw
                }
                goto error; // unrecognized 2nd byte
            }

        case 0xDB: // various FP instructions
            modrm = *p++;
            length++; //  account for FP prefix
            switch (modrm)
            {
            case 0xE3:
                return length + 1;
            default:  // r bits matter if not one of the above specific opcodes
#if 0
                switch ((modrm & 0x38) >> 3)
                {
                case 0: goto modrm_fetched;  // fld
                case 1: return length + 1; // fxch
                case 2: goto modrm_fetched; // fst
                case 3: goto modrm_fetched; // fstp
                case 4: goto modrm_fetched; // fldenv
                case 5: goto modrm_fetched; // fldcw
                case 6: goto modrm_fetched; // fnstenv
                case 7: goto modrm_fetched; // fnstcw
                }
#endif
                goto error; // unrecognized 2nd byte
            }

        case 0xDD: // various FP instructions
            modrm = *p++;
            length++; //  account for FP prefix
            switch (modrm)
            {
            case 0xE1: case 0xE9:
                return length + 1;
            default:  // r bits matter if not one of the above specific opcodes
                switch ((modrm & 0x38) >> 3)
                {
                case 0: goto modrm_fetched;  // fld
                    // case 1: return length+1; // fisttp
                case 2: goto modrm_fetched; // fst
                case 3: goto modrm_fetched; // fstp
                case 4: return length + 1; // frstor
                case 5: return length + 1; // fucomp
                case 6: goto modrm_fetched; // fnsav
                case 7: goto modrm_fetched; // fnstsw
                }
                goto error; // unrecognized 2nd byte
            }

        case 0xF3: // funny prefix REPE
            opcode2 = *p++;  // get second opcode byte
            switch (opcode2)
            {
            case 0x90: // == PAUSE
            case 0xA4: case 0xA5: case 0xA6: case 0xA7: case 0xAA: case 0xAB: // string operators
                return length + 2;
            case 0xC3: // (REP) RET
                if (*p++ != 0xCC)
                    return length + 2; // only (REP) RET
                if (*p++ != 0xCC)
                    goto error;
                if (*p++ == 0xCC)
                    return length + 5; // (REP) RET CLONE IS LONG JUMP RELATIVE
                goto error;
            case 0x66: // operand size override (32->16 bits)
                if (*p++ == 0xA5) // "rep movsw"
                    return length + 3;
                goto error;
            default: goto error;
            }

        case 0xF6: // funny subblock of opcodes
            modrm = *p++;
            if ((modrm & 0x20) == 0)
                length++; // 8 bit immediate operand
            goto modrm_fetched;

        case 0xF7: // funny subblock of opcodes
            modrm = *p++;
            if ((modrm & 0x30) == 0)
                length += 4; // 32 bit immediate operand
            goto modrm_fetched;

            // Intel's special prefix opcode
        case 0x0F:
            length += 2; // add one for special prefix, and one for following opcode
            opcode2 = *p++;
            switch (opcode2)
            {
            case 0x31: // RDTSC
                return length;

                // CMOVxx
            case 0x40: case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: case 0x47:
            case 0x48: case 0x49: case 0x4A: case 0x4B: case 0x4C: case 0x4D: case 0x4E: case 0x4F:
                goto modrm;

                // JC relative 32 bits
            case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87:
            case 0x88: case 0x89: case 0x8A: case 0x8B: case 0x8C: case 0x8D: case 0x8E: case 0x8F:
                return length + 4; // account for subopcode and displacement

                // SETxx rm32
            case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
            case 0x98: case 0x99: case 0x9A: case 0x9B: case 0x9C: case 0x9D: case 0x9E: case 0x9F:
                goto modrm;

            case 0xA2: // CPUID
                return length + 2;

            case 0xAE: // LFENCE, SFENCE, MFENCE
                opcode2 = *p++;
                switch (opcode2)
                {
                case 0xE8: // LFENCE
                case 0xF0: // MFENCE
                case 0xF8: // SFENCE
                    return length + 1;
                default:
                    goto error;
                }

            case 0xAF: // imul
            case 0xB0: // cmpxchg 8 bits
                goto error;

            case 0xB1: // cmpxchg 32 bits
            case 0xB6: case 0xB7: // movzx
            case 0xBC: /* bsf */ case 0xBD: // bsr
                // case 0xBE: case 0xBF: // movsx 
            case 0xC1: // xadd
            case 0xC7: // cmpxchg8b
                goto modrm;

            default:
                goto error;
            } // switch

         // ALL THE THE REST OF THE INSTRUCTIONS; these are instructions that runtime system shouldn't ever use
        default:
            /* case 0x26: case 0x36: // ES: SS: prefixes
               case 0x9A:
               case 0xC8: case 0xCA: case 0xCB: case 0xCD: case 0xCE: case 0xCF:
               case 0xD6: case 0xD7:
               case 0xE4: case 0xE5: case 0xE6: case 0xE7: case 0xEA: case 0xEB: case 0xEC: case 0xED: case 0xEF:
               case 0xF4: case 0xFA: case 0xFB:
                */
            goto error;
        }

    modrm:
        modrm = *p++;
    modrm_fetched:
        if (modrm >= 0xC0)
            return length + 1;  // account for modrm opcode
        else
        {  /* memory access */
            if ((modrm & 0x7) == 0x04)
            { /* instruction with SIB byte */
                length++; // account for SIB byte
                sib = *p++; // fetch the sib byte
                if ((sib & 0x7) == 0x05)
                {
                    if ((modrm & 0xC0) == 0x40)
                        return length + 1 + 1; // account for MOD + byte displacment
                    else return length + 1 + 4; // account for MOD + dword displacement
                }
            }
            switch (modrm & 0xC0)
            {
            case 0x0:
                if ((modrm & 0x07) == 0x05)
                    return length + 5; // 4 byte displacement
                else return length + 1; // zero length offset
            case 0x80:
                return length + 5;  // 4 byte offset
            default:
                return length + 2;  // one byte offset
            }
        }

    error:
        {
            //printf("InstructionLength: unhandled opcode at %8x with opcode %2x\n", pc, opcode);
        }
        return 0; // can't actually execute this
    }
}