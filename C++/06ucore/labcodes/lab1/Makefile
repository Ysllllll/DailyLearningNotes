PROJ	:= challenge
EMPTY	:=
SPACE	:= $(EMPTY) $(EMPTY)
SLASH	:= /

V       := @
#need llvm/cang-3.5+
#USELLVM := 1
# try to infer the correct GCCPREFX
ifndef GCCPREFIX
GCCPREFIX := $(shell if i386-elf-objdump -i 2>&1 | grep '^elf32-i386$$' >/dev/null 2>&1; \
	then echo 'i386-elf-'; \
	elif objdump -i 2>&1 | grep 'elf32-i386' >/dev/null 2>&1; \
	then echo ''; \
	else echo "***" 1>&2; \
	echo "*** Error: Couldn't find an i386-elf version of GCC/binutils." 1>&2; \
	echo "*** Is the directory with i386-elf-gcc in your PATH?" 1>&2; \
	echo "*** If your i386-elf toolchain is installed with a command" 1>&2; \
	echo "*** prefix other than 'i386-elf-', set your GCCPREFIX" 1>&2; \
	echo "*** environment variable to that prefix and run 'make' again." 1>&2; \
	echo "*** To turn off this error, run 'gmake GCCPREFIX= ...'." 1>&2; \
	echo "***" 1>&2; exit 1; fi)
endif

# try to infer the correct QEMU
ifndef QEMU
QEMU := $(shell if which qemu-system-i386 > /dev/null; \
	then echo 'qemu-system-i386'; exit; \
	elif which i386-elf-qemu > /dev/null; \
	then echo 'i386-elf-qemu'; exit; \
	elif which qemu > /dev/null; \
	then echo 'qemu'; exit; \
	else \
	echo "***" 1>&2; \
	echo "*** Error: Couldn't find a working QEMU executable." 1>&2; \
	echo "*** Is the directory containing the qemu binary in your PATH" 1>&2; \
	echo "***" 1>&2; exit 1; fi)
endif

# eliminate default suffix rules
.SUFFIXES: .c .S .h

# delete target files if there is an error (or make is interrupted)
.DELETE_ON_ERROR:

# define compiler and flags
ifndef  USELLVM
HOSTCC		:= gcc
HOSTCFLAGS	:= -g -Wall -O2
CC		:= $(GCCPREFIX)gcc
CFLAGS	:= -fno-builtin -Wall -ggdb -m32 -gstabs -nostdinc $(DEFS)
CFLAGS	+= $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
else
HOSTCC		:= clang
HOSTCFLAGS	:= -g -Wall -O2
CC		:= clang
CFLAGS	:= -fno-builtin -Wall -g -m32 -mno-sse -nostdinc $(DEFS)
CFLAGS	+= $(shell $(CC) -fno-stack-protector -E -x c /dev/null >/dev/null 2>&1 && echo -fno-stack-protector)
endif

CTYPE	:= c S

LD      := $(GCCPREFIX)ld
LDFLAGS	:= -m $(shell $(LD) -V | grep elf_i386 2>/dev/null)
LDFLAGS	+= -nostdlib

OBJCOPY := $(GCCPREFIX)objcopy
OBJDUMP := $(GCCPREFIX)objdump

COPY	:= cp
MKDIR   := mkdir -p
MV		:= mv
RM		:= rm -f
AWK		:= awk
SED		:= sed
SH		:= sh
TR		:= tr
TOUCH	:= touch -c

OBJDIR	:= obj
BINDIR	:= bin

ALLOBJS	:=
ALLDEPS	:=
TARGETS	:=

include tools/function.mk

listf_cc = $(call listf,$(1),$(CTYPE))

# for cc
add_files_cc = $(call add_files,$(1),$(CC),$(CFLAGS) $(3),$(2),$(4))# add_files(libs/*.c, gcc, $(CFLAGS), libs)
create_target_cc = $(call create_target,$(1),$(2),$(3),$(CC),$(CFLAGS))

# for hostcc
add_files_host = $(call add_files,$(1),$(HOSTCC),$(HOSTCFLAGS),$(2),$(3))
create_target_host = $(call create_target,$(1),$(2),$(3),$(HOSTCC),$(HOSTCFLAGS))

