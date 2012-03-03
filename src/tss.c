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
	tss->eflags	= eflags;
	tss->esp	= ( unsigned int ) esp;
	tss->ds		= ds;
	tss->es		= ds;
	tss->ss		= ss;
	tss->esp0	= ( unsigned int ) esp0;
	tss->ss0	= ss0;
}

