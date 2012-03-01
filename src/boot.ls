OUTPUT_FORMAT(binary)

OUTPUT_ARCH(i386)

SECTIONS{
	
	
	. = 0x1000;
	_kernel_begin = .;
	ipl 0x1000 : {
	_ipl_begin = .;
		ipl.o;
	_ipl_end = .;
		setup.o;
		_kernel_main_begin = .;
		
		
		
		
		
		
		_debug = .;
		
		
		
		
		
		
		
		
		
		_kernel_main_end = .;
		
	}
	
	_kernel_end = .;

	
	
	
	_kernel_end_sector = ((_kernel_end - _kernel_begin + _kernel_main_end - _kernel_main_begin ) + 511) / 512;
	_kernel_size = _kernel_end - _kernel_begin;
	_kernel_main_size = _kernel_main_end - _kernel_main_begin;
	_boot_size = _kernel_main_begin - _ipl_begin + 0x1000;
}
