/* $Id: ecoh571.c,v 1.2 2008/10/30 16:00:19 aantipa Exp $ */
/* $Source: /cvs/ca/eng/ecoh/src/Optimized_64bit/ecoh571.c,v $ */
/*

	ecoh571.c			
	Elliptic Curve Only Hash implementation using sect571r1.


        COMMAND LINES:

		GNU GCC
        gcc 64-bit:     gcc -o ecoh.exe -D "_x64_" -O3 ecoh.param.c ecoh.c ecoh.main.c
        gcc 32-bit:     gcc -o ecoh.exe -O3 ecoh.param.c ecoh.c ecoh.main.c
		
		MICROSOFT VISUAL STUDIO
        cl 64-bit		cl /Feecoh /O2 /GL -D "_x64_" ecoh.main.c ecoh.param.c ecoh.c
		cl 32-bit		cl /Feecoh /O2 /GL ecoh.main.c ecoh.param.c ecoh.c
						
*/


#include "ecoh.h"

#define SHIFT_RIGHT_19(x18,x17,x16,x15,x14,x13,x12,x11,x10,x9,x8,x7,x6,x5,x4,x3,x2,x1,x0) \
(x0)  = (((uint)x0)  >> 1) + (((uint)x1)  << (LIMB_BIT_SIZE-1)); \
(x1)  = (((uint)x1)  >> 1) + (((uint)x2)  << (LIMB_BIT_SIZE-1)); \
(x2)  = (((uint)x2)  >> 1) + (((uint)x3)  << (LIMB_BIT_SIZE-1)); \
(x3)  = (((uint)x3)  >> 1) + (((uint)x4)  << (LIMB_BIT_SIZE-1)); \
(x4)  = (((uint)x4)  >> 1) + (((uint)x5)  << (LIMB_BIT_SIZE-1)); \
(x5)  = (((uint)x5)  >> 1) + (((uint)x6)  << (LIMB_BIT_SIZE-1)); \
(x6)  = (((uint)x6)  >> 1) + (((uint)x7)  << (LIMB_BIT_SIZE-1)); \
(x7)  = (((uint)x7)  >> 1) + (((uint)x8)  << (LIMB_BIT_SIZE-1)); \
(x8)  = (((uint)x8)  >> 1) + (((uint)x9)  << (LIMB_BIT_SIZE-1)); \
(x9)  = (((uint)x9)  >> 1) + (((uint)x10) << (LIMB_BIT_SIZE-1)); \
(x10) = (((uint)x10) >> 1) + (((uint)x11) << (LIMB_BIT_SIZE-1)); \
(x11) = (((uint)x11) >> 1) + (((uint)x12) << (LIMB_BIT_SIZE-1)); \
(x12) = (((uint)x12) >> 1) + (((uint)x13) << (LIMB_BIT_SIZE-1)); \
(x13) = (((uint)x13) >> 1) + (((uint)x14) << (LIMB_BIT_SIZE-1)); \
(x14) = (((uint)x14) >> 1) + (((uint)x15) << (LIMB_BIT_SIZE-1)); \
(x15) = (((uint)x15) >> 1) + (((uint)x16) << (LIMB_BIT_SIZE-1)); \
(x16) = (((uint)x16) >> 1) + (((uint)x17) << (LIMB_BIT_SIZE-1)); \
(x17) = (((uint)x17) >> 1) + (((uint)x18) << (LIMB_BIT_SIZE-1)); \
(x18)          >>= 1;

#define SHIFT_LEFT_19(x18,x17,x16,x15,x14,x13,x12,x11,x10,x9,x8,x7,x6,x5,x4,x3,x2,x1,x0) \
(x18) = (((uint)x18) << 1) + (((uint)x17) >> (LIMB_BIT_SIZE-1)); \
(x17) = (((uint)x17) << 1) + (((uint)x16) >> (LIMB_BIT_SIZE-1)); \
(x16) = (((uint)x16) << 1) + (((uint)x15) >> (LIMB_BIT_SIZE-1)); \
(x15) = (((uint)x15) << 1) + (((uint)x14) >> (LIMB_BIT_SIZE-1)); \
(x14) = (((uint)x14) << 1) + (((uint)x13) >> (LIMB_BIT_SIZE-1)); \
(x13) = (((uint)x13) << 1) + (((uint)x12) >> (LIMB_BIT_SIZE-1)); \
(x12) = (((uint)x12) << 1) + (((uint)x11) >> (LIMB_BIT_SIZE-1)); \
(x11) = (((uint)x11) << 1) + (((uint)x10) >> (LIMB_BIT_SIZE-1)); \
(x10) = (((uint)x10) << 1) + (((uint)x9)  >> (LIMB_BIT_SIZE-1)); \
(x9)  = (((uint)x9)  << 1) + (((uint)x8)  >> (LIMB_BIT_SIZE-1)); \
(x8)  = (((uint)x8)  << 1) + (((uint)x7)  >> (LIMB_BIT_SIZE-1)); \
(x7)  = (((uint)x7)  << 1) + (((uint)x6)  >> (LIMB_BIT_SIZE-1)); \
(x6)  = (((uint)x6)  << 1) + (((uint)x5)  >> (LIMB_BIT_SIZE-1)); \
(x5)  = (((uint)x5)  << 1) + (((uint)x4)  >> (LIMB_BIT_SIZE-1)); \
(x4)  = (((uint)x4)  << 1) + (((uint)x3)  >> (LIMB_BIT_SIZE-1)); \
(x3)  = (((uint)x3)  << 1) + (((uint)x2)  >> (LIMB_BIT_SIZE-1)); \
(x2)  = (((uint)x2)  << 1) + (((uint)x1)  >> (LIMB_BIT_SIZE-1)); \
(x1)  = (((uint)x1)  << 1) + (((uint)x0)  >> (LIMB_BIT_SIZE-1)); \
(x0)           <<= 1;

#define SHIFT_RIGHT_10(x9,x8,x7,x6,x5,x4,x3,x2,x1,x0) \
(x0) = (((uint)x0) >> 1) + (((uint)x1) << (LIMB_BIT_SIZE-1)); \
(x1) = (((uint)x1) >> 1) + (((uint)x2) << (LIMB_BIT_SIZE-1)); \
(x2) = (((uint)x2) >> 1) + (((uint)x3) << (LIMB_BIT_SIZE-1)); \
(x3) = (((uint)x3) >> 1) + (((uint)x4) << (LIMB_BIT_SIZE-1)); \
(x4) = (((uint)x4) >> 1) + (((uint)x5) << (LIMB_BIT_SIZE-1)); \
(x5) = (((uint)x5) >> 1) + (((uint)x6) << (LIMB_BIT_SIZE-1)); \
(x6) = (((uint)x6) >> 1) + (((uint)x7) << (LIMB_BIT_SIZE-1)); \
(x7) = (((uint)x7) >> 1) + (((uint)x8) << (LIMB_BIT_SIZE-1)); \
(x8) = (((uint)x8) >> 1) + (((uint)x9) << (LIMB_BIT_SIZE-1)); \
(x9)         >>= 1;

#define SHIFT_LEFT_10(x9,x8,x7,x6,x5,x4,x3,x2,x1,x0) \
(x9)  = (((uint)x9)  << 1) + (((uint)x8)  >> (LIMB_BIT_SIZE-1)); \
(x8)  = (((uint)x8)  << 1) + (((uint)x7)  >> (LIMB_BIT_SIZE-1)); \
(x7)  = (((uint)x7)  << 1) + (((uint)x6)  >> (LIMB_BIT_SIZE-1)); \
(x6)  = (((uint)x6)  << 1) + (((uint)x5)  >> (LIMB_BIT_SIZE-1)); \
(x5)  = (((uint)x5)  << 1) + (((uint)x4)  >> (LIMB_BIT_SIZE-1)); \
(x4)  = (((uint)x4)  << 1) + (((uint)x3)  >> (LIMB_BIT_SIZE-1)); \
(x3)  = (((uint)x3)  << 1) + (((uint)x2)  >> (LIMB_BIT_SIZE-1)); \
(x2)  = (((uint)x2)  << 1) + (((uint)x1)  >> (LIMB_BIT_SIZE-1)); \
(x1)  = (((uint)x1)  << 1) + (((uint)x0)  >> (LIMB_BIT_SIZE-1)); \
(x0)           <<= 1;
/*****************************************************************************
  
  	external variables required for this file
  
 *****************************************************************************/

extern curve sect283r1;
extern curve sect409r1;
extern curve sect571r1;

extern uchar	TBIT[];
extern uchar	BMASK[];
extern uint		BIT[];
extern uint		F2X2[];
extern uint		F2X22[];
extern uint		HT283[][SECT283_SIZE];
extern uint		HT409[][SECT409_SIZE];
extern uint		HT571[][SECT571_SIZE];
extern uint		NIB_MASK[];

void	PrintInt2(uint sz, uint *op)
{	
	uint i;
	for(i=sz-1;i!=-1;i--){	printf(SZ_PRINTINT, op[i]);	}
}

void	PrintInt(uint sz, uint *op)
{	
	uint i;
	for(i=0;i<sz;i++){	printf(SZ_PRINTINT, op[i]);		}
}
/*****************************************************************************
  
  	Some MACROS for setting and adding counters
  
 *****************************************************************************/
#ifdef _x64_
#define SET(OP,UI,LEN)		{	(OP)[0]=(UI); if((LEN)==2){	OP[1]=0;	} }
#define SETDL(OP,DL,LEN)	SET((OP),(DL),(LEN))
#define SETCTR(OP,CTR,LEN)	{	(OP)[0]=(CTR)[0]; if((LEN)==2){	(OP)[1]=(CTR)[1];	} }
#define ADD(OP,UI,LEN)		{\
	if((((OP)[0])+=(UI))<(UI)){\
		if((LEN)==2){ if((++((OP)[1]))==0)	return FAIL;	}\
		else return FAIL;\
	}\
}
#define ADDDL(OP,DL,LEN)	( ADD((OP),(DL),(LEN))) 

#define INCCTR(CTR, LEN)	{ \
	if(++((CTR)[0])==0){	\
		if((LEN)==2){\
			if(++((CTR)[1])==0)	return FAIL; \
		}\
		else	return FAIL;\
	}\
}
	
