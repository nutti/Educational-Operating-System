#include "asm_util.h"
#include "graphics.h"
#include "gdt.h"

extern char _kernel_begin;
extern char _kernel_end;
extern char _kernel_main_begin;
extern char _debug;

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
	
	asm_disable_intr();
	InitGDT();
	InitIDT();
	InitPIC();
	asm_enable_intr();
	
	printf_str( 0, 0, " " );
	printf_str( 0, 0, "%d", (int)&_debug - (int)&_kernel_main_begin );
	printf_str( 0, 20, "%d", (int)&_kernel_main_begin - (int)&_kernel_begin );
	//printf_str( 0, 0, "Welcome to my OS" );
	//printf_str( 0, 16, "KernelSize %d", kernel_size );
	
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
