///////////////////////////////////////////////////////////////////////////////
//
//	@file :	graphics.h
//	@brief :	Lowest level function of graphics.
//	@author :	nutti
//	@date :	2011.9.20 (Tue) 17:35
//
///////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_GRAPHICS_H
#define INCLUDED_GRAPHICS_H

#ifndef NULL
#define NULL 0
#endif

void init_palette();
void print( int x, int y, char c );
void print_str( int x, int y, char* pStr );
void printf_str( int x, int y, char* pStr, ... );

#endif
