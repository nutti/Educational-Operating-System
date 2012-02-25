
#include "stdio.h"
#include "asm_util.h"
#include "graphics.h"
#include "gdt.h"

extern char _kernel_begin;
extern char _kernel_end;


void test()
{
	unsigned short *video = ( unsigned short* ) 0xb800;
	unsigned char* str = "hello,world";
	unsigned char c;
	int i = 0xa0000;
	
	char* p = &_kernel_begin;
	char* pp = &_kernel_end;
	
	int kernel_size = pp - p;
	
	int cnt = 0;
	
	for( ; i <= 0xaffff; ++i ){
		asm_write_mem( i, 10 );
	}
	//puts( "hello" );
	init_palette();
	i = 0xa0000;
	for( ; i <= 0xaffff; ++i ){
		asm_write_mem( i, 0 );
	}
	//for( ; i <= 0xaffff; ++i ){
	//	asm_write_mem( i, std_font[ cnt ] );
	//	++cnt;
	//	if( cnt == 8 * 16 ){
	//		cnt = 0;
	//	}	
	//}
	
	InitGDT();
	
	//asm_memcpy( (char*)0x1000, (char*)0x280000, kernel_size );
	
	// セグメント間ジャンプはここでは行えない。なぜならば、ジャンプ先のコードがわからないからだ。
	
	//asm_seg_jmp( 2, 0x00 );
	
	//*( (char*)0x2000 ) = 0x22;

	print_str( 20, 20, "ABACDEIGKMNSQRTUVWXYZ" );
	print_str( 20, 60, "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
	print_str( 20, 40, "0123456789" );
	print_str( 20, 80, "abcdefghijklmnopqrstuvwxyz" );
	
	print_str( 20, 130, "8f6skk30flsFikFDI" );
	
	i = 23;
	
	printf_str( 20, 150, "aq%drt", i );
	
	printf_str( 20, 180, "%d", kernel_size );

	//print( 20, 20, 'A' );
	//print( 28, 20, 'B' );
	//print( 36, 20, 'A' );


	//asm_halt();
	//bios_print();
	//while( c = *str++ ){
	//	*video++ = ( 0x0E << 8 ) | 'a';
	//}
	//return;
}


