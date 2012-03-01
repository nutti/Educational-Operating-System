///////////////////////////////////////////////////////////////////////////////
//
//	@file :	gdt.c
//	@brief :	Segment descriptor table.
//	@author :	nutti
//	@date :	2011.9.21 (Wed) 16:24
//
///////////////////////////////////////////////////////////////////////////////

#include "gdt.h"
#include "asm_util.h"

void InitGDT()
{
	SegDesc* pDesc = (SegDesc*)GDT_ADDR;
	int i;
	for( i = 0; i < NUM_GDT; ++i ){
		SetSegDesc( pDesc + i, 0, 0, 0 );
	}
	SetSegDesc( pDesc + 1, 0xffffffff, 0x00000000, 0x4092 );	// 全メモリアクセス用
	SetSegDesc( pDesc + 2, 0x0007ffff, 0x00280000, 0x409a );
	SetSegDesc( pDesc + 3, 0x0007ffff, 0x00290000, 0x4092 );
	asm_lgdt( 0xffff, 0x00270000 );
}

void SetSegDesc( SegDesc* pDesc, int limit, int base, int op )
{
	// G bit
	if( limit > 0xffff ){
		op |= 0x8000;
		limit /= 0x1000;		// 4KB単位にする必要がある。
	}

	pDesc->limitL = limit & 0xffff;
	pDesc->baseL = base & 0xffff;
	pDesc->baseM = ( base >> 16 ) & 0xff;
	pDesc->type = op & 0xff;
	pDesc->limitH = ( ( limit >> 16 ) & 0x0f ) | ( ( op >> 8 ) & 0xf0 );
	pDesc->baseH = ( base >> 24 ) & 0xff;
}
