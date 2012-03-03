##################################################
#
#	@file :	setup.s
#	@brief :	Setup operating system.
#	@author :	nutti
#	@date :	2011.9.10 (Sat) 21:08
#
##################################################

stack_begin = 0x1000

.code16

	.global	begint
	.extern	test
	.extern	asm_write_mem
	.extern	kernel_main_begin
	.extern boot_size

begint:
	#call	show
	
	# VGAモードに以降
	movb	$0x13, %al
	movb	$0x00, %ah
	int		$0x10
	
	jmp		setup_gdt
	
setup_gdt:
	# Stop interrupts from PIC.
	movb	$0xff, %al
	outb	%al, $0x21
	nop
	outb	%al, $0x21
	
	cli				# Stop interrupt acceptance.
	
	# Enablre A20 signal line.
	
	
	lgdt	gdtr
	
	# Realmode to protect mode.
	movl	%cr0, %eax
	orl		$0x01, %eax
	movl	%eax, %cr0
	
	# Reset pipeline on CPU
	jmp		reset_pipeline
	
	
reset_pipeline:


	# 1000番地にデータが読み込まれているはずなので、280000にデータをコピーする。
	# プロテクトモードではこの様な処理は不可能であった。
	movl		$_boot_size, %eax
	#movl		$_kernel_main_begin, %eax
	movl		$0x280000, %edx
	#movl		$_kernel_main_size, %ecx
	movl		$0x40000, %ecx
cpy:
	movl		(%eax), %ebx
	movl		%ebx, (%edx)
	addl		$4, %eax
	addl		$4, %edx
	subl		$1, %ecx
	jnz			cpy

	#ljmp	$0x08, $set_cs
	
set_cs:


	# セグメントの変更
	movw	$0x10, %ax
	movw	%ax, %ds
	movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss

	movl	$0x310000, %esp
	#movl	$stack_begin, %esp
	
	ljmp $0x08, $fin
	
# 32bitコード。これ以降はBIOSの機能を使えない！！
.code32
	
fin:
#hlt
	#call	test
	lgdt	gdtr2
	#ljmp	$0x8, $0x280000
	
end:
	# call test でhaltを呼んでいるため、ここには戻ってこないはず。
	#call	asm_halt
	#movw	$0x18, %ax
	#movw	%ax, %ds
	#movw	$0x10, %ax
	movw	$0x10, %ax
	movw	%ax, %ds
movw	%ax, %es
	movw	%ax, %fs
	movw	%ax, %gs
	movw	%ax, %ss


	ljmp	$0x8, $0
	#hlt		_
	#ljmp	$0x8, $0x280000
	hlt
	

	
	
.align 8
	
gdtr:
gdtr_limit:
	.word	gdt_end - gdt - 1
gdtr_base:
	.long	gdt
	
.align 8
	
gdt:
# 空のディスクリプタ
gdt_null:
	.word	0x00
	.word	0x00
	.byte	0x00
	.byte	0x00
	.byte	0x00
	.byte	0x00

# カーネルコード
gdt_kernel_cs:
#	.word	0xffff, 0x0000, 0x9200, 0x00cf
	.word	0xffff
	.word	0x00
	.byte	0x00
	.byte	0x98
	.byte	0xdf
	.byte	0x00
	
# カーネルデータ
gdt_kernel_ds:
#	.word	0xffff, 0x0000, 0x9a28, 0x0047
	.word	0xffff
	.word	0x00
	.byte	0x00
	.byte	0x92
	.byte	0xdf
	.byte	0x00
	


	
gdt_end:






.align 8
	
gdtr2:
gdtr_limit2:
	.word	gdt_end2 - gdt2 - 1
gdtr_base2:
	.long	gdt2
	
.align 8
	
gdt2:
# 空のディスクリプタ
gdt_null2:
	.word	0x00
	.word	0x00
	.byte	0x00
	.byte	0x00
	.byte	0x00
	.byte	0x00
	
# カーネルコード
gdt_kernel_cs2:

	.word	0xffff, 0x0000, 0x9a28, 0x0047
	
# カーネルデータ
gdt_kernel_ds2:
	.word	0xffff, 0x0000, 0x9200, 0x00cf

gdt_kernel_ds2_2:
	.word	0xffff, 0x0000, 0x9a28, 0x0047

gdt_end2:
