#include "asm_util.h"
#include "graphics.h"
#include "gdt.h"
#include "int.h"
#include "tss.h"
#include "task.h"

void switch_task( int prev, int next );
void switch_task_2( int prev, int next );

TSS32 tss[ 2 ];
unsigned char stack[ 2 ][ 2048 ];

void task_a()
{
//	asm_halt();
	int a = 0;
	printf_str( 100, 0, "task a : %d", a );
	//task_switch();
	//asm_halt();
	switch_task_2( TASK_INFO_ADDR + sizeof( TaskInfo ), TASK_INFO_ADDR + sizeof( TaskInfo ) * 2 );
	for( ; ; ){
		++a;
		if( a % 10 == 0 ){
			switch_task_2( TASK_INFO_ADDR + sizeof( TaskInfo ), TASK_INFO_ADDR + sizeof( TaskInfo ) * 2 );
		}
		printf_str( 100, 0, "task a : %d", a );
	}
}

void task_b()
{
	int b = 0;
	printf_str( 100, 100, "task b : %d", b );
	//asm_halt();
	switch_task_2( TASK_INFO_ADDR + sizeof( TaskInfo ) * 2, TASK_INFO_ADDR + sizeof( TaskInfo ) );
	for( ; ; ){
		++b;
		if( b % 10 == 0 ){
			switch_task_2( TASK_INFO_ADDR + sizeof( TaskInfo ) * 2, TASK_INFO_ADDR /*+ sizeof( TaskInfo )*/ );
		}
		printf_str( 100, 100, "task b : %d", b );
	}
}


void kernel_main()
{

	int i = 0xa0000;
	for( ; i <= 0xaffff; ++i ){
		asm_write_mem( i, 1 );
	}

	// Delete boot sector and temporary data.
	for( i = 0x1000; i <= 0x9fc00; ++i ){
		asm_write_mem( i, 0 );
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


	set_task( 1, task_a, stack[ 0 ] + 2048 );
	set_task( 2, task_b, stack[ 1 ] + 2048 );


	switch_task_2( TASK_INFO_ADDR, TASK_INFO_ADDR + sizeof( TaskInfo ) );

	for(;;){
		print_str( 0, 20, "fin" );
		asm_halt();
	}

	asm_halt();
}
