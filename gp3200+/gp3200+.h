/*
	gp3200+ header
	cording by daretoku_taka
	version : 1.0
	codename : thunderbolt
*/

#include <stdio.h>
#include <string.h>

#ifndef __GP3200P_H__
#define __GP3200P_H__

#define GP32P_REV		"gp3200+ 1.0"

#ifndef UINT8
typedef unsigned char UINT8;
#endif
#ifndef UINT16
typedef unsigned short UINT16;
#endif
#ifndef UINT32
typedef unsigned int UINT32;
#endif
#ifndef UINT
typedef unsigned int UINT;
#endif
#ifndef INT8
typedef signed char INT8;
#endif
#ifndef INT16
typedef signed short INT16;
#endif
#ifndef INT32
typedef signed int INT32;
#endif

#define		ARGS_IR		(0x00)
#define		ARGS_MR		(0x01)
#define		ARGS_ME		(0x02)
#define		ARGS_E		(0x03)
#define		ARGS_FR		(0x04)
#define		ARGS_FE		(0x05)
#define		ARGS_SR		(0x06)
#define		ARGS_MRAE	(0x07)
#define		ARGS_MRAR	(0x08)
#define		ARGS_RAE	(0x09)
#define		ARGS_RAR	(0x0a)


typedef UINT32 gp3200_float;

typedef struct {
	UINT32 r_regs[36]; /* R0 - R31, SP, Y, BP, F */
	UINT32 r_segs[8]; /* S0 - S7 */ /* CS = S0 , DS = S1 */
	gp3200_float f_regs[32]; /* F0 - F31 */
	UINT32 r_pc;
} gp3200_htx;

typedef struct {
	/* register */
	UINT32 r_regs[36]; /* R0 - R31, SP, Y, BP, F */
	UINT32 r_segs[8]; /* S0 - S7 */ /* CS = S0 , DS = S1 */
	gp3200_float f_regs[32]; /* F0 - F31 */
	UINT32 r_pc;
	//UINT32 r_sp;
	gp3200_htx r_htx[512];
	
	/* use emulation */
	UINT8 r_if;
	UINT32 r_hf;
	void *cme_dst;
	void *cme_src;
	
	/* i/o & memory */
	UINT8 *r_memory;

	/* callback */
	void (*on_int)(void *cpu,int intno,UINT8 *r_memory);
	void (*on_iow)(void *cpu, int address, int data);
	void (*on_wait)(int msec);
	int (*on_ior)(void *cpu, int address);
} gp3200_cpu;

#define GP32_CLCS_EX(v,op) \
		if(bpd == 0x04) v = *data1 op *data2; \
		else if(bpd == 0x02) v = *((INT16 *)data1) op *data2; \
		else if(bpd == 0x01) v = *((INT8 *)data1) op *data2; \
		else if(bpd == 0x40) v = *data1 op *data2; \
		else if(bpd == 0x20) v = *data1 op *((INT16 *)data2); \
		else if(bpd == 0x10) v = *data1 op *((INT8 *)data2); \

#define GP32_CLC_EX(v,op) \
		if(bpd == 0x04) v = *((UINT32 *)data1) op *((UINT32 *)data2); \
		else if(bpd == 0x02) v = *((UINT16 *)data1) op *((UINT32 *)data2); \
		else if(bpd == 0x01) v = *((UINT8 *)data1) op *((UINT32 *)data2); \
		else if(bpd == 0x40) v = *((UINT32 *)data1) op *((UINT32 *)data2); \
		else if(bpd == 0x20) v = *((UINT32 *)data1) op *((UINT16 *)data2); \
		else if(bpd == 0x10) v = *((UINT32 *)data1) op *((UINT8 *)data2); \

#define GP32_CLCS(op) GP32_CLCS_EX(dmy,op)

#define GP32_CLC(op) GP32_CLC_EX(dmy,op)


/* float32.c */
gp3200_float gp3200_f32_float2f32(float val);
gp3200_float gp3200_f32_int2float(INT32 val);
gp3200_float gp3200_f32_fadd(gp3200_float val1, gp3200_float val2);
gp3200_float gp3200_f32_fsub(gp3200_float val1, gp3200_float val2);
gp3200_float gp3200_f32_fmul(gp3200_float val1, gp3200_float val2);
gp3200_float gp3200_f32_fdiv(gp3200_float val1, gp3200_float val2);
float gp3200_f32_getfloat(gp3200_float float32);

/* gp3200.c */
void gp3200_init(gp3200_cpu *cpu, UINT8 *memory);
void gp3200_step(gp3200_cpu *cpu);

#endif