#else
#define SET(OP,UI,LEN)		{	(OP)[0]=(UI);	(OP)[1]=0;	if((LEN)==4) { (OP)[2]=0; (OP)[3]=0;	} }
#define SETDL(OP,DL,LEN)	{	(OP)[0]=(uint)((DL)&0xFFFFFFFF);	\
	(OP)[1]=(uint)((DL)>>32);	\
	if((LEN)==4)	{ (OP)[2]=0; (OP)[3]=0;	} \
}
#define SETCTR(OP,CTR,LEN)	{	SETDL((OP), (CTR)[0], 2);	if((LEN)==4)	SETDL(&((OP)[2]), ((CTR)[1]), 2);	}
#define ADD(OP,UI,LEN)		{\
	if((((OP)[0])+=UI)<(UI)){\
		if((++((OP)[1]))==0){\
			if((LEN)==4){\
				if((++((OP)[2]))==0){\
					if((++((OP)[3]))==0)	return FAIL;\
				}}}\
		else	return FAIL;\
	}}\

#define ADDDL(OP,DL,LEN)	{  \
	uint cx = 0;\
	if((((OP)[0])+=(uint)((DL)&0xFFFFFFFF))<(uint)((DL)&0xFFFFFFFF)){\
		cx = 1;\
		if((((OP)[1])+=cx))  cx = 0;\
	}\
	if((((OP)[1])+=(uint)((DL)>>32))<(uint)((DL)>>32))	++cx;\
	if((LEN)==4){\
		if((((OP)[2])+=cx)<cx){\
			if((++((OP)[3]))==0)	return FAIL;\
		}\
	}\
	else \
		if (cx)	return FAIL;\
}
#define INCCTR(CTR, LEN)	{ \
	if(++((CTR)[0])==0){	\
		if((LEN)==4){\
			if(++((CTR)[1])==0)	return FAIL; \
		}\
		else	return FAIL;\
	}\
}
#endif
static const unsigned char byterev[256] =  {
     0x00, 0x80,
     0x40, 0xC0,
     0x20, 0xA0,
     0x60, 0xE0,
     0x10, 0x90,
     0x50, 0xD0,
     0x30, 0xB0,
     0x70, 0xF0,
     0x08, 0x88,
     0x48, 0xC8,
     0x28, 0xA8,
     0x68, 0xE8,
     0x18, 0x98,
     0x58, 0xD8,
     0x38, 0xB8,
     0x78, 0xF8,
     0x04, 0x84,
     0x44, 0xC4,
     0x24, 0xA4,
     0x64, 0xE4,
     0x14, 0x94,
     0x54, 0xD4,
     0x34, 0xB4,
     0x74, 0xF4,
     0x0C, 0x8C,
     0x4C, 0xCC,
     0x2C, 0xAC,
     0x6C, 0xEC,
     0x1C, 0x9C,
     0x5C, 0xDC,
     0x3C, 0xBC,
     0x7C, 0xFC,
     0x02, 0x82,
     0x42, 0xC2,
     0x22, 0xA2,
     0x62, 0xE2,
     0x12, 0x92,
     0x52, 0xD2,
     0x32, 0xB2,
     0x72, 0xF2,
     0x0A, 0x8A,
     0x4A, 0xCA,
     0x2A, 0xAA,
     0x6A, 0xEA,
     0x1A, 0x9A,
     0x5A, 0xDA,
     0x3A, 0xBA,
     0x7A, 0xFA,
     0x06, 0x86,
     0x46, 0xC6,
     0x26, 0xA6,
     0x66, 0xE6,
     0x16, 0x96,
     0x56, 0xD6,
     0x36, 0xB6,
     0x76, 0xF6,
     0x0E, 0x8E,
     0x4E, 0xCE,
     0x2E, 0xAE,
     0x6E, 0xEE,
     0x1E, 0x9E,
     0x5E, 0xDE,
     0x3E, 0xBE,
     0x7E, 0xFE,
     0x01, 0x81,
     0x41, 0xC1,
     0x21, 0xA1,
     0x61, 0xE1,
     0x11, 0x91,
     0x51, 0xD1,
     0x31, 0xB1,
     0x71, 0xF1,
     0x09, 0x89,
     0x49, 0xC9,
     0x29, 0xA9,
     0x69, 0xE9,
     0x19, 0x99,
     0x59, 0xD9,
     0x39, 0xB9,
     0x79, 0xF9,
     0x05, 0x85,
     0x45, 0xC5,
     0x25, 0xA5,
     0x65, 0xE5,
     0x15, 0x95,
     0x55, 0xD5,
     0x35, 0xB5,
     0x75, 0xF5,
     0x0D, 0x8D,
     0x4D, 0xCD,
     0x2D, 0xAD,
     0x6D, 0xED,
     0x1D, 0x9D,
     0x5D, 0xDD,
     0x3D, 0xBD,
     0x7D, 0xFD,
     0x03, 0x83,
     0x43, 0xC3,
     0x23, 0xA3,
     0x63, 0xE3,
     0x13, 0x93,
     0x53, 0xD3,
     0x33, 0xB3,
     0x73, 0xF3,
     0x0B, 0x8B,
     0x4B, 0xCB,
     0x2B, 0xAB,
     0x6B, 0xEB,
     0x1B, 0x9B,
     0x5B, 0xDB,
     0x3B, 0xBB,
     0x7B, 0xFB,
     0x07, 0x87,
     0x47, 0xC7,
     0x27, 0xA7,
     0x67, 0xE7,
     0x17, 0x97,
     0x57, 0xD7,
     0x37, 0xB7,
     0x77, 0xF7,
     0x0F, 0x8F,
     0x4F, 0xCF,
     0x2F, 0xAF,
     0x6F, 0xEF,
     0x1F, 0x9F,
     0x5F, 0xDF,
     0x3F, 0xBF,
     0x7F, 0xFF
};
/*****************************************************************************
  
  	START OF ECOH FUNCTIONS AND AUXILARY FUNCTIONS
  
 *****************************************************************************
 
  	ByteReverse			This function byte reverses limbs for the purpose of
  						formatting
 
 *****************************************************************************/
void Unload(uchar *rop, uint *op, uint count)
{
    uint	i;
	uchar	bval;
	memcpy(rop, (uchar*)op, count);
	for(i=0;i<count/2;i++){
		bval = rop[i];
		rop[i] = rop[count-(i+1)];
		rop[count-(i+1)] = bval;
	}
}
/*****************************************************************************
  
  	LimbReverse			This function limb reverses an multi-limb element for
  						the purpose of formatting
  
 *****************************************************************************/
void FormatData(uint *buffer, uint count)
{
	uint	value, ctr, i = 0;

	ctr = count;
	
	while((--ctr)>(i)){
		value = buffer[i];
		buffer[i++]=buffer[ctr];
		buffer[ctr] = value;
	}
	while( count-- ){
        value = *buffer;
#ifdef _x64_
        value = ((value&0xFF00FF00FF00FF00L)>>8)|((value&0x00FF00FF00FF00FFL)<<8);
        value =((value&0x0000FFFF0000FFFF)<<16)|((value&0xFFFF0000FFFF0000)>>16);
		*buffer++= (value>>32)|(value<<32);
#else
		value = ((value&0xFF00FF00L)>>8)|((value&0x00FF00FFL)<<8);
        *buffer++ =(value<<16)|(value>>16);
#endif
     }
}
/*****************************************************************************

  	BitReverse	This function reverses bits ina an multi-limb element 

 *****************************************************************************/
void BitReverse(uint *buffer, uint count) 
{
	int i; 
	uint reversed, original, temp; 
	char *buf = (char*)buffer; 
	/* do a byte reverse */
	FormatData(buffer, count); 
	/* do a bit reverse in each byte */
	for (i=0; i<count; i++)	{
	/* cmn_wordReverse */
		original = buffer[i]; 
		reversed = byterev[original & 0xFF]; 
                original >>= 8;
		temp = byterev[original & 0xFF]; 
		reversed += temp << 8; 
                original >>= 8;
		temp = byterev[original & 0xFF]; 
                reversed += temp << 16;
                original >>= 8;
		temp = byterev[original & 0xFF]; 
                reversed += temp << 24; 
#ifdef _x64_
                original >>= 8;
		temp = byterev[original & 0xFF]; 
                reversed += temp << 32; 
                original >>= 8;
		temp = byterev[original & 0xFF]; 
                reversed += temp << 40; 
                original >>= 8;
		temp = byterev[original & 0xFF]; 
                reversed += temp << 48; 
                original >>= 8;
		temp = byterev[original & 0xFF]; 
                reversed += temp << 56; 
		
#endif 

		buffer[i] = reversed; 
	}

}
/*****************************************************************************
  
  	START OF f_{2^m} FUNCTIONS
  
 *****************************************************************************
  
  	IsInt()		checks to see if int array is = a single int, returns 0 
  					on false, and 1 on true
 
 *****************************************************************************/
int		IsInt(uint *op, uint sz, uint ui)
{
	uint i;
	if(op[0]!=ui)	return 0;
	for(i=sz-1;i>0;i--){	if(op[i])	return 0;	}
	return 1;
}
/*****************************************************************************

  	GetBitLength()		returns the bit length floor(log_2(op))

 *****************************************************************************/
uint	GetBitLength(uint sz, uint *op)
{
	sint	i, d;
	uint	n;
	if(sz == 0)		return 0;
	d = sz-1;
	while(op[d]==0){	--d;	}
	if(d<0)			return 0;	
	n = op[d];
	i = 0;
	while(n){	n>>=1;	++i;	}
	return (d*(LIMB_BIT_SIZE)) + i;
}
/***************************************************************************** 

 	PolyShiftRight()	This function shifts a polynomial to the right 1 bit
  						in F_2[z]

 *****************************************************************************/
void	PolyShiftRight(uint *rop, uint *op, uint sz)
{
	uint	t1, t2;
	--sz;
	t2 = 0;
	do{
		if(op[sz]&0x01)	t1 = TOPBIT;
		else			t1 = 0;
		rop[sz] = (op[sz]>>1)|t2;
		t2 = t1;
	}while(--sz!=(-1));

}
/*****************************************************************************

  	PolyAdd()		This function add's to polynomials of the same length in 
  					F_2[z]

 *****************************************************************************/
void	PolyAdd(uint *rop, uint *op1, uint *op2, uint sz)
{
	uint i;
	i = -sz;	op1 -=i;	op2 -=i;	rop -=i;
	do{
		rop[i] = op1[i]^op2[i];
	}while(++i!=0);
}
/*****************************************************************************

  	PolyModT571()		Performs modular reduction by the polynomial
  						reduce modulo f(x) = x^571 + x^10 + x^5 + x^2 + 1

  						See [1], Algorithm 2.45, page 56

 *****************************************************************************/