cgtype = $(patsubst %.$(2),%.$(3),$(1))
objfile = $(call toobj,$(1))
asmfile = $(call cgtype,$(call toobj,$(1)),o,asm)
outfile = $(call cgtype,$(call toobj,$(1)),o,out)
symfile = $(call cgtype,$(call toobj,$(1)),o,sym)

# for match pattern
match = $(shell echo $(2) | $(AWK) '{for(i=1;i<=NF;i++){if(match("$(1)","^"$$(i)"$$")){exit 1;}}}'; echo $$?)

# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
# include kernel/user

INCLUDE	+= libs/

CFLAGS	+= $(addprefix -I,$(INCLUDE))

LIBDIR	+= libs

$(call add_files_cc,$(call listf_cc,$(LIBDIR)),libs,)# __objs_libs = obj/libs/*.o

# -------------------------------------------------------------------
# kernel

KINCLUDE	+= kern/debug/ \
			   kern/driver/ \
			   kern/trap/ \
			   kern/mm/

KSRCDIR		+= kern/init \
			   kern/libs \
			   kern/debug \
			   kern/driver \
			   kern/trap \
			   kern/mm

KCFLAGS		+= $(addprefix -I,$(KINCLUDE))

$(call add_files_cc,$(call listf_cc,$(KSRCDIR)),kernel,$(KCFLAGS))# __objs_kernel = obj/kern/**/*.o

KOBJS	= $(call read_packet,kernel libs)

# create kernel target
kernel = $(call totarget,kernel)# totarget 函数是给输入参数增加前缀"bin/"，因此 kernel="bin/kernel"

$(kernel): tools/kernel.ld# kernel目标文件需要依赖tools/kernel.ld文件，kernel.ld文件是一个链接脚本

$(kernel): $(KOBJS)# 指出kernel目标文件依赖的obj文件  KOBJS=obj/libs/*.o  obj/kern/**/*.o
	@echo + ld $@
	$(V)$(LD) $(LDFLAGS) -T tools/kernel.ld -o $@ $(KOBJS)
	@$(OBJDUMP) -S $@ > $(call asmfile,kernel)
	@$(OBJDUMP) -t $@ | $(SED) '1,/SYMBOL TABLE/d; s/ .* / /; /^$$/d' > $(call symfile,kernel)

$(call create_target,kernel)

# -------------------------------------------------------------------

# create bootblock
bootfiles = $(call listf_cc,boot)# bootfiles = boot/*.c boot/*.S
$(foreach f,$(bootfiles),$(call cc_compile,$(f),$(CC),$(CFLAGS) -Os -nostdinc))# 编译bootfiles生成.o文件

bootblock = $(call totarget,bootblock)# totarget 函数是给输入参数增加前缀"bin/"，因此 bootblock="bin/bootblock"

$(bootblock): $(call toobj,$(bootfiles)) | $(call totarget,sign)# 声明bin/bootblock依赖于obj/boot/*.o和bin/sign文件
	@echo + ld $@ 
	$(V)$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 $^ -o $(call toobj,bootblock) 
	@$(OBJDUMP) -S $(call objfile,bootblock) > $(call asmfile,bootblock)
	@$(OBJCOPY) -S -O binary $(call objfile,bootblock) $(call outfile,bootblock)
	@$(call totarget,sign) $(call outfile,bootblock) $(bootblock)

$(call create_target,bootblock)

# -------------------------------------------------------------------

# create 'sign' tools
$(call add_files_host,tools/sign.c,sign,sign)# 设置__objs/sign = obj/sign/tools/sign.o
$(call create_target_host,sign,sign)# 生成目标文件，即生成obj/sign/tools/sign.o

# -------------------------------------------------------------------

# create ucore.img
UCOREIMG	:= $(call totarget,ucore.img)# UCOREIMG = bin/ucore.img

