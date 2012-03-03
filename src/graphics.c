///////////////////////////////////////////////////////////////////////////////
//
//	@file :	graphics.c
//	@brief :	Lowest level function of graphics.
//	@author :	nutti
//	@date :	2011.9.20 (Tue) 17:36
//
///////////////////////////////////////////////////////////////////////////////

#include "graphics.h"

extern char std_font[ 8 * 16 * 52 ];
//char* std_font = 0x280000;

//const int OFFSET = 0x280000;
const int OFFSET = 0x0;


void init_palette()
{
	static unsigned char rgb[ 2 * 3 ] = {	0x00, 0x00, 0x00,
												0xff, 0xff, 0xff };

	int i = 0;
	int eflags;
	eflags = asm_get_eflags();
	asm_disable_intr();
	asm_out8( 0x03c8, 0 );
	for( i = 0; i < 2; ++i ){
		asm_out8( 0x03c9, rgb[ i * 3 ] / 4 );
		asm_out8( 0x03c9, rgb[ i * 3 + 1 ] / 4 );
		asm_out8( 0x03c9, rgb[ i * 3 + 2 ] / 4 );
	}
	asm_set_eflags( eflags );
}

void print( int x, int y, char c )
{
	int i = 0xa0000;
	int j, k;
	for( j = 0; j < 16; ++j ){
		for( k = 0; k < 8; ++k ){
			if( c >= 'A' && c <= 'Z' ){
				asm_write_mem( i + ( x + k ) + ( y + j ) * 320, std_font[ OFFSET + k + j * 8 + ( c - 'A' + 10 ) * 8 * 16 ] );
			}
			else if( c >= 'a' && c <= 'z' ){
				asm_write_mem( i + ( x + k ) + ( y + j ) * 320, std_font[ OFFSET + k + j * 8 + ( c - 'a' + 10 + 26 ) * 8 * 16 ] );
			}	
			else if( c >= '0' && c <= '9' ){
				asm_write_mem( i + ( x + k ) + ( y + j ) * 320, std_font[ OFFSET + k + j * 8 + ( c - '0' ) * 8 * 16 ] );
			}
			else if( c == ' ' ){
			}
		}
	}
}

void print_str( int x, int y, char* pStr )
{
	int cnt = 0;
	while( *pStr ){
		print( x + cnt * 8, y, *pStr++ );
		++cnt;
	}
}

void printf_str( int x, int y, char* pStr, ... )
{
	int cnt = 0;
	int argc = 0;				// 現在指している引数の番号
	int** argv = NULL;		// 引数を指すポインタ
	char str[ 256 ];
	int digit = 1;
	
	pStr += OFFSET;

	argv = &pStr;
	
	while( *pStr ){
		digit = 1;
		if( *pStr != '%' ){
			print( x + cnt * 8, y, *pStr );
		}
		else{
			++pStr;
			++argc;
			++argv;
			switch( *pStr ){
				case 'd':
					digit = int_to_str( *argv, str );
					print_str( x + cnt * 8, y, str );
					break;
				default:
					break;
			}
		}
		cnt += digit;
		++pStr;
	}
}

int int_to_str( int val, char* pStr )
{
	char str[ 256 ];
	char* p = str;
	int i = 0;
	int cnt = 0;
	
	str[ 0 ] = val % 10 + 0x30;
	while( val = val / 10 ){
		++i;
		str[ i ] = val % 10 + 0x30;
	}
	str[ i + 1 ] = '\0';
	
	while( *p ){
		pStr[ i - cnt ] = *p;
		++p;
		++cnt;
	}
	pStr[ i + 1 ] = '\0';
	return i + 1;
}