void	PolyModT571(uint *rop, uint *op)
{
	uint j, h1;
#ifdef _x64_
	for(j=17;j>8;j--){
		h1 = op[j];
		op[j-9]^=(h1<<5)^(h1<<7)^(h1<<10)^(h1<<15);
		op[j-8]^=(h1>>59)^(h1>>57)^(h1>>54)^(h1>>49);
	}
	h1 = op[8]>>59;
	op[0] ^= h1^(h1<<2)^(h1<<5)^(h1<<10);
	op[8] &=0x07FFFFFFFFFFFFFF;
#else
	for(j=35;j>17;j--){
		h1 = op[j];
		op[j-18]^=(h1<<5)^(h1<<7)^(h1<<10)^(h1<<15);
		op[j-17]^=(h1>>27)^(h1>>25)^(h1>>22)^(h1>>17);
	}
	h1 = op[17]>>27;
	op[0] ^= h1^(h1<<2)^(h1<<5)^(h1<<10);
	op[17] &=0x07FFFFFF;
#endif
	memcpy(rop, op, sizeof(uint)*SECT571_SIZE);
}
/*****************************************************************************

  	PolyMulMod()		modular polynomial multiplication in GF(2)[X]/(f(x))


  	RESTRICTIONS:		op1, op2 < f(x)

 *****************************************************************************/
void	PolyMulMod(uint *rop, uint *op1, uint *op2, uint sz)
#ifdef _x64_
{
    uint tempB0,tempB1,tempB2,tempB3,tempB4,tempB5,tempB6,tempB7,tempB8,tempB9;
    uint r[18];  /* to store the double size product */
    uint mask;
    uint* ptr;
    int i;

    tempB0 = op2[0];
    tempB1 = op2[1];
    tempB2 = op2[2];
    tempB3 = op2[3];
    tempB4 = op2[4];
    tempB5 = op2[5];
    tempB6 = op2[6];
    tempB7 = op2[7];
    tempB8 = op2[8];
    tempB9 = (uint)0;

    for ( mask = (uint) 0; mask < (uint) 16; ) {
        r[mask++] = 0;
        r[mask++] = 0;
        r[mask++] = 0;
        r[mask++] = 0;
    }
    r[16] = 0;
    r[17] = 0;

    mask = 1;
    for (;;) 
    {
        i = 8;
        for (;;) 
        {
            ptr = r+i;
            if (op1[i] & mask) {
                ptr[0] ^= tempB0;
                ptr[1] ^= tempB1;
                ptr[2] ^= tempB2;
                ptr[3] ^= tempB3;
                ptr[4] ^= tempB4;
                ptr[5] ^= tempB5;
                ptr[6] ^= tempB6;
                ptr[7] ^= tempB7;
                ptr[8] ^= tempB8;
                ptr[9] ^= tempB9;
            }
            i--;
            /* check condition here for an odd number of loops */
            if (i < 0) break;
            ptr--;
            if (op1[i] & mask) {
                ptr[0] ^= tempB0;
                ptr[1] ^= tempB1;
                ptr[2] ^= tempB2;
                ptr[3] ^= tempB3;
                ptr[4] ^= tempB4;
                ptr[5] ^= tempB5;
                ptr[6] ^= tempB6;
                ptr[7] ^= tempB7;
                ptr[8] ^= tempB8;
                ptr[9] ^= tempB9;
            }
            i--;
            /* check condition here for an even number of loops */
        }

        mask <<= 1;
        if (!mask) break;

        SHIFT_LEFT_10(tempB9,tempB8,tempB7,tempB6,tempB5,tempB4,tempB3,tempB2,tempB1,tempB0);
    }

    /* reduce the product to single size and store it in c */
    PolyModT571(rop, r); 
    return;
}
#else
{
    uint tempB0,tempB1,tempB2,tempB3,tempB4,tempB5,tempB6,tempB7,tempB8,tempB9,tempB10,tempB11,tempB12,tempB13,tempB14,tempB15,tempB16,tempB17,tempB18;
    uint r[36];  /* to store the double size product */
    uint mask;
    uint* ptr;
    int i;

    tempB0 = op2[0];
    tempB1 = op2[1];
    tempB2 = op2[2];
    tempB3 = op2[3];
    tempB4 = op2[4];
    tempB5 = op2[5];
    tempB6 = op2[6];
    tempB7 = op2[7];
    tempB8 = op2[8];
    tempB9 = op2[9];
    tempB10 = op2[10];
    tempB11 = op2[11];
    tempB12 = op2[12];
    tempB13 = op2[13];
    tempB14 = op2[14];
    tempB15 = op2[15];
    tempB16 = op2[16];
    tempB17 = op2[17];
    tempB18 = (uint)0;

    for ( mask = (uint) 0; mask < (uint) 36; ) {
        r[mask++] = 0;
        r[mask++] = 0;
        r[mask++] = 0;
        r[mask++] = 0;
    }

    mask = 1;
    for (;;) 
    {
        i = 17;
        for (;;) 
        {
            ptr = r+i;
            if (op1[i] & mask) {
                ptr[0] ^= tempB0;
                ptr[1] ^= tempB1;
                ptr[2] ^= tempB2;
                ptr[3] ^= tempB3;
                ptr[4] ^= tempB4;
                ptr[5] ^= tempB5;
                ptr[6] ^= tempB6;
                ptr[7] ^= tempB7;
                ptr[8] ^= tempB8;
                ptr[9] ^= tempB9;
                ptr[10] ^= tempB10;
                ptr[11] ^= tempB11;
                ptr[12] ^= tempB12;
                ptr[13] ^= tempB13;
                ptr[14] ^= tempB14;
                ptr[15] ^= tempB15;
                ptr[16] ^= tempB16;
                ptr[17] ^= tempB17;
                ptr[18] ^= tempB18;
            }
            i--;
            /* check condition here for an odd number of loops */
            ptr--;
            if (op1[i] & mask) {
                ptr[0] ^= tempB0;
                ptr[1] ^= tempB1;
                ptr[2] ^= tempB2;
                ptr[3] ^= tempB3;
                ptr[4] ^= tempB4;
                ptr[5] ^= tempB5;
                ptr[6] ^= tempB6;
                ptr[7] ^= tempB7;
                ptr[8] ^= tempB8;
                ptr[9] ^= tempB9;
                ptr[10] ^= tempB10;
                ptr[11] ^= tempB11;
                ptr[12] ^= tempB12;
                ptr[13] ^= tempB13;
                ptr[14] ^= tempB14;
                ptr[15] ^= tempB15;
                ptr[16] ^= tempB16;
                ptr[17] ^= tempB17;
                ptr[18] ^= tempB18;
            }
            i--;
            /* check condition here for an even number of loops */
            if (i < 0) break;
        }

        mask <<= 1;
        if (!mask) break;

        SHIFT_LEFT_19(tempB18,tempB17,tempB16,tempB15,tempB14,tempB13,tempB12,tempB11,tempB10,tempB9,tempB8,tempB7,tempB6,tempB5,tempB4,tempB3,tempB2,tempB1,tempB0);
    }

    /* reduce the product to single size and store it in c */
    PolyModT571(rop, r);
    return;
}
#endif 
/*****************************************************************************

  	PolySqrMod()		modular polynomial square in GF(2)[X]/(f(x)) 

  						performs a modified version of [1], Algorithm 2.39, 
  						p. 53

  	RESTRICTIONS:		op < f(x)

 *****************************************************************************/
void	PolySqrMod(uint *rop, uint *op, uint sz)
{
	uint	j, out[2*MAX_SIZE];
	/*************************************************************************

	  	square polynomial using the F2X2 look-up table

	*************************************************************************/
	for(j=0;j<sz;j++){
#ifdef _x64_
		out[2*j] = F2X2[op[j]&0xFF]|(F2X22[(op[j]&0xFF00)>>8]);;
		out[2*j] |= F2X22[(op[j]&0xFF0000)>>16]<<16|(F2X22[(op[j]&0xFF000000)>>24]<<32);
		out[2*j+1] = F2X2[(op[j]&0xFF00000000)>>32]|(F2X22[(op[j]&0xFF0000000000)>>40]);;
		out[2*j+1] |= F2X22[(op[j]&0xFF000000000000)>>48]<<16|(F2X22[(op[j]&0xFF00000000000000)>>56]<<32);
#else
		out[2*j] = F2X2[op[j]&0xFF]|(F2X22[(op[j]&0xFF00)>>8]);;
		out[2*j+1] = F2X2[(op[j]&0xFF0000)>>16]|(F2X22[(op[j]&0xFF000000)>>24]);
#endif
	}
        PolyModT571(rop, out);
}	

/*****************************************************************************

  	PolyInv()		perform a field inversion in GF(2)[X]/(f(x))


  	RESTRICTIONS:		op < f(x)
  	
 *****************************************************************************/
