/*
	gp3200+ float32
	cording by daretoku_taka
*/
#include "gp3200+.h"


gp3200_float gp3200_f32_int2float(INT32 val)
{
	return val * 1024;
}

gp3200_float gp3200_f32_float2f32(float val)
{
	return val * 1024.0f;
}

gp3200_float gp3200_f32_fadd(gp3200_float val1, gp3200_float val2)
{
	return gp3200_f32_float2f32(gp3200_f32_getfloat(val1) + gp3200_f32_getfloat(val2));
}

gp3200_float gp3200_f32_fsub(gp3200_float val1, gp3200_float val2)
{
	return gp3200_f32_float2f32(gp3200_f32_getfloat(val1) - gp3200_f32_getfloat(val2));
}

gp3200_float gp3200_f32_fmul(gp3200_float val1, gp3200_float val2)
{
	return gp3200_f32_float2f32(gp3200_f32_getfloat(val1) * gp3200_f32_getfloat(val2));
}

gp3200_float gp3200_f32_fdiv(gp3200_float val1, gp3200_float val2)
{
	return gp3200_f32_float2f32(gp3200_f32_getfloat(val1) / gp3200_f32_getfloat(val2));
}

float gp3200_f32_getfloat(gp3200_float float32)
{
	return (float)float32 / 1024.0f;
}
