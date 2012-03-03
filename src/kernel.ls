OUTPUT_FORMAT(binary)

OUTPUT_ARCH(i386)

MEMORY
{
	rom (rx)	: org = 0x280000, l = 128K
	ram (rwx)	: org = 0x0, l = 200K
}

SECTIONS
{
	.text : {
		jmpmain.o(.text)
		main.o(.text)
		asm_util.o(.text)
		font.o(.text)
		graphics.o(.text)
		gdt.o(.text)
		int.o(.text)
		tss.o(.text)
		memory.o(.text)
		*(.rodata)
	} > rom

	.data : {
		*(.data)
	} > ram AT > rom

	.bss : {
		*(.bss)
	} > ram

	_kernel_begin = .;
	_ipl_begin = .;
	_ipl_end = .;
		_kernel_main_begin = .;
		_debug = .;
		_kernel_main_end = .;
	_kernel_end = .;
	
	_kernel_end_sector = ((_kernel_end - _kernel_begin + _kernel_main_end - _kernel_main_begin ) + 511) / 512;
	_kernel_size = _kernel_end - _kernel_begin;
	_kernel_main_size = _kernel_main_end - _kernel_main_begin;
}
