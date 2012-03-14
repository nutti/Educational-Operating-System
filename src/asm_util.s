##############################################################################
#
#	@file :	asm_util.s
#	@brief :	Utility functions which are described from assembler.
#	@author :	nutti
#	@date :	2011.9.15 (Thu) 17:45
#
##############################################################################

.code32

	.global	asm_halt
	.global	asm_write_mem
	.global	asm_enable_intr
	.global	asm_disable_intr
	.global	asm_get_eflags
	.global	asm_set_eflags
	.global	asm_out8
	.global	asm_lgdt
	.global	asm_seg_jmp
	.global	asm_memcpy
	.global	LoadIDT
	.global	IntHook21
	.extern	IntHandler21
	.global task_switch
	.global load_tr
	.global switch_task


########################################################################
#
#	@name	:	asm_halt
#	@brief :	Stop CPU.
#	@usage :	C language can call this function as follows.
#				void asm_halt();
#
########################################################################
asm_halt:
	hlt
	ret

########################################################################
#
#	@name	:	asm_write_mem
#	@brief :	Write memory directly.
#	@usage :	C language can call this function as follows.
#				void asm_write_mem( int addr, int val );
#				addr is the memory address on which will be written.
#				val is the value which will be written.
#
########################################################################
asm_write_mem:
	movl	4(%esp), %ecx
	movb	8(%esp), %al
	movb	%al, (%ecx)
	ret
	
########################################################################
#
#	@name	:	asm_enable_intr
#	@brief :	Enable interrupt.
#	@usage :	C language can call this function as follows.
#				void asm_enable_intr();
#
########################################################################
asm_enable_intr:
	sti
	ret
	
########################################################################
#
#	@name	:	asm_disable_intr
#	@brief :	Disable interrupt.
#	@usage :	C language can call this function as follows.
#				void asm_disable_intr();
#
########################################################################
asm_disable_intr:
	cli
	ret
	
	
# Enable intel syntax.
.intel_syntax noprefix

########################################################################
#
#	@name	:	asm_get_eflags
#	@brief :	Get eflags.
#	@usage :	C language can call this function as follows.
#				int get_eflags();
#				eflags will be returned.
#
########################################################################
asm_get_eflags:
	pushfd
	pop		eax
	ret

########################################################################
#
#	@name	:	asm_set_eflags
#	@brief :	Set eflags.
#	@usage :	C language can call this function as follows.
#				void set_eflags( int eflags );
#				eflags will be set.
#
########################################################################
asm_set_eflags:
	mov		eax, [esp+4]
	push	eax
	popfd
	ret
	
########################################################################
#
#	@name	:	asm_out8
#	@brief :	Output. (8bit)
#	@usage :	C language can call this function as follows.
#				void asm_out8( int port, int data );
#				port is port number.
#				data is data which will be sent.
#
########################################################################
asm_out8:
	mov			edx, [esp+4]
	mov			al, [esp+8]
	out			dx, al
	ret
	
########################################################################
#
#	@name	:	asm_lgdt
#	@brief :	Load segment descriptor table.
#	@usage :	C language can call this function as follows.
#				void asm_lgdt( int limit, int addr );
#				limit is the size of gdt.
#				addr is the address of first gdt.
#
########################################################################
asm_lgdt:
	mov		ax, [esp+4]
	mov		[esp+6], ax
	lgdt	[esp+6]
	ret
	
########################################################################
#
#	@name	:	LoadIDT
#	@brief :	Load interrupt descriptor table.
#	@usage :	C language can call this function as follows.
#				void LoadIDT( int limit, int addr );
#				limit is the size of idt.
#				addr is the address of first idt.
#
########################################################################
LoadIDT:
	mov		ax, [esp+4]
	mov		[esp+6], ax
	lidt	[esp+6]
	ret	
	
########################################################################
#
#	@name	:	asm_seg_jmp
#	@brief :	Jump to another segment.
#	@usage :	C language can call this function as follows.
#				void asm_seg_jmp( int seg, int addr );
#				seg is the segment to be jampped.
#				addr is the address.
#
########################################################################
asm_seg_jmp:
	mov		eax, [esp+4]
	mov		edx, [esp+8]
	mov		esp, 0x310000
	jmp		16,	 0x0000001b
	
########################################################################
#
#	@name	:	asm_memcpy
#	@brief :	Copy memory.
#	@usage :	C language can call this function as follows.
#				void asm_memcpy( char* pSrc, char* pDest, int count );
#				pSrc is the source address of copy.
#				pDest is the destination address to be copied.
#
########################################################################
asm_memcpy:
	push	eax
	push	ebx
	push	ecx
	push	edx
	mov		eax, [esp+20]		# pushした分、スタックポインタを移動させる。
	mov		edx, [esp+24]
	mov		ecx, [esp+28]
memcopy:
	mov		ebx, [eax]
	mov		[edx], ebx
	add		eax, 4
	add		edx, 4
	sub		ecx, 1
	jnz		memcopy
	pop		edx
	pop		ecx
	pop		ebx
	pop		eax
	ret
	
########################################################################
#
#	@name	:	IntHook21
#	@brief :	Hook IRQ21.
#
########################################################################
IntHook21:
	push	es
	push	ds
	pushad					# レジスタの全保存
	mov		eax, esp
	push	eax
	mov		ax, ss
	mov		ds, ax
	mov		es, ax			# C言語の関数を呼ぶときは、SS=DS=ESでなくてはならない
	call	IntHandler21
	pop		eax
	popad
	pop		ds
	pop		es				# レジスタの復帰
	iretd					# 割り込みルーチンからの復帰

########################################################################
#
#	@name	:	load_tr
#	@brief	:	Load TR register.
#	@usage	:	void load_tr( int tr );
#
########################################################################
load_tr:
	LTR		[esp+4]
	RET


########################################################################
#
#	@name	:	task_switch
#	@brief	:	Task switch function.
#	@usage	:	void task_switch();
#
########################################################################
task_switch:
	ljmp 	5*8:0x0
	#hlt
	#ljmp	$0x20,$0x0
	RET


# switch_task( src, dst )
switch_task:
	pushad
	mov		ebx, [esp+36]	# src

	# Copy to previous task.
	mov		eax, [esp+32]
	mov		[ebx+40], eax	# eax
	mov		eax, [esp+28]
	mov		[ebx+44], eax	# ecx
	mov		eax, [esp+24]
	mov		[ebx+48], eax	# edx
	mov		eax, [esp+20]
	mov		[ebx+52], eax	# ebx
	mov		eax, [esp+16]
	mov		[ebx+56], eax	# esp
	mov		eax, [esp+12]
	mov		[ebx+60], eax	# ebp
	mov		eax, [esp+8]
	mov		[ebx+64], eax	# esi
	mov		eax, [esp+4]
	mov		[ebx+68], eax	# edi
	mov		[ebx+72], es	# es
	mov		[ebx+76], cs	# cs
	mov		[ebx+80], ss	# ss
	mov		[ebx+84], ds	# ds
	mov		[ebx+88], fs	# fs
	mov		[ebx+92], gs	# gs

	popad

	# Move to destination task.
	mov		ebx, [esp+8]
	mov		ecx, [ebx+44]	# ecx
	mov		edx, [ebx+48]	# edx
	mov		esp, [ebx+56]	# esp
	mov		ebp, [ebx+60]	# ebp
	mov		esi, [ebx+64]	# esi
	mov		edi, [ebx+68]	# edi
#	mov		es, [ebx+72]	# es
#	mov		cs, [ebx+76]	# cs
#	mov		ss, [ebx+80]	# ss
#	mov		ds, [ebx+84]	# ds
	mov		fs, [ebx+88]	# fs
	mov		gs, [ebx+92]	# gs
	mov		eax, [ebx+40]	# eax
#	mov		ebx, [ebx+52]	# ebx

	push	[ebx+80]		# push ss
	push	[ebx+56]		# push esp
	push	[ebx+36]		# push eflags
	push	[ebx+76]		# push cs
	push	[ebx+32]		# push eip
	push	[ebx+52]		# push ebx
	mov		es, [ebx+72]	# es
	mov		ds, [ebx+84]	# ds
	pop		ebx
#	hlt
	iretd
