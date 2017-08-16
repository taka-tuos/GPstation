/*
	gp3200+ maincode
	cording by daretoku_taka
	version : 1.0
	codename : thundervolt
*/

#include "gp3200+.h"
#include <stdlib.h>


//#ifdef __DEBUG__
const char *opdis[] = {
	"0nop",
	"2mov",
	"2add",
	"2sub",
	"2imul",
	"2idiv",
	"2and",
	"2or",
	"1not",
	"2xor",
	"1int",
	"1call",
	"0ret",
	"1jmp",
	"2cmp",
	"2sar",
	"2sal",
	"1je",
	"1jae",
	"1jbe",
	"1ja",
	"1jb",
	"1jz",
	"1jnz",
	"1jne",
	"2in",
	"2out",
	"1push",
	"1pop",
	"0cli",
	"0sti",
	"2fmov",
	"2fadd",
	"2fsub",
	"2fmul",
	"2fdiv",
	"2farjmp",
	"2farcall",
	"0farret",
	"1lsp",
	"1ssp",
	"2hmxs",
	"1hmxc",
	"2hmxi",
	"1htxs",
	"1htxl",
	"2htxp",
	"2umul",
	"2udiv",
};
/*
void args_print(UINT8 type1, UINT8 type2, UINT8 args1, UINT8 args2, int isdouble, int isfloat, gp3200_cpu *cpu)
{
	if(isfloat) {
		
		
		if(isdouble) {
			
		}
	} else {
		INT32 data1,data2;
		unsigned int plus;
		
		switch(type1) {
		case ARGS_IR:
			data = cpu->r_regs[args1 & 0xff];
			break;
		case ARGS_MR:
			data1 = *((INT32 *)&cpu->r_memory[cpu->r_regs[args1 & 0xff] + cpu->r_segs[1]]);
			break;
		case ARGS_MRAR:
			data1 = *((INT32 *)&cpu->r_memory[(cpu->r_regs[args1 & 0xff] + cpu->r_segs[1] + cpu->r_regs[cpu->r_memory[cpu->r_pc+1]])]);
			break;
		case ARGS_MRAE:
			plus = *((UINT32 *)&cpu->r_memory[cpu->r_pc + 1]);
			data1 = *((INT32 *)&cpu->r_memory[cpu->r_regs[args1 & 0xff] + cpu->r_segs[1] + plus]);
			break;
		case ARGS_ME:
			data1 = *((INT32 *)&cpu->r_memory[args1 + cpu->r_segs[1]]);
			break;
		case ARGS_E:
			data1 = *((int *)&args1);
			break;
		case ARGS_SR:
			data1 = *(&cpu->r_segs[args1 & 0xff]);
			break;
		}
		
		if(isdouble) {
			printf(",");
			switch(type[1]) {
			case ARGS_IR:
				data2 = &cpu->r_regs[args2 & 0xff];
				break;
			case ARGS_MR:
				data2 = *((INT32 *)&cpu->r_memory[cpu->r_regs[args2 & 0xff] + cpu->r_segs[1]]);
				break;
			case ARGS_ME:
				data2 = *((INT32 *)&cpu->r_memory[args2 + cpu->r_segs[1]]);
				break;
			case ARGS_MRAR:  
				data2 = *((INT32 *)&cpu->r_memory[(cpu->r_regs[args2 & 0xff] + cpu->r_segs[1] + (((UINT32 *)cpu->r_memory)[cpu->r_pc+1]))]);
				break;
			case ARGS_MRAE:
				plus = *((UINT32 *)&cpu->r_memory[cpu->r_pc + 1]);
				data2 = *((INT32 *)&cpu->r_memory[cpu->r_regs[args2 & 0xff] + cpu->r_segs[1] + plus]);
				break;
			case ARGS_E:
				data2 = *((int *)&args2);
				break;
			case ARGS_FR:
				data2 = (INT32)gp3200_f32_getfloat(cpu->f_regs[args2 & 0xff]);
				if(type[0] == ARGS_MR || type[0] == ARGS_ME) data2 = *((INT32 *)&cpu->f_regs[args2 & 0xff]);
				break;
			case ARGS_FE:
				data2 = (INT32)gp3200_f32_getfloat(gp3200_f32_int2float(args2));
				break;
			case ARGS_SR:
				data2 = *(&cpu->r_segs[args2 & 0xff]);
				break;
			}
		}
		
		printf(")\n");
	}
}

#endif*/

