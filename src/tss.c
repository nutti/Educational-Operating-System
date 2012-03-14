#include "tss.h"
#include "memory.h"

void set_tss(	TSS32* tss,
				unsigned int cs,
				unsigned int ds,
				void (*f)(),
				unsigned int eflags,
				unsigned char* esp,
				unsigned int ss,
				unsigned char* esp0,
				unsigned int ss0 )
{
	zero_memory( tss, sizeof( TSS32 ) );
	tss->cs		= cs;
	tss->eip	= ( unsigned int ) f;
	//printf_str( 80, 60, "%d", tss->eip );
	tss->eflags	= eflags;
	tss->esp	= ( unsigned int ) esp;
	//printf_str( 40, 40, "%d", tss->esp );
	tss->ds		= ds;
	tss->es		= ds;
	tss->fs		= ds;
	tss->gs		= ds;
	tss->ss		= ss;
	tss->esp0	= ( unsigned int ) esp0;
	//printf_str( 100, 100, "%d", tss->esp0 );
	tss->ss0	= ss0;
	tss->iomap	= 0x40000000;
	//tss->esp0 = tss->esp1 = tss->esp2 = tss->esp;
	//tss->ss0 = tss->ss1 = tss->ss2 = tss->ss;
}

