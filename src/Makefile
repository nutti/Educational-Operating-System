AS			= as						# GNU as
#SRC			= ipl.s					# Source files
#OBJCOPY	= objcopy					# Convert binary file from object file.
IMG			= ../bin/os.img			# Image file of operating system.
OBJS		= ipl.o setup.o asm_util.o graphics.o gdt.o main.o jmpmain.o int.o			# Immediate object file.
BIN			= os.bin					# Immediate binary file.
DD			= dd						# Copy.
QEMU		= qemu						# Emulator
RM			= rm						# Remove
LINKER_SCRIPT		= os.ls			# Linker scripts.
CC			= gcc
CFLAGS		= -O2 -c -masm=intel -m32 -Wno-pointer-arith -fno-stack-protector
FONT2ASM	= tools/font2asm.pl
MAKEFILE	= Makefile





all: $(OBJS) $(LINKER_SCRIPT) $(MAKEFILE) font/std.font
	-$(RM) font.s
	$(FONT2ASM) font/std.font font.s
	$(AS) -o font.o font.s --32
	ld $(OBJS) -T $(LINKER_SCRIPT) -o $(BIN)
	#$(OBJCOPY) -S -O binary $(OBJS) $(BIN)
	$(DD) if=/dev/zero of=$(IMG) count=2880
	$(DD) if=$(BIN) of=$(IMG) conv=notrunc

run: $(OBJS) $(LINKER_SCRIPT) $(MAKEFILE)
	-$(RM) font.s
	$(FONT2ASM) font/std.font font.s
	$(AS) -o font.o font.s --32
	ld $(OBJS) -T $(LINKER_SCRIPT) -o $(BIN)
	#$(OBJCOPY) -S -O binary $(OBJS) $(BIN)
	$(DD) if=/dev/zero of=$(IMG) count=2880
	$(DD) if=$(BIN) of=$(IMG) conv=notrunc
	$(QEMU) -m 128 -fda $(IMG) -monitor stdio
	
make_font: $(FONT2ASM) font/std.font
	-$(RM) font.s
	$(FONT2ASM) font/std.font font.s
	
.s.o:
	$(AS) -o $@ $^ --32
	
.c.o:
	$(CC) $(CFLAGS) -o $@ $^

clean:
	-$(RM) $(OBJS)
	-$(RM) $(BIN)
	
erase:
	-$(RM) $(OBJS)
	-$(RM) $(BIN)
	-$(RM) $(IMG)
	
ipl.o: ipl.s
setup.o: setup.s
asm_util.o: asm_util.s
graphics.o: graphics.c
gdt.o : gdt.c
main.o : main.c
jmpmain.o : jmpmain.s
int.o : int.c
