#include "asm_util.h"
#include "graphics.h"
#include "gdt.h"
#include "int.h"
#include "tss.h"
#include "task.h"

void switch_task( int prev, int next );

TSS32 tss[ 2 ];
unsigned char stack[ 2 ][ 2048 ];

void task_a()
{
//	asm_halt();
	int a = 0;
	printf_str( 100, 0, "task a : %d", a );
	//task_switch();
	//asm_halt();
	switch_task( TASK_INFO_ADDR + sizeof( TaskInfo ), TASK_INFO_ADDR + sizeof( TaskInfo ) * 2 );
	for( ; ; ){
		++a;
		printf_str( 100, 0, "task a : %d", a );
	}
}

void task_b()
{
	int b = 0;
	switch_task( TASK_INFO_ADDR + sizeof( TaskInfo ) * 2, TASK_INFO_ADDR + sizeof( TaskInfo ) );
	for( ; ; ){
		++b;
		printf_str( 100, 100, "task b : %d", b );
	}
}


void kernel_main()
{

	int i = 0xa0000;
	for( ; i <= 0xaffff; ++i ){
	//	asm_write_mem( i, 1 );
	}

	//if( *(char*)0x8ffffff == 0 ){
	//	asm_halt();
	//}
	//asm_write_mem( 0x8ffffff, 1 );

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

	printf_str( 20, 20, "%d", sizeof( TSS32 ) );

	//set_task( 0, task_b, stack[ 1 ] + 2048 );
	set_task( 1, task_a, stack[ 0 ] + 2048 );
	set_task( 2, task_b, stack[ 1 ] + 2048 );

	//asm_halt();

	switch_task( TASK_INFO_ADDR, TASK_INFO_ADDR + sizeof( TaskInfo ) );
	asm_halt();

	printf_str( 0, 0, "%d", asm_get_eflags() );

	asm_halt();

	/*set_tss(	&tss[ 0 ],
				0x8,
				0x10,
				task_a,
				0x00000202,
				stack[ 0 ] + 2048,
				0x10,
				0,
				0 );*/

	zero_memory( tss, sizeof( TSS32 ) );

	//tss[ 0 ].eflags = asm_get_eflags();
	tss[ 0 ].iomap = 0x40000000;
	tss[ 0 ].ldtr = 0;

	set_tss(	tss + 1,
				0x10,
				0x10,
				task_a,
				0x00000202,
				stack[ 0 ] + 2048,
				0x10,
				0,	
				0 );

	

	SegDesc* pDesc = (SegDesc*)GDT_ADDR;



	SetSegDesc( pDesc + 4, 103, (unsigned int)tss, 0x0089 );
	SetSegDesc( pDesc + 5, 103, (unsigned int)(tss + 1), 0x0089 );
	asm_lgdt( 0xffff, 0x00270000 );

	load_tr( 4 * 8 );

	//asm_halt();

	task_switch();

	//task_a();

	for(;;){
		asm_halt();
	}

	asm_halt();
}
