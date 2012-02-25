#ifndef INCLUDED_ASM_UTIL_H
#define INCLUDED_ASM_UTIL_H

void asm_write_mem( int addr, int val );
void asm_halt( void );
void asm_enable_intr( void );
void asm_disable_intr( void );
int asm_get_eflags();
void asm_set_eflags( int eflags );
void asm_out8( int port, int data );
void asm_lgdt( int limit, int addr );
//void asm_seg_jmp( int seg, int addr );			// retが使えないので呼び出しもとに戻れない
void asm_memcpy( char* pSrc, char* pDst, int count );
void LoadIDT( int limit, int addr );
void IntHook21();
void asm_enable_intr();
void asm_disable_intr();

#endif