void gp3200_get_int1(UINT32 **data, gp3200_cpu *cpu)
{
	UINT8 type = cpu->r_memory[cpu->r_pc + 1] & 0xf;
	UINT32 args1 = *((UINT32 *)&cpu->r_memory[cpu->r_pc + 2]);

	cpu->r_pc += 2;
	
	switch(type) {
	case ARGS_IR:
		cpu->r_pc += 1;
		*data = &cpu->r_regs[args1 & 0xff];
		break;
	case ARGS_MR:
		cpu->r_pc += 1;
		*data = (UINT32 *)&cpu->r_memory[cpu->r_regs[args1 & 0xff] + cpu->r_segs[1]];
		break;
	case ARGS_MRAR:
		*data = (UINT32 *)&cpu->r_memory[(cpu->r_regs[args1 & 0xff] + cpu->r_segs[1] + *((UINT32 *)&cpu->r_memory[cpu->r_pc+1]))];
		cpu->r_pc += 5;
		break;
	case ARGS_MRAE:
		*data = (UINT32 *)&cpu->r_memory[(args1 + cpu->r_segs[1] + *((UINT32 *)(&cpu->r_memory[cpu->r_pc+4])))];
		cpu->r_pc += 5;
		break;
	case ARGS_ME:
		cpu->r_pc += 4;
		*data = (UINT32 *)&cpu->r_memory[args1 + cpu->r_segs[1]];
		break;
	case ARGS_E:
		cpu->r_pc += 4;
		**data = *((int *)&args1);
		break;
	case ARGS_SR:
		cpu->r_pc += 1;
		*data = &cpu->r_segs[args1 & 0xff];
		break;
	}
	
	
#ifdef __DEBUG__
	printf("(%08x)\n",**data);
#endif
}
	
