# The xv6 kernel starts executing in this file. This file is linked with
# the kernel C code, so it can refer to kernel symbols such as main().
# The boot block (bootasm.S and bootmain.c) jumps to entry below.
        
# Multiboot header, for multiboot boot loaders like GNU Grub.
# http://www.gnu.org/software/grub/manual/multiboot/multiboot.html
#
# Using GRUB 2, you can boot xv6 from a file stored in a
# Linux file system by copying kernel or kernelmemfs to /boot
# and then adding this menu entry:
#
# menuentry "xv6" {
# 	insmod ext2
# 	set root='(hd0,msdos1)'
# 	set kernel='/boot/kernel'
# 	echo "Loading ${kernel}..."
# 	multiboot ${kernel} ${kernel}
# 	boot
# }

#include "asm.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"

# Multiboot header.  Data to direct multiboot loader.
.p2align 2 # 地址的低2位应该是0，否则会插入0补齐
.text
.globl multiboot_header
multiboot_header:
  #define magic 0x1badb002
  #define flags 0
  .long magic
  .long flags
  .long (-magic-flags)

# By convention, the _start symbol specifies the ELF entry point.
# Since we haven't set up virtual memory yet, our entry point is
# the physical address of 'entry'.
.globl _start
_start = V2P_WO(entry) # 在kernel.ld里面显示这里是kernel的开始

# Entering xv6 on boot processor, with paging off.
.globl entry # 我们是怎样把entry塞进elfhdr里面的
entry:
  # Turn on page size extension for 4Mbyte pages 开启超过4KB的页
  # 可是我们的分页大小就是4KB啊，哪里用了4MB的了 entrypgdir就是4MB的
  movl    %cr4, %eax
  orl     $(CR4_PSE), %eax
  movl    %eax, %cr4
  # Set page directory
  movl    $(V2P_WO(entrypgdir)), %eax # 这里必须是物理地址，因为还不知道如何翻译
  movl    %eax, %cr3
  # Turn on paging. 打开分页
  movl    %cr0, %eax
  orl     $(CR0_PG|CR0_WP), %eax # 分页，写保护
  movl    %eax, %cr0

  # Set up the stack pointer.
  movl $(stack + KSTACKSIZE), %esp

  # Jump to main(), and switch to executing at
  # high addresses. The indirect call is needed because
  # the assembler produces a PC-relative instruction fixme 普通的jump和eip到底有什么关系
  # for a direct jump.
  #  a register-indirect absolute jump using the EAX register. 会被解释成物理地址而不是虚拟地址
  mov $main, %eax
  jmp *%eax

.comm stack, KSTACKSIZE # .comm 表示这个符号会和别的同名符号合并，并且开够4MB空间

//gerw done