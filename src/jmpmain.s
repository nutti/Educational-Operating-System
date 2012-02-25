.code32

	.global kernel_main

jmp_main:
	#.byte 0x00			# なぜかこれがないと動作しない(2の倍数+1ならOKらしい。)
	#nop						# 多分stdio.oが問題を引き起こしていたと思われる。原因は不明。
	#hlt
	call		kernel_main
	hlt
	hlt