int gp3200_get_int2(UINT32 **data1, UINT32 **data2, gp3200_cpu *cpu)
{
	UINT8 type[2];
	UINT32 args1 = *((UINT32 *)&cpu->r_memory[cpu->r_pc + 2]);
	UINT32 args2;
	UINT32 bpd = (cpu->r_memory[cpu->r_pc] & 0x03) + 1;
	
	type[0] = cpu->r_memory[cpu->r_pc + 1] & 0xf;
	type[1] = (cpu->r_memory[cpu->r_pc + 1] >> 4) & 0xf;

	cpu->r_pc += 2;

	unsigned int plus;
	
	switch(type[0]) {
	case ARGS_IR:
		cpu->r_pc += 1;
		*data1 = &cpu->r_regs[args1 & 0xff];
		break;
	case ARGS_MR:
		cpu->r_pc += 1;
		*data1 = (UINT32 *)&cpu->r_memory[cpu->r_regs[args1 & 0xff] + cpu->r_segs[1]];
		break;
	case ARGS_MRAR:
		*data1 = (UINT32 *)&cpu->r_memory[(cpu->r_regs[args1 & 0xff] + cpu->r_segs[1] + cpu->r_regs[cpu->r_memory[cpu->r_pc+1]])];
		cpu->r_pc += 5;
		break;
	case ARGS_MRAE:
		plus = *((UINT32 *)&cpu->r_memory[cpu->r_pc + 1]);
		*data1 = (UINT32 *)&cpu->r_memory[cpu->r_regs[args1 & 0xff] + cpu->r_segs[1] + plus];
		cpu->r_pc += 5;
		break;
	case ARGS_ME:
		cpu->r_pc += 4;
		*data1 = (UINT32 *)&cpu->r_memory[args1 + cpu->r_segs[1]];
		break;
	case ARGS_E:
		cpu->r_pc += 4;
		**data1 = *((int *)&args1);
		break;
	case ARGS_SR:
		cpu->r_pc += 1;
		*data1 = &cpu->r_segs[args1 & 0xff];
		break;
	}
	
	args2 = *((UINT32 *)&cpu->r_memory[cpu->r_pc + 0]);
	
	switch(type[1]) {
	case ARGS_IR:
		cpu->r_pc += 1;
		*data2 = &cpu->r_regs[args2 & 0xff];
		break;
	case ARGS_MR:
		cpu->r_pc += 1;
		*data2 = (UINT32 *)&cpu->r_memory[cpu->r_regs[args2 & 0xff] + cpu->r_segs[1]];
		bpd <<= 4;
		break;
	case ARGS_ME:
		cpu->r_pc += 4;
		*data2 = (UINT32 *)&cpu->r_memory[args2 + cpu->r_segs[1]];
		bpd <<= 4;
		break;
	case ARGS_MRAR:  
		*data2 = (UINT32 *)&cpu->r_memory[(cpu->r_regs[args2 & 0xff] + cpu->r_segs[1] + (((UINT32 *)cpu->r_memory)[cpu->r_pc+1]))];
		cpu->r_pc += 5;
		bpd <<= 4;
		break;
	case ARGS_MRAE:
		plus = *((UINT32 *)&cpu->r_memory[cpu->r_pc + 1]);
		*data2 = (UINT32 *)&cpu->r_memory[cpu->r_regs[args2 & 0xff] + cpu->r_segs[1] + plus];
		cpu->r_pc += 5;
		bpd <<= 4;
		break;
	case ARGS_E:
		cpu->r_pc += 4;
		**data2 = *((int *)&args2);
		break;
	case ARGS_FR:
		cpu->r_pc += 1;
		**data2 = (UINT32)gp3200_f32_getfloat(cpu->f_regs[args2 & 0xff]);
		if(type[0] == ARGS_MR || type[0] == ARGS_ME) *data2 = (UINT32 *)&cpu->f_regs[args2 & 0xff];
		break;
	case ARGS_FE:
		cpu->r_pc += 4;
		**data2 = (UINT32)gp3200_f32_getfloat(gp3200_f32_int2float(args2));
		break;
	case ARGS_SR:
		cpu->r_pc += 1;
		*data2 = &cpu->r_segs[args2 & 0xff];
		break;
	}
	
	
#ifdef __DEBUG__
	printf("(%08x,%08x)\n",**data1,**data2);
#endif
	
	return bpd;
}

void gp3200_get_float2(gp3200_float **data1, gp3200_float **data2, gp3200_cpu *cpu)
{
	UINT8 opcode = cpu->r_memory[cpu->r_pc + 0] >> 2;
	UINT8 type[2];
	UINT32 args1 = *((UINT32 *)&cpu->r_memory[cpu->r_pc + 2]);
	UINT32 args2;
	
	type[0] = cpu->r_memory[cpu->r_pc + 1] & 0xf;
	type[1] = (cpu->r_memory[cpu->r_pc + 1] >> 4) & 0xf;

	cpu->r_pc += 2;
	
	switch(type[0]) {
	case ARGS_FR:
		cpu->r_pc += 1;
		*data1 = &cpu->f_regs[args1 & 0xff];
		break;
	}
	
	args2 = *((UINT32 *)&cpu->r_memory[cpu->r_pc + 0]);
	
	switch(type[1]) {
	case ARGS_FR:
		cpu->r_pc += 1;
		*data2 = &cpu->f_regs[args2 & 0xff];
		break;
	case ARGS_IR:
		cpu->r_pc += 1;
		**data2 = gp3200_f32_int2float(cpu->r_regs[args2 & 0xff]);
		break;
	case ARGS_MR:
		cpu->r_pc += 1;
		**data2 = gp3200_f32_int2float(cpu->r_memory[cpu->r_regs[args2 & 0xff] + cpu->r_segs[1]]);
		break;
	case ARGS_MRAR:
		*data2 = (UINT32 *)&cpu->r_memory[(cpu->r_regs[args2 & 0xff] + cpu->r_segs[1] + *((UINT32 *)(&cpu->r_memory[cpu->r_pc+1])))];
		cpu->r_pc += 5;
		break;
	case ARGS_MRAE:
		*data2 = (UINT32 *)&cpu->r_memory[(args2 + cpu->r_segs[1] + *((UINT32 *)(&cpu->r_memory[cpu->r_pc+4])))];
		cpu->r_pc += 5;
		break;
	case ARGS_ME:
		cpu->r_pc += 4;
		**data2 = gp3200_f32_int2float(cpu->r_memory[args2 + cpu->r_segs[1]]);
		break;
	case ARGS_E:
		cpu->r_pc += 4;
		**data2 = gp3200_f32_int2float(*((int *)&args2));
		break;
	case ARGS_FE:
		cpu->r_pc += 4;
		**data2 = *((gp3200_float *)&args2);
		break;
	}
	
#ifdef __DEBUG__
	printf("(%f,%f)\n",gp3200_f32_getfloat(**data1),gp3200_f32_getfloat(**data2));
#endif
};