void	PolyInv(uint *rop, uint *op, uint *fx, uint sz)
#ifdef _x64_
{
/* This function requires 4*sbwords+eps of stack storage */

    uint temp[18] = {(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0}; /* temp storage */
    int uc = 8;      /* upper comma. */
    int lc = 8;      /* lower comma. */
    int count = 0;
    int i; 
    uint w;

    /* upper register (merged) */
    uint u9 = (uint)0, u8 = (uint)0, u7 = (uint)0, u6 = (uint)0, u5 = (uint)0, u4 = (uint)0, u3 = (uint)0, u2 = (uint)0, u1 = (uint)0, u0 = (uint)0;

    /* lower register (merged) */
    uint l9 = (uint)0, l8 = (uint)0, l7 = (uint)0, l6 = (uint)0, l5 = (uint)0, l4 = (uint)0, l3 = (uint)0, l2 = (uint)0, l1 = (uint)0, l0 = (uint)0;

    /* multiply element by x^2L */
    temp[sz+8] = op[8];
    temp[sz+7] = op[7];
    temp[sz+6] = op[6];
    temp[sz+5] = op[5];
    temp[sz+4] = op[4];
    temp[sz+3] = op[3];
    temp[sz+2] = op[2];
    temp[sz+1] = op[1];
    temp[sz] = op[0];
    PolyModT571(temp+sz, temp); 
    temp[8] = 0;
    temp[7] = 0;
    temp[6] = 0;
    temp[5] = 0;
    temp[4] = 0;
    temp[3] = 0;
    temp[2] = 0;
    temp[1] = 0;
    temp[0] = 0;
    PolyModT571(temp, temp); 

    /* Final reduce and check to see if the element is zero */
    if ( IsInt(temp, sz, 0)) {
        return;
    }

    u9 = ((uint)1) << (LIMB_BIT_SIZE - 1);
    l9 = 0;
    /* set the modulus */
    u8 = fx[8]; l8 = temp[8];
    u7 = fx[7]; l7 = temp[7];
    u6 = fx[6]; l6 = temp[6];
    u5 = fx[5]; l5 = temp[5];
    u4 = fx[4]; l4 = temp[4];
    u3 = fx[3]; l3 = temp[3];
    u2 = fx[2]; l2 = temp[2];
    u1 = fx[1]; l1 = temp[1];
    u0 = fx[0]; l0 = temp[0];

    /* We know that the irreducible has a 1 in position 0.
     * Jump into the code that shifts a 1 into position 0 of the lower reg.
     */
    goto AlignLower;

    /* main loop */
    /* select an inner loop based upon the comma positions */
    for (;;)
    {
        /* case upper comma == lower comma */
        if (uc == lc) {
            switch (uc) { /* jump to a specific optimized loop */
                case 8: goto commaU8L8;
                case 7: goto commaU7L7;
                case 6: goto commaU6L6;
                case 5: goto commaU5L5;
                case 4: goto commaU4L4;
                case 3: goto commaU3L3;
                case 2: goto commaU2L2;
                case 1: goto commaU1L1;
                case 0: goto commaU0L0;
            }
        }

        /* case upper comma > lower comma */
        while (uc > lc) {

            switch (uc) { /* xor downto uc+1 */
                 case 0: l1 ^= u1;
                 case 1: l2 ^= u2;
                 case 2: l3 ^= u3;
                 case 3: l4 ^= u4;
                 case 4: l5 ^= u5;
                 case 5: l6 ^= u6;
                 case 6: l7 ^= u7;
                 case 7: l8 ^= u8;
                 case 8: l9 ^= u9;
            }
            switch (lc) { /* xor upto lc */
                 case 8: u8 ^= l8;
                 case 7: u7 ^= l7;
                 case 6: u6 ^= l6;
                 case 5: u5 ^= l5;
                 case 4: u4 ^= l4;
                 case 3: u3 ^= l3;
                 case 2: u2 ^= l2;
                 case 1: u1 ^= l1;
                 case 0: u0 ^= l0;
            }
            /* shift upper reg right until the rightmost bit is one */
            do {
                SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
 AlignUpper:
                /* move the comma to the right if we need to */
                switch (uc) {
                    case 8: if  (u8) break; --uc;
                    case 7: if  (u7) break; --uc;
                    case 6: if  (u6) break; --uc;
                    case 5: if  (u5) break; --uc;
                    case 4: if  (u4) break; --uc;
                    case 3: if  (u3) break; --uc;
                    case 2: if  (u2) break; --uc;
                    case 1: if  (u1) break; --uc;
                    case 0: if (!u0) return;
                            if (u0 == (uint) 1) {
                                temp[8] = l9;
                                temp[7] = l8;
                                temp[6] = l7;
                                temp[5] = l6;
                                temp[4] = l5;
                                temp[3] = l4;
                                temp[2] = l3;
                                temp[1] = l2;
                                temp[0] = l1;
                                /* use uc as the loop counter */
                                uc = lc;
                                goto doneCommasNotAligned;
                            }
                }
            } while (!(u0 & 1));
        } /* while (uc > lc) */

        /* case upper comma < lower comma */
        while (uc < lc) {

            switch (lc) { /* xor downto lc+1 */
                 case 0: u1 ^= l1;
                 case 1: u2 ^= l2;
                 case 2: u3 ^= l3;
                 case 3: u4 ^= l4;
                 case 4: u5 ^= l5;
                 case 5: u6 ^= l6;
                 case 6: u7 ^= l7;
                 case 7: u8 ^= l8;
                 case 8: u9 ^= l9;
            }
            switch (uc) { /* xor upto uc */
                 case 8: l8 ^= u8;
                 case 7: l7 ^= u7;
                 case 6: l6 ^= u6;
                 case 5: l5 ^= u5;
                 case 4: l4 ^= u4;
                 case 3: l3 ^= u3;
                 case 2: l2 ^= u2;
                 case 1: l1 ^= u1;
                 case 0: l0 ^= u0;
            }
            /* shift lower reg right until the rightmost bit is one */
            do {
                SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
 AlignLower:
                /* move the comma to the right if we need to */
                switch (lc) {
                    case 8: if  (l8) break; --lc;
                    case 7: if  (l7) break; --lc;
                    case 6: if  (l6) break; --lc;
                    case 5: if  (l5) break; --lc;
                    case 4: if  (l4) break; --lc;
                    case 3: if  (l3) break; --lc;
                    case 2: if  (l2) break; --lc;
                    case 1: if  (l1) break; --lc;
                    case 0: if (!l0) return;
                            if (l0 == (uint) 1) {
                                temp[8] = u9;
                                temp[7] = u8;
                                temp[6] = u7;
                                temp[5] = u6;
                                temp[4] = u5;
                                temp[3] = u4;
                                temp[2] = u3;
                                temp[1] = u2;
                                temp[0] = u1;
                                /* use uc as the loop counter */
                                uc = uc;
                                goto doneCommasNotAligned;
                            }
                }
            } while (!(l0 & 1));
        } /* while (uc < lc) */

    } /* main loop */
 commaU8L8:
    /* uc == lc == 8 */
    for (;;) {
        while (u8 >= l8) {
            l9^=u9;
            u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u8) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u9^=l9;
            l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l8) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u8 <= l8);
    }


 commaU7L7:
    /* uc == lc == 7 */
    for (;;) {
        while (u7 >= l7) {
            l9^=u9;  l8^=u8;
            u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u7) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;
            l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l7) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u7 <= l7);
    }


 commaU6L6:
    /* uc == lc == 6 */
    for (;;) {
        while (u6 >= l6) {
            l9^=u9;  l8^=u8;  l7^=u7;
            u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u6) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;
            l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l6) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u6 <= l6);
    }


 commaU5L5:
    /* uc == lc == 5 */
    for (;;) {
        while (u5 >= l5) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;
            u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u5) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;
            l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l5) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u5 <= l5);
    }


 commaU4L4:
    /* uc == lc == 4 */
    for (;;) {
        while (u4 >= l4) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;
            u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u4) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;
            l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l4) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u4 <= l4);
    }


 commaU3L3:
    /* uc == lc == 3 */
    for (;;) {
        while (u3 >= l3) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;
            u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u3) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;
            l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l3) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u3 <= l3);
    }


 commaU2L2:
    /* uc == lc == 2 */
    for (;;) {
        while (u2 >= l2) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;
            u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u2) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;
            l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l2) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u2 <= l2);
    }


 commaU1L1:
    /* uc == lc == 1 */
    for (;;) {
        while (u1 >= l1) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;
            u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u1) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;
            l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l1) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u1 <= l1);
    }


 commaU0L0:
    /* uc == lc == 0 */
    for (;;) {
        while (u0 >= l0) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;
            u0^=l0;
            //if (!u0) return SB_FAIL_NO_INVERSE;
            do {
                SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            } while (!(u0 & (uint) 1));
            if (u0 == (uint) 1) goto ULIsUnity;
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;
            l0^=u0;
            //if (!l0) return SB_FAIL_NO_INVERSE;
            do {
                SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            } while (!(l0 & (uint) 1));
            if (l0 == (uint) 1) goto LLIsUnity;
        } while (u0 <= l0);
    }


    /* extract almost inverse from the regs depending on the comma positions  */

 doneCommasNotAligned:
    /* it's possible for the commas to be aligned at 0 here */
    while ( uc > 0 ) {
        uc--;
        temp[uc] = 0;
    }
    goto finish;


 ULIsUnity:
    /* upper left is unity, so result is in the lower right */
    temp[8] = l9;
    temp[7] = l8;
    temp[6] = l7;
    temp[5] = l6;
    temp[4] = l5;
    temp[3] = l4;
    temp[2] = l3;
    temp[1] = l2;
    temp[0] = l1;
    goto finish;

 LLIsUnity:
    /* lower left is unity, so result is in the upper right */
    temp[8] = u9;
    temp[7] = u8;
    temp[6] = u7;
    temp[5] = u6;
    temp[4] = u5;
    temp[3] = u4;
    temp[2] = u3;
    temp[1] = u2;
    temp[0] = u1;

 finish:

    BitReverse(temp, 9); 

    /* multiply almost inverse by x^(2L-C) */
    /* first multiply and reduce in chunks of the word size */
    count = (sz << (LIMB_LGBITS_SIZE+1)) - count;
    while ( count >= LIMB_BIT_SIZE)
    {
        count -= LIMB_BIT_SIZE;
	temp[9] = temp[8];
	temp[8] = temp[7];
	temp[7] = temp[6];
	temp[6] = temp[5];
	temp[5] = temp[4];
	temp[4] = temp[3];
	temp[3] = temp[2];
	temp[2] = temp[1];
	temp[1] = temp[0];

        temp[0] = 0;
        w = temp[9];
        temp[0] ^= (w << 5) ^ (w << 7) ^ (w << 10) ^ (w << 15);
        temp[1] ^= (w >> 59) ^ (w >> 57) ^ (w >> 54) ^ (w >> 49);
    }

    /* multiply by the remaining multiplier */
    temp[9] = 0;
