OUTPUT_FORMAT(binary)

OUTPUT_ARCH(i386)

SECTIONS{
	
	
	. = 0x0;
	_kernel_begin = .;
	ipl 0x0 : {
	_ipl_begin = .;
	_ipl_end = .;
		
		_kernel_main_begin = .;
		
		
		jmpmain.o;
		main.o;
		asm_util.o;
		
		
		
		
		_debug = .;
		
		
		
		font.o;
		graphics.o;
		gdt.o;
		int.o;
		
		
		
		
		
		
		_kernel_main_end = .;
		
	}
	
	_kernel_end = .;

	
	
	
	_kernel_end_sector = ((_kernel_end - _kernel_begin + _kernel_main_end - _kernel_main_begin ) + 511) / 512;
	_kernel_size = _kernel_end - _kernel_begin;
	_kernel_main_size = _kernel_main_end - _kernel_main_begin;
}
