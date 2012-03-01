AS			= as						# GNU as
#SRC			= ipl.s					# Source files
#OBJCOPY	= objcopy					# Convert binary file from object file.
IMG			= ../image/os.img			# Image file of operating system.
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
OD			= od
OD_OPT		= -t x1z -A x
OD_OUT_FILE	= os.img.od
OBJDUMP		= objdump
OBJDUMP_OPT	= -b binary -m i386 -D
OBJDUMP_FILE= os.img.objdump
KERNEL_BIN	= kernel.bin
BOOT_BIN	= boot.bin
KERNEL_OBJS	= asm_util.o graphics.o font.o gdt.o main.o jmpmain.o int.o
BOOT_OBJS	= ipl.o setup.o

all: $(KERNEL_OBJS) $(BOOT_OBJS) $(LINKER_SCRIPT) $(MAKEFILE) font/std.font
	-$(RM) font.s
	$(FONT2ASM) font/std.font font.s
	$(AS) -o font.o font.s --32
	#ld $(OBJS) -T $(LINKER_SCRIPT) -o $(BIN)
	ld $(BOOT_OBJS) -T boot.ls -o $(BOOT_BIN)
	ld $(KERNEL_OBJS) -T kernel.ls -o $(KERNEL_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(BIN)
	$(DD) if=/dev/zero of=$(IMG) count=2880
	$(DD) if=$(BIN) of=$(IMG) conv=notrunc
	$(OD) $(OD_OPT) $(IMG) > $(OD_OUT_FILE)
	$(OD) $(OD_OPT) $(KERNEL_BIN) > kernel.bin.od
	$(OBJDUMP) $(OBJDUMP_OPT) $(IMG) > $(OBJDUMP_FILE)
	$(OBJDUMP) $(OBJDUMP_OPT) $(KERNEL_BIN) > kernel.bin.objdump

run: $(KERNEL_OBJS) $(BOOT_OBJS) $(LINKER_SCRIPT) $(MAKEFILE)
	-$(RM) font.s
	$(FONT2ASM) font/std.font font.s
	$(AS) -o font.o font.s --32
#	ld $(OBJS) -T $(LINKER_SCRIPT) -o $(BIN)
	ld $(BOOT_OBJS) -T boot.ls -o $(BOOT_BIN)
	ld $(KERNEL_OBJS) -T kernel.ls -o $(KERNEL_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(BIN)
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
	-$(RM) $(KERNEL_OBJS)
	-$(RM) $(BOOT_OBJS)
	-$(RM) $(BIN)
	
erase:
	-$(RM) $(KERNEL_OBJS)
	-$(RM) $(BOOT_OBJS)
	-$(RM) $(BIN)
	-$(RM) $(IMG)
	
ipl.o: ipl.s
setup.o: setup.s
asm_util.o: asm_util.s
font.o: font.s
graphics.o: graphics.c
gdt.o : gdt.c
main.o : main.c
jmpmain.o : jmpmain.s
int.o : int.c