for (i=count; i>0; i--) {
    SHIFT_LEFT_10(temp[9],temp[8],temp[7],temp[6],temp[5],temp[4],temp[3],temp[2],temp[1],temp[0]);
}
        w = temp[9];
        temp[0] ^= (w << 5) ^ (w << 7) ^ (w << 10) ^ (w << 15);
        temp[1] ^= (w >> 59) ^ (w >> 57) ^ (w >> 54) ^ (w >> 49);

    temp[9] = 0;
    temp[10] = 0;
    temp[11] = 0;
    temp[12] = 0;
    temp[13] = 0;
    temp[14] = 0;
    temp[15] = 0;
    temp[16] = 0;
    temp[17] = 0;

    PolyModT571(rop, temp);
    return; 
}
#else
{
    uint temp[36] = {(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0,(uint)0}; /* temp storage */
    int uc = 17;      /* upper comma. */
    int lc = 17;      /* lower comma. */
    int count = 0;
    int i;
    uint w;

    /* upper register (merged) */
    uint u18 = (uint)0, u17 = (uint)0, u16 = (uint)0, u15 = (uint)0, u14 = (uint)0, u13 = (uint)0, u12 = (uint)0, u11 = (uint)0, u10 = (uint)0, u9 = (uint)0, u8 = (uint)0, u7 = (uint)0, u6 = (uint)0, u5 = (uint)0, u4 = (uint)0, u3 = (uint)0, u2 = (uint)0, u1 = (uint)0, u0 = (uint)0;

    /* lower register (merged) */
    uint l18 = (uint)0, l17 = (uint)0, l16 = (uint)0, l15 = (uint)0, l14 = (uint)0, l13 = (uint)0, l12 = (uint)0, l11 = (uint)0, l10 = (uint)0, l9 = (uint)0, l8 = (uint)0, l7 = (uint)0, l6 = (uint)0, l5 = (uint)0, l4 = (uint)0, l3 = (uint)0, l2 = (uint)0, l1 = (uint)0, l0 = (uint)0;

    /* multiply element by x^2L */
    temp[sz+17] = op[17];
    temp[sz+16] = op[16];
    temp[sz+15] = op[15];
    temp[sz+14] = op[14];
    temp[sz+13] = op[13];
    temp[sz+12] = op[12];
    temp[sz+11] = op[11];
    temp[sz+10] = op[10];
    temp[sz+9]  = op[9];
    temp[sz+8]  = op[8];
    temp[sz+7]  = op[7];
    temp[sz+6]  = op[6];
    temp[sz+5]  = op[5];
    temp[sz+4]  = op[4];
    temp[sz+3]  = op[3];
    temp[sz+2]  = op[2];
    temp[sz+1]  = op[1];
    temp[sz]    = op[0];

    PolyModT571(temp+sz, temp);

    temp[17] = 0;
    temp[16] = 0;
    temp[15] = 0;
    temp[14] = 0;
    temp[13] = 0;
    temp[12] = 0;
    temp[11] = 0;
    temp[10] = 0;
    temp[9]  = 0;
    temp[8]  = 0;
    temp[7]  = 0;
    temp[6]  = 0;
    temp[5]  = 0;
    temp[4]  = 0;
    temp[3]  = 0;
    temp[2]  = 0;
    temp[1]  = 0;
    temp[0]  = 0;

    PolyModT571(temp, temp);

    /* Final reduce and check to see if the element is zero */
    if ( IsInt(temp, sz, 0) ) {
        return;
    }

    u18 = ((uint)1) << (LIMB_BIT_SIZE - 1);
    l18 = 0;
    /* set the modulus */
    u17 = fx[17]; l17 = temp[17];
    u16 = fx[16]; l16 = temp[16];
    u15 = fx[15]; l15 = temp[15];
    u14 = fx[14]; l14 = temp[14];
    u13 = fx[13]; l13 = temp[13];
    u12 = fx[12]; l12 = temp[12];
    u11 = fx[11]; l11 = temp[11];
    u10 = fx[10]; l10 = temp[10];
    u9 = fx[9]; l9 = temp[9];
    u8 = fx[8]; l8 = temp[8];
    u7 = fx[7]; l7 = temp[7];
    u6 = fx[6]; l6 = temp[6];
    u5 = fx[5]; l5 = temp[5];
    u4 = fx[4]; l4 = temp[4];
    u3 = fx[3]; l3 = temp[3];
    u2 = fx[2]; l2 = temp[2];
    u1 = fx[1]; l1 = temp[1];
    u0 = fx[0]; l0 = temp[0];


    /* We know that the irreducible has a 1 in position 0.
     * Jump into the code that shifts a 1 into position 0 of the lower reg.
     */
    goto AlignLower;

    /* main loop */
    /* select an inner loop based upon the comma positions */
    for (;;)
    {
        /* case upper comma == lower comma */
        if (uc == lc) {
            switch (uc) { /* jump to a specific optimized loop */
                case 17: goto commaU17L17;
                case 16: goto commaU16L16;
                case 15: goto commaU15L15;
                case 14: goto commaU14L14;
                case 13: goto commaU13L13;
                case 12: goto commaU12L12;
                case 11: goto commaU11L11;
                case 10: goto commaU10L10;
                case 9: goto commaU9L9;
                case 8: goto commaU8L8;
                case 7: goto commaU7L7;
                case 6: goto commaU6L6;
                case 5: goto commaU5L5;
                case 4: goto commaU4L4;
                case 3: goto commaU3L3;
                case 2: goto commaU2L2;
                case 1: goto commaU1L1;
                case 0: goto commaU0L0;
            }
        }

        /* case upper comma > lower comma */
        while (uc > lc) {

            switch (uc) { /* xor downto uc+1 */
                 case 0: l1 ^= u1;
                 case 1: l2 ^= u2;
                 case 2: l3 ^= u3;
                 case 3: l4 ^= u4;
                 case 4: l5 ^= u5;
                 case 5: l6 ^= u6;
                 case 6: l7 ^= u7;
                 case 7: l8 ^= u8;
                 case 8: l9 ^= u9;
                 case 9: l10 ^= u10;
                 case 10: l11 ^= u11;
                 case 11: l12 ^= u12;
                 case 12: l13 ^= u13;
                 case 13: l14 ^= u14;
                 case 14: l15 ^= u15;
                 case 15: l16 ^= u16;
                 case 16: l17 ^= u17;
                 case 17: l18 ^= u18;
            }
            switch (lc) { /* xor upto lc */
                 case 17: u17 ^= l17;
                 case 16: u16 ^= l16;
                 case 15: u15 ^= l15;
                 case 14: u14 ^= l14;
                 case 13: u13 ^= l13;
                 case 12: u12 ^= l12;
                 case 11: u11 ^= l11;
                 case 10: u10 ^= l10;
                 case 9: u9 ^= l9;
                 case 8: u8 ^= l8;
                 case 7: u7 ^= l7;
                 case 6: u6 ^= l6;
                 case 5: u5 ^= l5;
                 case 4: u4 ^= l4;
                 case 3: u3 ^= l3;
                 case 2: u2 ^= l2;
                 case 1: u1 ^= l1;
                 case 0: u0 ^= l0;
            }
            /* shift upper reg right until the rightmost bit is one */
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
 AlignUpper:
                /* move the comma to the right if we need to */
                switch (uc) {
                    case 17: if  (u17) break; --uc;
                    case 16: if  (u16) break; --uc;
                    case 15: if  (u15) break; --uc;
                    case 14: if  (u14) break; --uc;
                    case 13: if  (u13) break; --uc;
                    case 12: if  (u12) break; --uc;
                    case 11: if  (u11) break; --uc;
                    case 10: if  (u10) break; --uc;
                    case 9: if  (u9) break; --uc;
                    case 8: if  (u8) break; --uc;
                    case 7: if  (u7) break; --uc;
                    case 6: if  (u6) break; --uc;
                    case 5: if  (u5) break; --uc;
                    case 4: if  (u4) break; --uc;
                    case 3: if  (u3) break; --uc;
                    case 2: if  (u2) break; --uc;
                    case 1: if  (u1) break; --uc;
                    case 0: if (!u0) return;
                            if (u0 == (uint) 1) {
                                temp[17] = l18;
                                temp[16] = l17;
                                temp[15] = l16;
                                temp[14] = l15;
                                temp[13] = l14;
                                temp[12] = l13;
                                temp[11] = l12;
                                temp[10] = l11;
                                temp[9] = l10;
                                temp[8] = l9;
                                temp[7] = l8;
                                temp[6] = l7;
                                temp[5] = l6;
                                temp[4] = l5;
                                temp[3] = l4;
                                temp[2] = l3;
                                temp[1] = l2;
                                temp[0] = l1;
                                /* use uc as the loop counter */
                                uc = lc;
                                goto doneCommasNotAligned;
                            }
                }
            } while (!(u0 & 1));
        } /* while (uc > lc) */

        /* case upper comma < lower comma */
        while (uc < lc) {

            switch (lc) { /* xor downto lc+1 */
                 case 0: u1 ^= l1;
                 case 1: u2 ^= l2;
                 case 2: u3 ^= l3;
                 case 3: u4 ^= l4;
                 case 4: u5 ^= l5;
                 case 5: u6 ^= l6;
                 case 6: u7 ^= l7;
                 case 7: u8 ^= l8;
                 case 8: u9 ^= l9;
                 case 9: u10 ^= l10;
                 case 10: u11 ^= l11;
                 case 11: u12 ^= l12;
                 case 12: u13 ^= l13;
                 case 13: u14 ^= l14;
                 case 14: u15 ^= l15;
                 case 15: u16 ^= l16;
                 case 16: u17 ^= l17;
                 case 17: u18 ^= l18;
            }
            switch (uc) { /* xor upto uc */
                 case 17: l17 ^= u17;
                 case 16: l16 ^= u16;
                 case 15: l15 ^= u15;
                 case 14: l14 ^= u14;
                 case 13: l13 ^= u13;
                 case 12: l12 ^= u12;
                 case 11: l11 ^= u11;
                 case 10: l10 ^= u10;
                 case 9: l9 ^= u9;
                 case 8: l8 ^= u8;
                 case 7: l7 ^= u7;
                 case 6: l6 ^= u6;
                 case 5: l5 ^= u5;
                 case 4: l4 ^= u4;
                 case 3: l3 ^= u3;
                 case 2: l2 ^= u2;
                 case 1: l1 ^= u1;
                 case 0: l0 ^= u0;
            }
            /* shift lower reg right until the rightmost bit is one */
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
 AlignLower:
                /* move the comma to the right if we need to */
                switch (lc) {
                    case 17: if  (l17) break; --lc;
                    case 16: if  (l16) break; --lc;
                    case 15: if  (l15) break; --lc;
                    case 14: if  (l14) break; --lc;
                    case 13: if  (l13) break; --lc;
                    case 12: if  (l12) break; --lc;
                    case 11: if  (l11) break; --lc;
                    case 10: if  (l10) break; --lc;
                    case 9: if  (l9) break; --lc;
                    case 8: if  (l8) break; --lc;
                    case 7: if  (l7) break; --lc;
                    case 6: if  (l6) break; --lc;
                    case 5: if  (l5) break; --lc;
                    case 4: if  (l4) break; --lc;
                    case 3: if  (l3) break; --lc;
                    case 2: if  (l2) break; --lc;
                    case 1: if  (l1) break; --lc;
                    case 0: if (!l0) return;
                            if (l0 == (uint) 1) {
                                temp[17] = u18;
                                temp[16] = u17;
                                temp[15] = u16;
                                temp[14] = u15;
                                temp[13] = u14;
                                temp[12] = u13;
                                temp[11] = u12;
                                temp[10] = u11;
                                temp[9] = u10;
                                temp[8] = u9;
                                temp[7] = u8;
                                temp[6] = u7;
                                temp[5] = u6;
                                temp[4] = u5;
                                temp[3] = u4;
                                temp[2] = u3;
                                temp[1] = u2;
                                temp[0] = u1;
                                /* use uc as the loop counter */
                                uc = uc;
                                goto doneCommasNotAligned;
                            }
                }
            } while (!(l0 & 1));
        } /* while (uc < lc) */

    } /* main loop */
 commaU17L17:
    /* uc == lc == 17 */
    for (;;) {
        while (u17 >= l17) {
            l18^=u18;
            u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u17) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;
            l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l17) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u17 <= l17);
    }


 commaU16L16:
    /* uc == lc == 16 */
    for (;;) {
        while (u16 >= l16) {
            l18^=u18;  l17^=u17;
            u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u16) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;
            l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l16) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u16 <= l16);
    }


 commaU15L15:
    /* uc == lc == 15 */
    for (;;) {
        while (u15 >= l15) {
            l18^=u18;  l17^=u17;  l16^=u16;
            u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u15) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;
            l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l15) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u15 <= l15);
    }


 commaU14L14:
    /* uc == lc == 14 */
    for (;;) {
        while (u14 >= l14) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;
            u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u14) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;
            l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l14) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u14 <= l14);
    }


 commaU13L13:
    /* uc == lc == 13 */
    for (;;) {
        while (u13 >= l13) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;
            u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u13) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;
            l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l13) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u13 <= l13);
    }


 commaU12L12:
    /* uc == lc == 12 */
    for (;;) {
        while (u12 >= l12) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;
            u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u12) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;
            l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l12) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u12 <= l12);
    }


 commaU11L11:
    /* uc == lc == 11 */
    for (;;) {
        while (u11 >= l11) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;
            u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u11) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;
            l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l11) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u11 <= l11);
    }


 commaU10L10:
    /* uc == lc == 10 */
    for (;;) {
        while (u10 >= l10) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;
            u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u10) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;
            l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l10) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u10 <= l10);
    }


 commaU9L9:
    /* uc == lc == 9 */
    for (;;) {
        while (u9 >= l9) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u9) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l9) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u9 <= l9);
    }


 commaU8L8:
    /* uc == lc == 8 */
    for (;;) {
        while (u8 >= l8) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;
            u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u8) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;
            l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l8) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u8 <= l8);
    }


 commaU7L7:
    /* uc == lc == 7 */
    for (;;) {
        while (u7 >= l7) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;
            u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u7) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;
            l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l7) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u7 <= l7);
    }


 commaU6L6:
    /* uc == lc == 6 */
    for (;;) {
        while (u6 >= l6) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;
            u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u6) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;
            l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l6) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u6 <= l6);
    }


 commaU5L5:
    /* uc == lc == 5 */
    for (;;) {
        while (u5 >= l5) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;
            u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u5) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;
            l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l5) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u5 <= l5);
    }


 commaU4L4:
    /* uc == lc == 4 */
    for (;;) {
        while (u4 >= l4) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;
            u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u4) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;
            l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l4) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u4 <= l4);
    }


 commaU3L3:
    /* uc == lc == 3 */
    for (;;) {
        while (u3 >= l3) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;
            u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u3) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;
            l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l3) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u3 <= l3);
    }


 commaU2L2:
    /* uc == lc == 2 */
    for (;;) {
        while (u2 >= l2) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;
            u2^=l2;  u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u2) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;
            l2^=u2;  l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l2) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u2 <= l2);
    }


 commaU1L1:
    /* uc == lc == 1 */
    for (;;) {
        while (u1 >= l1) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;
            u1^=l1;  u0^=l0;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u1) { --uc; goto AlignUpper; }
            } while (!(u0 & (uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;
            l1^=u1;  l0^=u0;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l1) { --lc; goto AlignLower; }
            } while (!(l0 & (uint) 1));
        } while (u1 <= l1);
    }


 commaU0L0:
    /* uc == lc == 0 */
    for (;;) {
        while (u0 >= l0) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;
            u0^=l0;
            //if (!u0) return SB_FAIL_NO_INVERSE;
            do {
                SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            } while (!(u0 & (uint) 1));
            if (u0 == (uint) 1) goto ULIsUnity;
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;
            l0^=u0;
            //if (!l0) return SB_FAIL_NO_INVERSE;
            do {
                SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            } while (!(l0 & (uint) 1));
            if (l0 == (uint) 1) goto LLIsUnity;
        } while (u0 <= l0);
    }


    /* extract almost inverse from the regs depending on the comma positions  */

 doneCommasNotAligned:
    /* it's possible for the commas to be aligned at 0 here */
    while ( uc > 0 ) {
        uc--;
        temp[uc] = 0;
    }
    goto finish;


 ULIsUnity:
    /* upper left is unity, so result is in the lower right */
    temp[17] = l18;
    temp[16] = l17;
    temp[15] = l16;
    temp[14] = l15;
    temp[13] = l14;
    temp[12] = l13;
    temp[11] = l12;
    temp[10] = l11;
    temp[9] = l10;
    temp[8] = l9;
    temp[7] = l8;
    temp[6] = l7;
    temp[5] = l6;
    temp[4] = l5;
    temp[3] = l4;
    temp[2] = l3;
    temp[1] = l2;
    temp[0] = l1;
    goto finish;

 LLIsUnity:
    /* lower left is unity, so result is in the upper right */
    temp[17] = u18;
    temp[16] = u17;
    temp[15] = u16;
    temp[14] = u15;
    temp[13] = u14;
    temp[12] = u13;
    temp[11] = u12;
    temp[10] = u11;
    temp[9] = u10;
    temp[8] = u9;
    temp[7] = u8;
    temp[6] = u7;
    temp[5] = u6;
    temp[4] = u5;
    temp[3] = u4;
    temp[2] = u3;
    temp[1] = u2;
    temp[0] = u1;

 finish:

    BitReverse(temp, 18);

    /* multiply almost inverse by x^(2L-C) */
    /* first multiply and reduce in chunks of the word size */
    count = (sz << (LIMB_LGBITS_SIZE+1)) - count;
    while ( count >= LIMB_BIT_SIZE )
    {
        count -= LIMB_BIT_SIZE;
        temp[19] = temp[18];
        temp[18] = temp[17];
        temp[17] = temp[16];
        temp[16] = temp[15];
        temp[15] = temp[14];
        temp[14] = temp[13];
        temp[13] = temp[12];
        temp[12] = temp[11];
        temp[11] = temp[10];
        temp[10] = temp[9];
        temp[9] = temp[8];
        temp[8] = temp[7];
        temp[7] = temp[6];
        temp[6] = temp[5];
        temp[5] = temp[4];
        temp[4] = temp[3];
        temp[3] = temp[2];
        temp[2] = temp[1];
        temp[1] = temp[0];

        temp[0] = 0;
        w = temp[18]; 
        temp[0] ^= (w << 5) ^ (w << 7) ^ (w << 10) ^ (w << 15);
        temp[1] ^= (w >> 27) ^ (w >> 25) ^ (w >> 22) ^ (w >> 17);
    }

    /* multiply by the remaining multiplier */
    temp[18] = 0;
