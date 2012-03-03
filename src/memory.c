#include "memory.h"

void memset( void* p, int val, int size )
{
	int i = 0;
	char* pp = p;
	for( i = 0; i < size; ++i, ++pp ){
		*pp = val;
	}
}

void zero_memory( void* p, int size )
{
	memset( p, 0, size );
}