void gp3200_init(gp3200_cpu *cpu, UINT8 *memory)
{
	int i;
	cpu->r_if = 1;
	for(i = 0; i < 8; i++) cpu->r_segs[i] = 0;
	cpu->r_memory = memory;
	cpu->on_int = 0;
}

void gp3200_loadhtx(gp3200_cpu *cpu, int ind)
{
	int i;
	for(i = 0; i < 36; i++) cpu->r_regs[i] = cpu->r_htx[ind].r_regs[i];
	for(i = 0; i < 8; i++) cpu->r_segs[i] = cpu->r_htx[ind].r_segs[i];
	for(i = 0; i < 32; i++) cpu->f_regs[i] = cpu->r_htx[ind].f_regs[i];
	cpu->r_pc = cpu->r_htx[ind].r_pc;
}

void gp3200_savehtx(gp3200_cpu *cpu, int ind)
{
	int i;
	for(i = 0; i < 36; i++) cpu->r_htx[ind].r_regs[i] = cpu->r_regs[i];
	for(i = 0; i < 8; i++) cpu->r_htx[ind].r_segs[i] = cpu->r_segs[i];
	for(i = 0; i < 32; i++) cpu->r_htx[ind].f_regs[i] = cpu->f_regs[i];
	cpu->r_htx[ind].r_pc = cpu->r_pc;
}

void gp3200_setphtx(gp3200_cpu *cpu, int ind, UINT32 pc)
{
	cpu->r_htx[ind].r_pc = pc;
}

void gp3200_push(gp3200_cpu *cpu, INT32 data)
{
	*((INT32 *)(cpu->r_memory + cpu->r_regs[32])) = data;
#ifdef __DEBUG__
	printf("push : sp=%08x\n", cpu->r_regs[32]);
	printf("push : [sp]=%08x\n", data);
#endif
	cpu->r_regs[32] -= 4;
}

INT32 gp3200_pop(gp3200_cpu *cpu)
{
	cpu->r_regs[32] += 4;
#ifdef __DEBUG__
	printf("pop : sp=%08x\n", cpu->r_regs[32]);
	printf("pop : [sp]=%08x\n", *((INT32 *)(cpu->r_memory + cpu->r_regs[32])));
#endif
	return *((INT32 *)(cpu->r_memory + cpu->r_regs[32]));
}