for (i=count; i>0; i--)  {
    SHIFT_LEFT_19(temp[18],temp[17],temp[16],temp[15],temp[14],temp[13],temp[12],temp[11],temp[10],temp[9],temp[8],temp[7],temp[6],temp[5],temp[4],temp[3],temp[2],temp[1],temp[0]);
}

        w = temp[18]; 
        temp[0] ^= (w << 5) ^ (w << 7) ^ (w << 10) ^ (w << 15);
        temp[1] ^= (w >> 27) ^ (w >> 25) ^ (w >> 22) ^ (w >> 17);

        temp[18] = 0;
        temp[19] = 0;
        temp[20] = 0;
        temp[21] = 0;
        temp[22] = 0;
        temp[23] = 0;
        temp[24] = 0;
        temp[25] = 0;
        temp[26] = 0;
        temp[27] = 0;
        temp[28] = 0;
        temp[29] = 0;
        temp[30] = 0;
        temp[31] = 0;
        temp[32] = 0;
        temp[33] = 0;
        temp[34] = 0;
        temp[35] = 0;
        PolyModT571(rop,temp);

    return;
}
#endif 
/*
  	END OF f_{2^m} FUNCTIONS
  
 *****************************************************************************/

/*****************************************************************************
  
  	START OF ECC FUNCTIONS
 
 *****************************************************************************
  
  	PointDouble()			This function doubles a point represented in
  							polynomial basis on a curve 
  
  							See [1] Group Laws for non-super singular curves
  							p. 81, Point Doubling
  
 *****************************************************************************/
void PointDouble(point *rop, point *op, curve *T)
{
	uint	l[2*MAX_SIZE], l2[2*MAX_SIZE], px[MAX_SIZE];
	
	if(op->x[0]==0){
		if(IsInt(op->x, T->sz, 0)){
			memset(rop->x, 0, sizeof(uint)*T->sz);
			memset(rop->y, 0, sizeof(uint)*T->sz);
			return;
		}
	}
	PolyInv(l, op->x, T->f, T->sz);			//	l = x^-1
	PolyMulMod(l, l, op->y, T->sz);			//	l = y/x in f2m
	PolyAdd(l, l, op->x, T->sz);			//	l = x + y/x in f2m
	PolySqrMod(l2, l, T->sz);				//	l2 = l^2 in f2m
	l[0]^=T->a;								//	l = l + 1 in f2m
	PolyAdd(px, l2, l, T->sz);				//	px = l^2 + l + a in f2m 
	PolySqrMod(l2, op->x, T->sz);			//	l2 = x^2
	PolyMulMod(l, l, px, T->sz);			//	l = px(l + 1) 
	PolyAdd(rop->y, l2, l, T->sz);			//	py = x^2 + px(l + 1) in f2m
	memcpy(rop->x, px, sizeof(uint)*T->sz);
}
/*****************************************************************************

  	PointAdd()				add one point to another on the curve 
							See [1] Group Laws for non-super singular curves
							p. 81, Point Addition


 *****************************************************************************/
