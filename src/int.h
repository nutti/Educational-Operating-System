///////////////////////////////////////////////////////////////////////////////
//
//	@file :	int.h
//	@brief :	Interrupt.
//	@author :	nutti
//	@date :	2011.10.9 (Sat) 14:37
//
///////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_INT_H
#define INCLUDED_INT_H

#define IDT_ADDR 0x0026f800		// IDTが存在するアドレス
#define NUM_IDT 256				// IDTの総数

#define PIC0_IMR 0x21
#define PIC0_OCW2 0x20
#define PIC0_ICW1 0x20
#define PIC0_ICW2 0x21
#define PIC0_ICW3 0x21
#define PIC0_ICW4 0x21

#define PIC1_IMR 0xa1
#define PIC1_OCW2 0xa0
#define PIC1_ICW1 0xa0
#define PIC1_ICW2 0xa1
#define PIC1_ICW3 0xa1
#define PIC1_ICW4 0xa1

#define AR_INTGATE32 0x008e		// IDTに対する割り込み処理用の属性

typedef struct _GateDesc
{
	unsigned short offsetL;		///< オフセットアドレス下位16bit
	unsigned short selector;		///< セレクタ値(関数のポインタ)
	unsigned char count;			///< スタックコピーカウント
	unsigned char type;			///< 種類
	unsigned short offsetH;		///< オフセットアドレス上位16bit
} GateDesc;

/**
*	@brief	IDTの初期化
*/
void InitIDT();
/**
*	@brief				Gate　Descriptorの設定
*	@param pDesc		設定するGateDescのアドレス
*	@param offset		オフセット
*	@param selector	セレクタ値（実行時にジャンプする関数のポインタ）
*	@param op			オプション（特権などの設定を行う）
*/
void SetGateDesc( GateDesc* pDesc, int offset, int selector, int op );
/**
*	@brief	PICの初期化
*/
void InitPIC();
/**
*	@brief	割り込みハンドラ。(IRQ21)
*/
void IntHandler21( int* esp );


#endif
