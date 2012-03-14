///////////////////////////////////////////////////////////////////////////////
//
//	@file :	int.c
//	@brief :	Interrupt.
//	@author :	nutti
//	@date :	2011.10.9 (Sat) 15:16
//
///////////////////////////////////////////////////////////////////////////////

#include "int.h"
#include "asm_util.h"
#include "graphics.h"

void InitIDT()
{
	GateDesc* pDesc = ( GateDesc* ) IDT_ADDR;
	int i = 0;
	
	for( i = 0; i < NUM_IDT; ++i ){
		SetGateDesc( pDesc + i, 0, 0, 0 );
	}
	// 実行権限が必要
	SetGateDesc( pDesc + 0x21, (int)IntHook21, 3 * 8, AR_INTGATE32 );
	
	LoadIDT( 0x7ff, IDT_ADDR );
}

void SetGateDesc( GateDesc* pDesc, int offset, int selector, int op )
{
	pDesc->offsetL = offset & 0xFFFF;
	pDesc->selector = selector;
	pDesc->count = ( op >> 8 ) & 0xFF;
	pDesc->type = op & 0xFF;
	pDesc->offsetH = ( offset >> 16 ) & 0xFFFF;
}

void InitPIC()
{
	// 割り込みの受付の禁止
	asm_out8( PIC0_IMR, 0xff );
	asm_out8( PIC1_IMR, 0xff );
	
	// マスタの設定
	asm_out8( PIC0_ICW1, 0x11 );		// エッジトリガモード
	asm_out8( PIC0_ICW2, 0x20 );		// IRQ0-7はINT20-27で受けることを指定
	asm_out8( PIC0_ICW3, 1 << 2 );	// スレーブは2番目にある(00000100)
	asm_out8( PIC0_ICW4, 0x01 );		// ノンバッファモード
	
	// スレーブの設定
	asm_out8( PIC1_ICW1, 0x11 );		// エッジトリガモード
	asm_out8( PIC1_ICW2, 0x28 );		// IRQ8-15はINT28-2fで受けることを指定
	asm_out8( PIC1_ICW3, 2 );			// マスタの2番目に接続する
	asm_out8( PIC1_ICW4, 0x01 );		// ノンバッファモード
	
	// PIC1以外の割り込みを禁止
	
	asm_out8( PIC0_IMR, 0xfb );
	asm_out8( PIC1_IMR, 0xff );
	
	asm_out8( PIC0_IMR, 0xf9 );
	asm_out8( PIC1_IMR, 0xff );
}

void IntHandler21( int* esp )
{
	print( 0, 50, 'c' );

	asm_out8( PIC0_OCW2, 0x61 );
	
		printf_str( 0, 80, "Interrupted!!%d", (int)IntHandler21 );
}