$(UCOREIMG): $(kernel) $(bootblock)# bin/ucore.img依赖于bin/kernel和bin/bootblock
	$(V)dd if=/dev/zero of=$@ count=10000
	$(V)dd if=$(bootblock) of=$@ conv=notrunc
	$(V)dd if=$(kernel) of=$@ seek=1 conv=notrunc

$(call create_target,ucore.img)
##  总结ucore.img的生成过程
#   1. 编译libs和kern目录下的所有.c和.S文件，生成.o文件，并链接得到bin/kernel文件
#   2. 编译boot目录下所有的.c和.S文件，生成.o文件，并链接得到bin/bootblock.out文件
#   3. 编译tools/sign.c文件，得到bin/sign文件
#   4. 利用bin/sign工具将bin/bootblock.out文件转化为512字节的bin/bootblock文件，并将bin/bootblock的最后两个字节设置为0x55AA
#   5. 为bin/ucore.img分配5000MB的内存空间，并将bin/bootblock复制到bin/ucore.img的第一个block，紧接着将bin/kernel复制到bin/ucore.img第二个block开始的位置

# >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

$(call finish_all)

IGNORE_ALLDEPS	= clean \
				  dist-clean \
				  grade \
				  touch \
				  print-.+ \
				  handin

ifeq ($(call match,$(MAKECMDGOALS),$(IGNORE_ALLDEPS)),0)
-include $(ALLDEPS)
endif

# files for grade script

TARGETS: $(TARGETS)

.DEFAULT_GOAL := TARGETS

.PHONY: qemu qemu-nox debug debug-nox
qemu-mon: $(UCOREIMG)
	$(V)$(QEMU)  -no-reboot -monitor stdio -hda $< -serial null
qemu: $(UCOREIMG)
	$(V)$(QEMU) -no-reboot -parallel stdio -hda $< -serial null
log: $(UCOREIMG)
	$(V)$(QEMU) -no-reboot -d int,cpu_reset  -D q.log -parallel stdio -hda $< -serial null
qemu-nox: $(UCOREIMG)
	$(V)$(QEMU)   -no-reboot -serial mon:stdio -hda $< -nographic
TERMINAL        :=gnome-terminal
debug: $(UCOREIMG)
	$(V)$(QEMU) -S -s -parallel stdio -hda $< -serial null &
	$(V)sleep 2
	$(V)$(TERMINAL) -e "gdb -q -tui -x tools/gdbinit"
	
debug-nox: $(UCOREIMG)
	$(V)$(QEMU) -S -s -serial mon:stdio -hda $< -nographic &
	$(V)sleep 2
	$(V)$(TERMINAL) -e "gdb -q -x tools/gdbinit"

.PHONY: grade touch

GRADE_GDB_IN	:= .gdb.in
GRADE_QEMU_OUT	:= .qemu.out
HANDIN			:= proj$(PROJ)-handin.tar.gz

TOUCH_FILES		:= kern/trap/trap.c

MAKEOPTS		:= --quiet --no-print-directory

grade:
	$(V)$(MAKE) $(MAKEOPTS) clean
	$(V)$(SH) tools/grade.sh

touch:
	$(V)$(foreach f,$(TOUCH_FILES),$(TOUCH) $(f))

print-%:
	@echo $($(shell echo $(patsubst print-%,%,$@) | $(TR) [a-z] [A-Z]))

.PHONY: clean dist-clean handin packall tags
clean:
	$(V)$(RM) $(GRADE_GDB_IN) $(GRADE_QEMU_OUT) cscope* tags
	-$(RM) -r $(OBJDIR) $(BINDIR)

dist-clean: clean
	-$(RM) $(HANDIN)

handin: packall
	@echo Please visit http://learn.tsinghua.edu.cn and upload $(HANDIN). Thanks!

packall: clean
	@$(RM) -f $(HANDIN)
	@tar -czf $(HANDIN) `find . -type f -o -type d | grep -v '^\.*$$' | grep -vF '$(HANDIN)'`

tags:
	@echo TAGS ALL
	$(V)rm -f cscope.files cscope.in.out cscope.out cscope.po.out tags
	$(V)find . -type f -name "*.[chS]" >cscope.files
	$(V)cscope -bq 
	$(V)ctags -L cscope.files
