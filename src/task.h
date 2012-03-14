#ifndef INCLUDED_TASK_H
#define INCLUDED_TASK_H

#define CUR_TASK_NUM_ADDR	0x25ff00		// Current task number
#define TASK_INFO_ADDR		0x260000		// Address to the head of TaskInfo structure.
#define TASK_INFO_TOTAL		10				// Number of tasks.

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
} TaskInfo;

void init_task();
void set_task( unsigned int task, void (*f)(), unsigned char* esp );
unsigned int get_cur_task_num();
//void switch_task( unsigned int task );

#endif
