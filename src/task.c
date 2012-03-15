#include "task.h"

void init_task()
{
	unsigned int* cur_task = (unsigned int*)CUR_TASK_NUM_ADDR;
	TaskInfo* t = (TaskInfo*)TASK_INFO_ADDR;
	int i = 0;
	
	*cur_task = 0;
	for( i = 0; i < TASK_INFO_TOTAL; ++i ){
		zero_memory( t + i, sizeof( TaskInfo ) );
	}
}

void set_task( unsigned int task, void (*f)(), unsigned char* esp )
{
	TaskInfo* t = (TaskInfo*)TASK_INFO_ADDR;
	if( task > TASK_INFO_TOTAL ){
		return;
	}
	t += task;
	zero_memory( t, sizeof( TaskInfo ) );
	t->cs = 0x18;
	t->ds = t->fs = t->gs = t->es = 0x10;
	t->ss = 0x10;
	t->eip = -0x280000 +   (unsigned int) f;
	t->esp = (unsigned int) esp;
	t->eflags = 0x00000202;
}

unsigned int get_cur_task_num()
{
	unsigned int* cur_task = (unsigned int*)CUR_TASK_NUM_ADDR;
	return *cur_task;
}

/*void switch_task( unsigned int task )
{
	
}*/
