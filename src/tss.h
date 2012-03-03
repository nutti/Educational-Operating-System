#ifndef INCLUDED_TSS_H
#define INCLUDED_TSS_H

typedef struct
{
	unsigned int	backlink;
	unsigned int	esp0;		// Stack pointer. ( Privilage level 0 )
	unsigned int	ss0;		// Stack segment. ( Privilage level 0 )
	unsigned int	esp1;		// Stack pointer. ( Privilage level 1 )
	unsigned int	ss1;		// Stack segment. ( Privilage level 1 )
	unsigned int	esp2;		// Stack pointer. ( Privilage level 2 )
	unsigned int	ss2;		// Stack segment. ( Privilage level 2 )
	unsigned int	cr3;		// Address table of task.
	unsigned int	eip;		// Instruction pointer.
	unsigned int	eflags;		// E-flags.
	unsigned int	eax;		// EAX register.
	unsigned int	ecx;		// ECX register.
	unsigned int	edx;		// EDX register.
	unsigned int	ebx;		// EBX register.
	unsigned int	esp;		// Stack pointer.
	unsigned int	ebp;
	unsigned int	esi;
	unsigned int	edi;
	unsigned int	es;
	unsigned int	cs;
	unsigned int	ss;
	unsigned int	ds;
	unsigned int	fs;
	unsigned int	gs;
	unsigned int	ldtr;
	unsigned int	iomap;
} TSS32;

void set_tss(	TSS32* tss,
				unsigned int cs,
				unsigned int ds,
				void (*f)(),
				unsigned int eflags,
				unsigned char* esp,
				unsigned int ss,
				unsigned char* esp0,
				unsigned int ss0 );

#endif
