///////////////////////////////////////////////////////////////////////////////
//
//	@file :	gdt.h
//	@brief :	Segment descriptor table.
//	@author :	nutti
//	@date :	2011.9.21 (Wed) 16:15
//
///////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_GDT_H
#define INCLUDED_GDT_H

#define GDT_ADDR 0x00270000
#define NUM_GDT 8192

typedef struct _SegDesc
{
	unsigned short limitL;
	unsigned short baseL;
	unsigned char baseM;
	unsigned char type;
	unsigned char limitH;
	unsigned char baseH;
} SegDesc;

void InitGDT();
void SetSegDesc( SegDesc* pDesc, int limit, int base, int op );

#endif
