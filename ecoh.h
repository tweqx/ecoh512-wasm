/* $Id: ecoh.h,v 1.2 2008/10/30 16:00:19 aantipa Exp $ */
/* $Source: /cvs/ca/eng/ecoh/src/Optimized_64bit/ecoh.h,v $ */
/*****************************************************************************

  	ecoh.h			Header file for ECOH hash

  	DATE:			28 August 2008

 *****************************************************************************/
#include <stdio.h>
#include <memory.h>
#include <time.h>

#ifndef _ECOH_H_
#define _ECOH_H_
/*****************************************************************************

  	Multi-platform constants and MACROS

 *****************************************************************************/
#define ECOH224_DATASIZE		16
#define ECOH224_DIGESTSIZE		28
#define ECOH256_DATASIZE		16
#define ECOH256_DIGESTSIZE		32
#define ECOH384_DATASIZE		24 
#define ECOH384_DIGESTSIZE		48
#define ECOH512_DATASIZE		32
#define ECOH512_DIGESTSIZE		64

#define YP_BIT_T283				255
#define YP_BIT_T409				319
#define YP_BIT_T571				511

#define	ECOH224					224
#define ECOH256					256
#define	ECOH384					384
#define ECOH512					512

#define uchar					unsigned char

#define	DATASTART				(16/sizeof(uint))
#define COUNTERSIZE				(8/sizeof(uint))

#define TRACET409(OP)		((OP)[0]&0x01)
/*****************************************************************************

  	Platform specific constants and MACROS

 *****************************************************************************/
#ifdef _x64_

#define LIMB_SIZE			8
#define LIMB_BIT_SIZE			64
#define LIMB_NIB_LENGTH			16
#define LIMB_LGBITS_SIZE		6
#define TOPBIT				0x8000000000000000

#define SECT283_SIZE			5
#define SECT409_SIZE			7
#define SECT571_SIZE			9
#define MAX_SIZE			9


#define ECOH224_CLEN			1
#define ECOH256_CLEN			1
#define ECOH384_CLEN			1
#define ECOH512_CLEN			2

#define MAX_DATASIZE			4

#define	uint				unsigned long long
#define	sint				long long

#define TRACET283(OP)		(((OP)[0]^((OP)[4]>>15))&0x01)
#define TRACET571(OP)		(((OP)[0]^((OP)[8]>>49)^((OP)[8]>>57))&0x01)
#define GETBIT(OP, I)		((((OP)[(I)/64]&(BIT[(I)%64]))>>((I)%64)))
#define SZ_PRINTINT			"0x%016I64X, "

#else

#define LIMB_SIZE			4
#define LIMB_BIT_SIZE			32
#define LIMB_NIB_LENGTH			8
#define LIMB_LGBITS_SIZE		5
#define TOPBIT				0x80000000

#define SECT283_SIZE			9

#define SECT409_SIZE			13
#define SECT571_SIZE			18
#define MAX_SIZE			18

#define ECOH224_CLEN			2
#define ECOH256_CLEN			2
#define ECOH384_CLEN			2
#define ECOH512_CLEN			4

#define MAX_DATASIZE			8

#define uint				unsigned int
#define sint				signed int

#define TRACET283(OP)		(((OP)[0]^((OP)[8]>>15))&0x01) 
#define TRACET571(OP)		(((OP)[0]^((OP)[17]>>17)^((OP)[17]>>25))&0x01)
#define GETBIT(OP, I)		((((OP)[(I)/32]&(BIT[(I)%32]))>>((I)%32)))
#define SZ_PRINTINT			"0x%08X, "

#endif
/*****************************************************************************

  	Multi-platform data structures

 *****************************************************************************/
typedef struct
{
	uint	x[MAX_SIZE];
	uint	y[MAX_SIZE];
}point;

typedef struct
{
	uint	sz;
	uint	a;
	uint	*b;
	uint	*N;
	uint	*f;
	point	*G;
}curve;
/*****************************************************************************
  
  	API Function Calls as Specified in ANSI C Cryptography API Profile
  	for SHA-3 Candadite Algorithm Submissions, Rev 5, 11 Febraury 2008.

 *****************************************************************************/
typedef	unsigned char		BitSequence;
typedef unsigned long long	DataLength;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2} HashReturn;
/*****************************************************************************

  	Platform-specific staet structures

 *****************************************************************************/
typedef struct
{
	uint		type;
	uint		DATASIZE;
	uint		DIGESTSIZE;
	uint		blen;
	uint		clen;
	DataLength	counter[2];
	DataLength	mlen[2];
	uint		N[MAX_DATASIZE];
	point		P;
	point		Q;
	curve		*T;
}hashState;

HashReturn	Init(hashState *state, int hashbitlen);
HashReturn	Update(hashState *state, const BitSequence *data, DataLength databitlen);
HashReturn	Final(hashState *state, BitSequence *hashval);
HashReturn	Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

#endif //_ECOH_H_