void gp3200_step(gp3200_cpu *cpu)
{
	UINT32 used, used1, used2;
	UINT32 *data = &used, *data1 = &used1, *data2 = &used2, bpd, dmy;
	UINT8 opcode = *(cpu->r_memory + cpu->r_pc + cpu->r_segs[0]) >> 2;
	
#ifdef __DEBUG__
	printf("%08x : ", cpu->r_pc + cpu->r_segs[0]);
	if(opcode <= 0x30) {
		printf(opdis[opcode]+1);
		if(opdis[opcode][0] - '0' == 0) printf("()\n");
	} else {
		printf("db(0x%02x)\n",opcode);
	}
#endif
	
	/*
#ifdef __DEBUG__
	printf("%x : ", cpu->r_pc + cpu->r_segs[0]);
	if(opcode <= 0x30) {
		printf(opdis[opcode]+1);
		printf("(");
		if(opdis[opcode][0] - '0' == 0) printf(")\n");
	} else {
		printf("db(0x%02x)\n",opcode);
	}
#endif
	*/
	switch(opcode) {
	case 0x00:		// NOP
		cpu->r_pc++;
		break;
	case 0x01:		// MOV
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLC(=);
		break;
	case 0x02:		// ADD
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLC(+=);
		break;
	case 0x03:		// SUB
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLC(-=);
		break;
	case 0x04:		// IMUL
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLCS(*=);
		break;
	case 0x05:		// IDIV
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLCS(/=);
		GP32_CLC_EX(cpu->r_regs[33],%)
		break;
	case 0x06:		// AND
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLC(&=);
		break;
	case 0x07:		// OR
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLC(|=);
		break;
	case 0x08:		// NOT
		gp3200_get_int1(&data, cpu);
		*data = ~*data;
		break;
	case 0x09:		// XOR
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLC(^=);
		break;
	case 0x0a:		// INT
		gp3200_get_int1(&data, cpu);
		
		if(cpu->r_if == 1) {
			if(cpu->on_int == 0) {
				cpu->r_pc = *((UINT32 *)&cpu->r_memory[*data * 8]);
				cpu->r_segs[0] = *((UINT32 *)&cpu->r_memory[*data * 8 + 4]);
			}
			else cpu->on_int(cpu, *data, cpu->r_memory);
		}
		break;
	case 0x0b:		// CALL
		gp3200_get_int1(&data, cpu);
		gp3200_push(cpu, cpu->r_pc);
		cpu->r_pc = *data;
		break;
	case 0x0c:		// RET
		cpu->r_pc = gp3200_pop(cpu);
		break;
	case 0x0d:		// JMP
		gp3200_get_int1(&data, cpu);
		cpu->r_pc = *data;
		break;
	case 0x0e:		// CMP
		gp3200_get_int2(&data1, &data2, cpu);
		cpu->r_regs[35] = 0;
		if(*data1 == *data2) cpu->r_regs[35] |= 1 << 0;
		if(*data1 >= *data2) cpu->r_regs[35] |= 1 << 1;
		if(*data1 <= *data2) cpu->r_regs[35] |= 1 << 2;
		if(*data1 >  *data2) cpu->r_regs[35] |= 1 << 3;
		if(*data1 <  *data2) cpu->r_regs[35] |= 1 << 4;
		if(*data1 ==      0) cpu->r_regs[35] |= 1 << 5;
		if(*data1 !=      0) cpu->r_regs[35] |= 1 << 6;
		if(*data1 != *data2) cpu->r_regs[35] |= 1 << 7;
		break;
	case 0x0f:		// SAR
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLC(>>=);
		break;
	case 0x10:		// SAL
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLC(<<=);
		break;
	case 0x11:		// JE
		gp3200_get_int1(&data, cpu);
		
		if(cpu->r_regs[35] & (1 << 0))
		{
			cpu->r_pc = *data;
			cpu->r_regs[35] = 0;
		}
		break;
	case 0x12:		// JAE
		gp3200_get_int1(&data, cpu);
		
		if(cpu->r_regs[35] & (1 << 1))
		{
			cpu->r_pc = *data;
			cpu->r_regs[35] = 0;
		}
		break;
	case 0x13:		// JBE
		gp3200_get_int1(&data, cpu);
		
		if(cpu->r_regs[35] & (1 << 2))
		{
			cpu->r_pc = *data;
			cpu->r_regs[35] = 0;
		}
		break;
	case 0x14:		// JA
		gp3200_get_int1(&data, cpu);
		
		if(cpu->r_regs[35] & (1 << 3))
		{
			cpu->r_pc = *data;
			cpu->r_regs[35] = 0;
		}
		break;
	case 0x15:		// JB
		gp3200_get_int1(&data, cpu);
		
		if(cpu->r_regs[35] & (1 << 4))
		{
			cpu->r_pc = *data;
			cpu->r_regs[35] = 0;
		}
		break;
	case 0x16:		// JZ
		gp3200_get_int1(&data, cpu);
		
		if(cpu->r_regs[35] & (1 << 5))
		{
			cpu->r_pc = *data;
			cpu->r_regs[35] = 0;
		}
		break;
	case 0x17:		// JNZ
		gp3200_get_int1(&data, cpu);
		
		if(cpu->r_regs[35] & (1 << 6))
		{
			cpu->r_pc = *data;
			cpu->r_regs[35] = 0;
		}
		break;
	case 0x18:		// JNE
		gp3200_get_int1(&data, cpu);
		
		if(cpu->r_regs[35] & (1 << 7))
		{
			cpu->r_pc = *data;
			cpu->r_regs[35] = 0;
		}
		break;
	case 0x19:		// IN
		gp3200_get_int2(&data1, &data2, cpu);
		*data2 = cpu->on_ior(cpu,*data1);
		break;
	case 0x1a:		// OUT
		gp3200_get_int2(&data1, &data2, cpu);
		cpu->on_iow(cpu,*data1,*data2);
		break;
	case 0x1b:		// PUSH
		gp3200_get_int1(&data, cpu);
		gp3200_push(cpu, *data);
		break;
	case 0x1c:		// POP
		gp3200_get_int1(&data, cpu);
		*data = gp3200_pop(cpu);
		break;
	case 0x1d:		// CLI
		cpu->r_if = 0;
		cpu->r_pc++;
		break;
	case 0x1e:		// STI
		cpu->r_if = 1;
		cpu->r_pc++;
		break;
	case 0x1f:		// FMOV
		gp3200_get_float2(&data1, &data2, cpu);
		*data1 = *data2;
		break;
	case 0x20:		// FADD
		gp3200_get_float2(&data1, &data2, cpu);
		*data1 = gp3200_f32_fadd(*data1, *data2);
		break;
	case 0x21:		// FSUB
		gp3200_get_float2(&data1, &data2, cpu);
		*data1 = gp3200_f32_fsub(*data1, *data2);
		break;
	case 0x22:		// FMUL
		gp3200_get_float2(&data1, &data2, cpu);
		*data1 = gp3200_f32_fmul(*data1, *data2);
		break;
	case 0x23:		// FDIV
		gp3200_get_float2(&data1, &data2, cpu);
		*data1 = gp3200_f32_fdiv(*data1, *data2);
		break;
	case 0x24:		// FARJMP
		gp3200_get_int2(&data1, &data2, cpu);
		cpu->r_pc = *data1;
		cpu->r_segs[0] = *data2;
		break;
	case 0x25:		// FARCALL
		gp3200_get_int2(&data1, &data2,cpu);
		gp3200_push(cpu, cpu->r_pc);
		gp3200_push(cpu, cpu->r_segs[0]);
		cpu->r_pc = *data1;
		cpu->r_segs[0] = *data2;
		break;
	case 0x26:		// FARRET
		cpu->r_segs[0] = gp3200_pop(cpu);
		cpu->r_pc = gp3200_pop(cpu);
		break;
	case 0x27:		// LSP
		gp3200_get_int1(&data, cpu);
		cpu->r_regs[32] = *data;
		break;
	case 0x28:		// SSP
		gp3200_get_int1(&data, cpu);
		*data = cpu->r_regs[32];
		break;
	case 0x29:		// HMXS
		gp3200_get_int2(&data1, &data2,cpu);
		cpu->cme_dst = &cpu->r_memory[*data1];
		cpu->cme_src = &cpu->r_memory[*data2];
		break;
	case 0x2a:		// HMXC
		gp3200_get_int1(&data, cpu);
		memcpy(cpu->cme_dst,cpu->cme_src,*data);
		break;
	case 0x2b:		// HMXI
		gp3200_get_int2(&data1, &data2,cpu);
		memset(cpu->cme_dst,*data1,*data2);
		break;
	case 0x2c:		// HTXS
		gp3200_get_int1(&data, cpu);
		gp3200_savehtx(cpu,*data);
		break;
	case 0x2d:		// HTXL
		gp3200_get_int1(&data, cpu);
		gp3200_loadhtx(cpu,*data);
		break;
	case 0x2e:		// HTXP
		gp3200_get_int2(&data1, &data2,cpu);
		gp3200_setphtx(cpu,*data1,*data2);
		break;
	case 0x2f:		// UMUL
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLC(*=);
		break;
	case 0x30:		// UDIV
		bpd = gp3200_get_int2(&data1, &data2, cpu);
		GP32_CLC(/=);
		GP32_CLC_EX(cpu->r_regs[33], %)
			break;
	default:
		break;
	}
}