void PointAdd(point *rop, point *op1, point *op2, curve *T)
{
	uint	l[2*MAX_SIZE], l2[2*MAX_SIZE], px[MAX_SIZE], py[MAX_SIZE];
	uint	op1x0, op1y0;

	op1x0 = IsInt(op1->x, T->sz, 0);	op1y0 = IsInt(op1->y, T->sz, 0);
	if(op1x0 && op1y0)	{
		memcpy(rop->x, op2->x, sizeof(uint)*T->sz);
		memcpy(rop->y, op2->y, sizeof(uint)*T->sz);
		return;
	}
	op1x0 = IsInt(op2->x, T->sz, 0);	op1y0 = IsInt(op2->y, T->sz, 0);
	if(op1x0 && op1y0)	{
		memcpy(rop->x, op1->x, sizeof(uint)*T->sz);
		memcpy(rop->y, op1->y, sizeof(uint)*T->sz);
		return;
	}
	if(memcmp(op1->x, op2->x, sizeof(uint)*T->sz)==0){
		if(memcmp(op1->y, op2->y, sizeof(uint)*T->sz)==0){
			PointDouble(rop, op1, T);		return;
		}
		else{
			memset(rop->x, 0, sizeof(uint)*T->sz);
			memset(rop->y, 0, sizeof(uint)*T->sz);
			return;
		}
	}
	PolyAdd(l2, op1->x, op2->x, T->sz);		//	l2 = x1 + x2
	PolyInv(l, l2, T->f, T->sz);			//	l = 1/(x1 + x2)
	PolyAdd(l2, op1->y, op2->y, T->sz);		//	l2 = y1 + y2
	PolyMulMod(l, l, l2, T->sz);			//	l = (y1 + y2)/(x1 + x2)
	PolySqrMod(l2, l, T->sz);				//	l2 = l^2
	PolyAdd(px, l2, l, T->sz);				//	px = l^2 + l in f2m
	PolyAdd(px, px, op1->x, T->sz);			//	px = l^2 + l + x1 in f2m
	PolyAdd(px, px, op2->x, T->sz);			//	px = l^2 + l + x1 + x2 in f2m
	px[0]^=T->a;							//	px = l^2 + l + x1 + x2 + a in f2m
	PolyAdd(l2, px, op1->x, T->sz);			//	l2 = x3 + x1 in f2m
	PolyMulMod(l2, l2, l, T->sz);			//	l2 = l(x3 + x1)
	PolyAdd(py, l2, px, T->sz);				//	py = l(x3 + x1) + x3 in f2m
	PolyAdd(py, py, op1->y, T->sz);			//	py = l(x3 + x1) + x3 + y1 in f2m
	memcpy(rop->x, px, sizeof(uint)*T->sz);	
	memcpy(rop->y, py, sizeof(uint)*T->sz);
}
/*****************************************************************************

  	BasePointMultiply()			This function performs a base point scalar
  								multiply.

  								See [1], Algorithm 3.41, p. 104.


 *****************************************************************************/
void	BasePointMultiply(point *P, uint sz, uint *k, curve *T)
{
	uint	i, j, n;
	point	A, B;

	n = GetBitLength(sz, k);
	n = (n + 3) >> 2;
	memset(A.x, 0, MAX_SIZE*sizeof(uint));	memset(A.y, 0, MAX_SIZE*sizeof(uint));
	memset(B.x, 0, MAX_SIZE*sizeof(uint));	memset(B.y, 0, MAX_SIZE*sizeof(uint));

	for(j=15;j>0;j--){
		//
		//	compute Q_j = \sum_{i:Ki = j}, where 
		//		K_i is the ith part base 2^4.
		//
		for(i=0;i<n;i++){

			if(((k[i/LIMB_NIB_LENGTH]&NIB_MASK[i%LIMB_NIB_LENGTH])>>(4*(i%LIMB_NIB_LENGTH)))==j){
				PointAdd(&B, &B, &T->G[i], T);			
			}
		}
		PointAdd(&A, &A, &B, T);							
	}
	memcpy(P->x, A.x, sizeof(uint)*T->sz);	memcpy(P->y, A.y, sizeof(uint)*T->sz);
	return;
}
/*****************************************************************************

  	SolveQuadratic()			Solves the quadratic equation for z

  								z^2 + z = op

  								Uses the basic version algorithm in [1], 
  								Algorithm 3.85, p.133

  	RESTRICTIONS:				trace(op) = 0
  								op gets destroyed

 *****************************************************************************/
void SolveQuadratic(uint *rop, uint *op, uint sz)
{
	uint			i, g[MAX_SIZE], b, qsz;
	/*************************************************************************/
	memset(g, 0, sizeof(uint)*sz);
	if(sz==SECT283_SIZE)	{	qsz = 141;	}
	else if(sz==SECT409_SIZE)	{	qsz = 204;	}
	else						{	qsz = 285;	}
	for(i=qsz;i>0;i--){
		if(GETBIT(op, 2*i)){
			b = BIT[i%LIMB_BIT_SIZE];
			op[i/LIMB_BIT_SIZE]^= b;
			g[i/LIMB_BIT_SIZE]^= b;
		}
	}
	for(i=0;i<qsz;i++){
		if(GETBIT(op, 2*i + 1)){	
			switch(sz)
			{
			case SECT283_SIZE:	PolyAdd(g, g, HT283[i], sz);	break;
			case SECT409_SIZE:	PolyAdd(g, g, HT409[i], sz);	break;
			case SECT571_SIZE:	PolyAdd(g, g, HT571[i], sz);	break;
			default:	break;
			}
		}
	}
	memcpy(rop, g, sizeof(uint)*sz);
}
/*****************************************************************************

  	PointDecompress	This function attempts to decompress a point.

  	it uses a hard coded (top bit of the 8th limb (index 7)) to determine which
  	point on the curve it is mapped to.

  	it assumes the x-coordinate of the passed point structure has trace = 1.
  	trace(rop->x)=1;

  	See [2], Octet-String-to-Elliptic-Curve-Point Conversion, \S 2.3.4, p.11.

 *****************************************************************************/
uint	PointDecompress(point *rop, curve *T)
{
	uint	yp, a[2*MAX_SIZE], b[MAX_SIZE], loop = 1;
	switch(T->sz)
	{
	case SECT283_SIZE:
		yp = GETBIT(rop->x, YP_BIT_T283);
		break;
	case SECT409_SIZE:
		yp = GETBIT(rop->x, YP_BIT_T409);
		break;
	case SECT571_SIZE:
		yp = GETBIT(rop->x, YP_BIT_T571);
		break;
	default:
		break;
	}
	/*************************************************************************

	  	Verify we are not decompressing the 0 value (should never happen)		

	 *************************************************************************/
	if(rop->x[0]==0){ if(IsInt(rop->x, T->sz, 0)){	return -1;	}	}
	/*************************************************************************

	  	compute a = x + T->a + T->b*x^{-2} in F_{2^m}

	 *************************************************************************/
	PolyInv(b, rop->x, T->f, T->sz);			//	a = x^-1
	PolySqrMod(a, b, T->sz);					//	b = x^-2
	PolyMulMod(a, a, T->b, T->sz);				//	a = T->b*(x^-2)
	switch(T->sz)
	{
	case SECT283_SIZE:
		if(TRACET283(a))	return -1;
		break;
	case SECT409_SIZE:
		if(TRACET409(a))	return -1;
		break;
	case SECT571_SIZE:
		if(TRACET571(a))	return -1;
		break;
	default:
		break;
	}
	PolyAdd(a, a, rop->x, T->sz);			//	a = x + T->b*(x^-2)
	a[0]^=T->a;								//	a = x + T->a + T->b*(x^-2)
	SolveQuadratic(b, a, T->sz);			//	solve quadratic z^2 + z = a
	if(yp!=(b[0]&0x01))	{		b[0]^=0x01;	}
	PolyMulMod(a, b, rop->x, T->sz);
	memcpy(rop->y, a, sizeof(uint)*T->sz);
	return 0;
}
/*
  	END OF ECC FUNCTIONS
  
 *****************************************************************************/

/*****************************************************************************

  	HashReturn Init(hashState *state, int hashbitlen);

  		Initializes a hashState with the intended hash length of this particular 
  		instantiation.  Additionally, any data independent setup is performed.

  	Parameters:

  		state:		a structure that holds the hashState information

  		hashbitlen: an integer value that indicates the length of the hash 
  					output in bits.

  	Returns:

  		SUCCESS				on success

  		BAD_HASHBITLEN		hashbitlen is invalid (e.g., unknown value)

 *****************************************************************************/
HashReturn Init(hashState *state, int hashbitlen)
{
	state->blen = 0;

	state->counter[0] = 0;	state->counter[1] = 0;
	state->mlen[0] = 0;		state->mlen[1] = 0;
	switch(hashbitlen)
	{
	case ECOH224:
		state->type = ECOH224;
		state->DATASIZE = ECOH224_DATASIZE;
		state->DIGESTSIZE = ECOH224_DIGESTSIZE;
		state->clen = ECOH224_CLEN;
		state->T = &sect283r1;
		break;
	case ECOH256:
		state->type = ECOH256;
		state->DATASIZE = ECOH256_DATASIZE;
		state->DIGESTSIZE = ECOH256_DIGESTSIZE;
		state->clen = ECOH256_CLEN;
		state->T = &sect283r1;
		break;
	case ECOH384:
		state->type = ECOH384;
		state->DATASIZE = ECOH384_DATASIZE;
		state->DIGESTSIZE = ECOH384_DIGESTSIZE;
		state->clen = ECOH384_CLEN;
		state->T = &sect409r1;
		break;
	case ECOH512:
		state->type = ECOH512;
		state->DATASIZE = ECOH512_DATASIZE;
		state->DIGESTSIZE = ECOH512_DIGESTSIZE;
		state->clen = ECOH512_CLEN;
		state->T = &sect571r1;
		break;
	default:
		return BAD_HASHBITLEN;
	}
	memset(state->N, 0, MAX_DATASIZE*LIMB_SIZE);
	memset(state->Q.x, 0, MAX_SIZE*LIMB_SIZE);
	memset(state->Q.y, 0, MAX_SIZE*LIMB_SIZE);
	memset(state->P.x, 0, MAX_SIZE*LIMB_SIZE);
	memset(state->P.y, 0, MAX_SIZE*LIMB_SIZE);
	return SUCCESS;
}
/*****************************************************************************

  	_Transform()		internal hashing of the data

 *****************************************************************************/
