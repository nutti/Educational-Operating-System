#include "asm_util.h"
#include "graphics.h"
#include "gdt.h"
#include "int.h"

extern char _kernel_begin;
extern char _kernel_end;
extern char _kernel_main_begin;
extern char _debug;
extern char _ipl_begin;
extern char _ipl_end;

extern char std_font[ 2 * 16 * 8 ];

void kernel_main()
{

	char* p = &_kernel_begin;
	char* pp = &_kernel_end;
	int kernel_size = pp - p;

	int i = 0xa0000;
	for( ; i <= 0xaffff; ++i ){
		asm_write_mem( i, 1 );
	}
	
	init_palette();
	i = 0xa0000;
	for( ; i <= 0xaffff; ++i ){
		asm_write_mem( i, 0 );
	}
	
	//for( i = 0xa0000; i <= 0xa0139; ++i ){
	//	asm_write_mem( i, 1 );
	//}

	asm_disable_intr();
	InitGDT();
	InitIDT();
	InitPIC();
	asm_enable_intr();

	//for( i = 0xa0000; i <= 0xa1139; ++i ){
	//	asm_write_mem( i, 1 );
	//}



	//printf_str( 0, 0, "hogehoge" );
	//printf_str( 0, 0, "%d", (int)&_debug - (int)&_kernel_main_begin );
	//printf_str( 0, 20, "%d", (int)&_kernel_main_begin - (int)&_kernel_begin );
//	printf_str( 0, 50, "%d", (int)IntHandler21 );
	
//	printf_str( 0, 0, "%d", (int)&_ipl_end - (int)&_ipl_begin );
//	printf_str( 0, 30, "%d", (int)&_kernel_main_begin - (int)&_ipl_end );

	//printf_str( 0, 0, "b" );

//	print( 0, 0, 'c' );
	//printf_str( 0, 0, "c%d", std_font );

	//printf_str( 0, 0, "aa" );
	printf_str( 0, 16, "KernelSize %d", kernel_size );
	
	for(;;){
		//asm_halt();
	}
	
	//i = 0;
	//while( 1 ){
	//	++i;
	//	printf_str( 0, 20, "%d", i );
	//}
	
	asm_halt();
}
