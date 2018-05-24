#include "asm.h"
#include "memlayout.h"
#include "mmu.h"

# Start the first CPU: switch to 32-bit protected mode, jump into C. 前512个字节
# The BIOS loads this code from the first sector of the hard disk into
# memory at physical address 0x7c00 and starts executing in real mode
# with %cs=0 %ip=7c00. 0x7c00是bios规定的，寄存器的值也是bios规定的

.code16                       # Assemble for 16-bit mode
.globl start
start:
  cli                         # BIOS enabled interrupts; disable

  # Zero data segment registers DS, ES, and SS.
  xorw    %ax,%ax             # Set %ax to zero
  movw    %ax,%ds             # -> Data Segment
  movw    %ax,%es             # -> Extra Segment
  movw    %ax,%ss             # -> Stack Segment 因为这些寄存器的值在运行完bios后是未知的，所以要清零

  # Physical address line A20 is tied to zero so that the first PCs
  # with 2 MB would run software that assumed 1 MB.  Undo that.
seta20.1: #保护模式介于实模式和32位模式之间
  inb     $0x64,%al               # Wait for not busy b字节
  testb   $0x2,%al
  jnz     seta20.1  #不等于时转移

  movb    $0xd1,%al               # 0xd1 -> port 0x64 11010001
  outb    %al,$0x64

seta20.2:
  inb     $0x64,%al               # Wait for not busy
  testb   $0x2,%al
  jnz     seta20.2

  movb    $0xdf,%al               # 0xdf -> port 0x60
  outb    %al,$0x60

  #虚拟地址： 段号：偏移，xv6几乎没用段模式，一个段有基址，边界，保护位
  #gdt让所有段都是基址位0且边界4GB

  # Switch from real to protected mode.  Use a bootstrap GDT that makes
  # virtual addresses map directly to physical addresses so that the
  # effective memory map doesn't change during the transition.
  lgdt    gdtdesc #將gdt地址設爲gdtdesc
  movl    %cr0, %eax
  orl     $CR0_PE, %eax# protection enable
  movl    %eax, %cr0 #給cr0或上一位，從此進入實模式

//PAGEBREAK!
  # Complete the transition to 32-bit protected mode by using a long jmp
  # to reload %cs and %eip.  The segment descriptors are set up with no
  # translation, so that the mapping is still the identity mapping.
  ljmp    $(SEG_KCODE<<3), $start32 #进入32位 格式：ljmp %cs %eip %cs=8 cs和gdt有关吗?
  # cs是选代码段的寄存器，段选择子(segment selector),从gdt，内核态中选取下标为1的段

.code32  # Tell assembler to generate 32-bit code now.
start32:
  # Set up the protected-mode data segment registers
  movw    $(SEG_KDATA<<3), %ax    # Our data segment selector=16 这个16指的到底是哪一个段 类似的，gdt，内核态中选取下标2的段
  movw    %ax, %ds                # -> DS: Data Segment所有段都指向同一个段号
  movw    %ax, %es                # -> ES: Extra Segment
  movw    %ax, %ss                # -> SS: Stack Segment
  movw    $0, %ax                 # Zero segments not ready for use
  movw    %ax, %fs                # -> FS
  movw    %ax, %gs                # -> GS

  # Set up the stack pointer and call into C.
  movl    $start, %esp #0x7c00是栈顶，向下扩展至0x0000，内存现状：0x7c00～0x7d00
  call    bootmain

  # If bootmain returns (it shouldn't), trigger a Bochs bochs是一种老的，慢的但是可以跨架构模拟的虚拟机
  # breakpoint if running under Bochs, then loop.
  movw    $0x8a00, %ax            # 0x8a00 -> port 0x8a00
  movw    %ax, %dx
  outw    %ax, %dx
  movw    $0x8ae0, %ax            # 0x8ae0 -> port 0x8a00
  outw    %ax, %dx
spin:
  jmp     spin

# Bootstrap GDT
.p2align 2                                # force 4 byte alignment
gdt:
  SEG_NULLASM                             # null seg #为什么要有一个空段 反正索引都是对的，人家就是有
  SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff)   # code seg
  SEG_ASM(STA_W, 0x0, 0xffffffff)         # data seg #数据区没有写标记 自带读标记

gdtdesc:
  .word   (gdtdesc - gdt - 1)             # sizeof(gdt) - 1 #为什么要减一呢？ fixme 这一位是段表的长度参数吗 一個段有64bits,4bytes
  .long   gdt                             # address gdt