uint	_Transform(hashState *state)
{
	SET(state->P.x, 1, state->clen);
	SETCTR(&state->P.x[state->clen], state->counter, state->clen);
	while(PointDecompress(&state->P, state->T))	{
		ADD(state->P.x, 2, state->clen);
	}
	/*************************************************************************

	  	Q = Q + P_i, up counter, zeroize blen

	 *************************************************************************/
	PointAdd(&state->Q, &state->Q, &state->P, state->T);
	INCCTR(state->counter, state->clen);
	state->blen = 0;
	return SUCCESS;
}
/*****************************************************************************

  	Update()

  	HashReturn Update(	hashState *state, const BitSequence *data, DataLength 
  						databitlen);

  	Process the supplied data.

  	Parameters:

  		state:		a structure that holds the hashState information

  		data:		the data to be hashed

  		databitlen: the length, in bits, of the data to be hashed

  	Returns:
  
  		SUCCESS			on success
  	
  		FAIL			on failure
  	
  		BAD_HASHBITLEN	on bad hash bit length

 *****************************************************************************/
HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen)
{
	DataLength	free, used;
	uint		datastart;
	uchar		*ptr;

	if(databitlen&0x7)			{	return BAD_HASHBITLEN;	}
	if(state->type == ECOH512)	{	datastart = DATASTART*2;	}
	else						{	datastart = DATASTART;		}
	/*************************************************************************

	  	add in the bit count

	 *************************************************************************/
	state->mlen[0]+=(databitlen);
	if(state->mlen[0]<databitlen)	if((++state->mlen[1])==0)	return BAD_HASHBITLEN;	
	databitlen>>=3;
	used = state->blen;
	ptr = ((unsigned char*)(&state->P.x[datastart])) + used;
	/*************************************************************************

	  	handle the case where partial data is in the buffer

	 *************************************************************************/
	if(used){
		free = state->DATASIZE - used;
		if(databitlen>=free){
			memcpy(ptr, data, (uint)free);
			databitlen-=free;
			databitlen+=free;
			FormatData(&state->P.x[datastart], (uint)state->DATASIZE/sizeof(uint));
			PolyAdd(state->N, state->N, &state->P.x[datastart], state->DATASIZE/sizeof(uint));
			if(_Transform(state))	return FAIL;
		}
		else{
			memcpy(ptr, data, (uint)databitlen);
			state->blen += (uint)databitlen;
			return SUCCESS;
		}
	}
	/*************************************************************************

	  	loop over the rest of the data, transforming every ECOH_DATASIZE

	 *************************************************************************/
	while(databitlen>=state->DATASIZE){
		memcpy(&state->P.x[datastart], data, (uint)state->DATASIZE);
		FormatData(&state->P.x[datastart], (uint)state->DATASIZE/sizeof(uint));
		PolyAdd(state->N, state->N, &state->P.x[datastart], state->DATASIZE/sizeof(uint));
		if(_Transform(state))	return FAIL;
		databitlen-=state->DATASIZE;
		data+=state->DATASIZE;
	}
	/*************************************************************************

	  	Load any remaining data

	 *************************************************************************/
	if(databitlen>0){
		memcpy(&state->P.x[datastart], data, (uint)databitlen);
		state->blen = (uint)databitlen;
	}
	return SUCCESS;
}
/*****************************************************************************

  	Final()

  		HashReturn Final(hashState *state, BitSequence *hashval);

  		Perform any post processing and output filtering required and return 
  		the final hash value.

  	Parameters:

  		state:		a structure that holds the hashState information

  		hashval:	the storage for the final hash value to be returned

  	Returns:

  		SUCCESS		on success

  		FAIL		otherwise

 *****************************************************************************/
HashReturn Final(hashState *state, BitSequence *hashval)
{
    uint		cnt, datastart;
	uchar	    *ptr;

	if(state->type == ECOH512)	{	datastart = DATASTART*2;	}
	else						{	datastart = DATASTART;		}
	
	cnt = state->blen;
	ptr = ((unsigned char*)(&state->P.x[datastart])) + cnt;
    
    *ptr++ = 0x80;

    cnt = state->DATASIZE - 1 - cnt;

    memset(ptr, 0, cnt );
    FormatData(&state->P.x[datastart], (uint)state->DATASIZE/sizeof(uint));
	PolyAdd(state->N, state->N, &state->P.x[datastart], state->DATASIZE/sizeof(uint));
	if(_Transform(state))	return FAIL;
    
	memcpy(&state->P.x[datastart], state->N, state->DATASIZE);

	SET(state->P.x, 1, state->clen);
	SETCTR(&state->P.x[state->clen], state->mlen, state->clen);

	while(PointDecompress(&state->P, state->T))	{
		ADD(state->P.x, 2, state->clen);
	}
	/*************************************************************************

	  	Q = Q + P, up counter, zeroize blen

	 *************************************************************************/
	PointAdd(&state->Q, &state->Q, &state->P, state->T);
	/*************************************************************************

	  	Q = Q + [Q.x/2]G where[Q.x/2] interpreted as an integer and divided by
	  	2, and G is the base point

	 *************************************************************************/
	PolyShiftRight(state->P.x, state->Q.x, state->T->sz);
	BasePointMultiply(&state->P, state->T->sz, state->P.x, state->T);
	PointAdd(&state->Q, &state->Q, &state->P, state->T);
	PolyShiftRight(state->Q.x, state->Q.x, state->T->sz);
	Unload(hashval, state->Q.x, state->DIGESTSIZE);
	return SUCCESS;
}
/*****************************************************************************
  	Hash()

  		HashReturn Hash(int hashbitlen, const BitSequence *data,  DataLength 
  							databitlen, BitSequence *hashval);


  		Hash the supplied data and provide the resulting hash value. Set return 
  		code as appropriate.

  	Parameters:

  		hashbitlen:			the length in bits of the desired hash value

  		data:				the data to be hashed

  		databitlen:			the length, in bits, of the data to be hashed

  		hashval:			the resulting hash value of the provided data

  	Returns:

  		SUCCESS				on success

  		FAIL				arbitrary failure

  		BAD_HASHBITLEN		unknown hashbitlen requested

 *****************************************************************************/
HashReturn Hash(int hashbitlen, const BitSequence *data,  DataLength databitlen, BitSequence *hashval)
{
	hashState	state;
	HashReturn	rval;
	DataLength	blen, dblen, cnt;
	uchar		*ptr;
	uint		datastart;

	rval = Init(&state, hashbitlen);
	if(rval)	{	return rval;	}
	
	if(state.type == ECOH512)	{	datastart = DATASTART*2;	}
	else						{	datastart = DATASTART;		}
	/*************************************************************************

	  	set mlen, anf the byte length blen, and the databitlength remainder

	 *************************************************************************/
	state.mlen[0]=(databitlen);
	blen = (databitlen>>3);
	dblen = (databitlen&7);
	/*************************************************************************

	  	loop over the rest of the data, transforming every ECOH_DATASIZE

	 *************************************************************************/
	while(blen>=state.DATASIZE){
		memcpy(&state.P.x[datastart], data, (uint)state.DATASIZE);
		FormatData(&state.P.x[datastart], (uint)state.DATASIZE/sizeof(uint));
		PolyAdd(state.N, state.N, &state.P.x[datastart], state.DATASIZE/sizeof(uint));
		if(_Transform(&state))	return FAIL;
		blen-=state.DATASIZE;
		data+=state.DATASIZE;
	}
	/*************************************************************************

	  	Load any remaining data

	 *************************************************************************/
	if(blen>0){
		memcpy(&state.P.x[datastart], data, (uint)blen);
		state.blen = blen;
		data+=blen;
	}
	cnt = state.blen;
	ptr = ((unsigned char*)(&state.P.x[datastart])) + cnt;
    if(dblen){
		*ptr++ = ((*data)&BMASK[dblen])|TBIT[dblen];
	}
	else{
		*ptr++ = 0x80;
	}
    cnt = state.DATASIZE - 1 - cnt;

    
    memset(ptr, 0, cnt );
	FormatData(&state.P.x[datastart], (uint)state.DATASIZE/sizeof(uint));
	PolyAdd(state.N, state.N, &state.P.x[datastart], state.DATASIZE/sizeof(uint));
	if(_Transform(&state))	return FAIL;


    memcpy(&state.P.x[datastart], state.N, state.DATASIZE);
	SET(state.P.x, 1, state.clen);
	SETCTR(&state.P.x[state.clen], state.mlen, state.clen);

	while(PointDecompress(&state.P, state.T))	{
		ADD(state.P.x, 2, state.clen);
	}
	/*************************************************************************

	  	Q = Q + P, up counter, zeroize blen

	 *************************************************************************/
	PointAdd(&state.Q, &state.Q, &state.P, state.T);
	/*************************************************************************

	  	Q = Q + [Q.x/2]G where[Q.x/2] interpreted as an integer and divided by
	  	2, and G is the base point

	 *************************************************************************/
	PolyShiftRight(state.P.x, state.Q.x, state.T->sz);
	BasePointMultiply(&state.P, state.T->sz, state.P.x, state.T);
	PointAdd(&state.Q, &state.Q, &state.P, state.T);
	PolyShiftRight(state.Q.x, state.Q.x, state.T->sz);
	Unload(hashval, state.Q.x, state.DIGESTSIZE);

	return SUCCESS;
}
/*
	END OF ECOH FUNCTIONS

 *****************************************************************************/

/*****************************************************************************

  	[1] Guide to Elliptic Curve Cryptography, Hankerson, D., Menezes, A., 
  		Vanstone, S., Springer-Verlag, New York, 2004.

  	[2] Standards for Efficient Cryptography Group, SEC 1, Elliptic Curve 
  		Cryptography, Working Draft, Version 1.9, Brown, D.L.R., 22 August 2008,
  		http://www.secg.org/download/aid-773/sec1_1point9.pdf 

 *****************************************************************************/
