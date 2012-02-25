##################################################
#
#	@file :	ipl.s
#	@brief :	Boot sector.
#	@author :	nutti
#	@date :	2011.9.5 (Mon) 17:30
#
##################################################

# Start up should be real mode.
base_address:
.code16

# Constant numbers
boot_begin = 0x7c00				# ブートセクタ読み込み先アドレス
ipl_size = 0x200					# IPLサイズ
fd_sector_per_track = 0x12		# 1トラックあたりのフロッピーディスクのセクタ数
fd_sector_length = 512			# セクタの長さ
kernel_begin_sector = 0x01		# カーネルが始まるセクタ番号
read_kernel_es_offset = 0x1000	# 0x1000から読み込まれる

	.extern	begint


# Jump instruction. ( 3 bytes )
	jmp		begin
	nop
	
	
# OEM name. ( 8 bytes )
oem_name:
	.ascii		"HelloIpl"
	
	
# BIOS Parameter Block ( 51 bytes )
sector_size:				# Size of sector. ( This value must be 512 in floppy disk. )
	.word		0x0200
cluster_size:				# Size of cluster. ( This value must be 1 in floppy disk. )
	.byte		0x01
fat_pos:					# Position of FAT.
	.word		0x0001
num_fat:					# Number of FAT. ( This value must be 2. )
	.byte		0x02
root_dir_size:			# Size of root directory.
	.word		0x00e0
num_sector:				# Number of sector. ( This value must be 2880. )
	.word		0x0b40
media_type:				# Media type.
	.byte		0xf0
fat_size:					# Size of FAT.
	.word		0x0009
num_sector_per_track:	# Number of sector per track.
	.word		0x0012
num_head:					# Number of head.
	.word		0x0002
partition:					# Partition.
	.long		0x0000
num_sector_re:			# Rewrite number of sector. ( This value must be 2880. )
	.long		0x00000b40
drive_no:					# Drive number.
	.byte		0x00
reserved:					# Reserved.
	.byte		0x00
ext_boot_code:			# Boot code.
	.byte		0x29
volume_serial:			# Serial number of volume.
	.long		0xffffffff
disk_name:					# Disk name.
	.ascii		"HELLO-OS   "
fat_name:					# FAT name.
	.ascii		"FAT12   "
	
	.skip		18, 0
	

# Bootstrap code ( 448 bytes )
begin:

	# Setup registers.
	movw	$0, %ax
	movw	%ax, %ss
	movw	$_kernel_begin, %sp
	movw	%ax, %ds
	movw	%ax, %es
	
	movb	%dl, boot_drive			# Save drive number to memory.
	
	# カーネルのソースコードは、0x7c00に存在する。
	# これを0x1000に移動させたい。
	# rep movsbは、cxレジスタに登録された回数だけ、
	# [DS:SI]の内容1バイトを[ES:DI]に2バイト転送する。
	# すなわち、DSとESを0にすることで、セグメントを0に設定し、
	# siをboot_beginとして、diをkernel_beginとすることで、
	# 0x7c00から0x1000へipl_size/2すなわち、216回、
	# 2バイトずつ転送する。
	movw	$boot_begin, %si
	movw	$_kernel_begin, %di
	movw	$(ipl_size / 2 ), %cx
	rep		movsw
	ljmp	$0x0000, $set_cs
	
set_cs:
	movw	$BOOT_MSG, %si
	call	_bios_print
	
read_disk:
	# Reset disk.
	movb	%dl, boot_drive
	call	reset_disk
	jc		show_error_msg
	movw	$DISK_RESET_MSG, %si
	call	_bios_print
	
	# Read disk.
	pushw	$0x00
	popw	%es	
	movw	$ipl_end, %di						# ES:DI = 0 + ipl_end ( 0x200 )
	movb	boot_drive, %dl					# DL must be drive number.
	movw	$kernel_begin_sector, %si		# SI must be sector number.
read_kernel:
	call	read_disk_sector
	jc		show_error_msg
	addw	$fd_sector_length, %di			# DI = DI + 0x200 (Next sector.)
	jnc		advance_read_kernel_sector		# DI < 0xFFFF
	movw	%es, %ax
	addw	$read_kernel_es_offset, %ax
	movw	%ax, %es							# ES += 0x1000
advance_read_kernel_sector:
	incw	%si									# SI = SI + 1 (Advance sector number.)
	cmpw	$_kernel_end_sector, %si
	jb		read_kernel						# kernel_end_sector > SI
	
	movw	$DISK_READ_MSG, %si
	call	_bios_print
	jmp		ipl_end							# Finish to load program from disk.
	
	


#####################################
#	Utility functions.
#####################################

	
########################################################################
#
#	@name	:	reset_disk
#	@brief :	Reset disk
#	@usage :	DL ( Drive number. )
#	@return :	AH ( Error code. )
#				EFLAGS ( Set carry flag if the error occured.
#
########################################################################
reset_disk:
	xor		%al, %al
	xor		%ah, %ah		# AH must be 0.
	int		$0x13			# Execute
	ret

########################################################################
#
#	@name	:	read_disk_sector
#	@brief :	Read disk sector.
#	@usage :	DL ( Drive number. )
#				SI ( Sector number. )
#				ES:DI ( Destination buffer address. )
#	@return :	AL ( Error code. )
#
########################################################################
read_disk_sector:
	pushw	%dx				# Save drive number.
	xorw	%dx, %dx
	movw	%si, %ax
	
	# Convert sector to track number.
	movb	$fd_sector_per_track, %dl
	divb	%dl								# AX / DL = AH(track), AL(sector, surplus)
	
	movb	%al, %ch			# CH must be track number.
	shrb	$0x01, %ch			# Get sector number in head.

	jnc		head_0				# CH % 2 == 0
	mov		$0x01, %dh			# DH must be head number.
head_0:
	movb	%ah, %cl
	incb	%cl					# CL must be sector number.

	movw	%di, %bx			# BX must be distination address.
	
	popw	%ax
	movb	%al, %dl			# DL must be drive number.
	
	movb	$0x01, %al			# AL must be number of sector which will be read.
	
	movb	$0x02, %ah			# AH must be 0x02.
	int		$0x13				# Execute.
	ret


	
########################################################################
#
#	@name	:	_bios_print
#	@brief :	Reset disk
#	@usage :	SI ( Head adress of string. String must be
#				finished with 0x00 )
#
########################################################################
_bios_print:
	movb	(%si), %al
	add		$0x01, %si
	cmpb	$0x00, %al
	je		bios_print_fin
	movb	$0x0e, %ah
	movw	$0x000f, %bx		# ここの値も異なっていたが不明。
	int		$0x10
	jmp		_bios_print
bios_print_fin:
	ret


########################################################################
#
#	@name	:	show_error_msg
#	@brief :	Reset disk
#	@usage :	SI ( Head adress of string. String must be
#				finished with 0x00 )
#
########################################################################
show_error_msg:
	movw	$ERROR_MSG, %si
	call	_bios_print
error_end:
	hlt
	jmp		error_end



#####################################
#	Messages and strings.
#####################################
	
ERROR_MSG:
	.ascii		"Error!!\r\n"
	.byte		0x00
	
NEW_LINE:
	.ascii		"\r\n"
	.byte		0x00
	
BOOT_MSG:
	#.byte		0x0a, 0x0a				# 改行コードのようだ
	.ascii		"Boot from floppy disk...\r\n"		# 復帰を入れないと表示がおかしくなる
	.byte		0x00
	
DISK_RESET_MSG:
	.ascii		"Success to reset disk.\r\n"
	.byte		0x00
	
DISK_READ_MSG:
	.ascii		"Success to read disk.\r\n"
	.byte		0x00
	

#####################################
#	Infomations.
#####################################

# Boot drive.
boot_drive:
	.byte		0x00
	

#####################################
#	End of ipl.s.
#####################################
	
# Fill 0.
	.org		0x1fe					# 510まで0で埋める命令
	
# Signature ( 2 bytes )
	.short		0xaa55
	
ipl_end:
