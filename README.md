# OperatingSystem

## 第0天
　　这一天的内容主要是作者对于写书动机、如何开发操作系统以及接下来30天要做的内容的介绍，作者的风格是大致讲解基础概念，直接上手开发，先做出demo来，然后逐步添加功能，在开发中理解概念，从错误中学习经验。
## 第1天
- 使用BZ（二进制编辑器）制作软盘映像文件，一开始跟着作者在BZ里输入代码，结果发现书里的代码不全。所以，只能直接从光盘内容里复制粘贴。在cmd界面，输入“run”指令，“操作系统”会在QEMU中运行，可以看到如下界面


<div align=center><img src="image\helloos0.png" height="340" width="500">
<div align=left>

- 作者开发了一个汇编语言编译器“nask”，通过汇编重写上节的代码，再进行加工润色，加上注释等。通过nask.exe将nas文件转化为img文件，即可通过上节的方法启动操作系统。

> 一些缩写及解释
DB: define byte
DW: define word
DD: define double-word
RESB: reserve byte
$: 当前行的字节数

## 第2天
- 对汇编代码的功能解释，即调用显卡在屏幕上输出字符。

> 汇编指令
  ORG origin 指定程序的装载地址
  JMP jump 跳转
  “entry:” 标号，代表地址
  MOV move 传送字或字节
  ADD add 加法
  CMP compare 比较
  JE jump if equal 等于转移
  INT interrupt 中断  利用INT可以调用BIOS中的函数
  HLT halt CPU睡眠

> 寄存器称及作用解释
  AX: accumulator 累加寄存器
  CX: counter 计数寄存器
  DX: data  数据寄存器
  BX: base 基址寄存器
  SP: stack pointer 栈指针寄存器
  BP: base pointer 基址指针寄存器
  SI: source index 源变址寄存器
  DI: destination index 目的变址寄存器
  AL: accumulator low 累加寄存器低位
  CL: counter low 计数寄存器低位
  DL: data low 数据寄存器低位
  BL: base low 基址寄存器低位
  AH: accumulator high 累加寄存器高位
  CH: counter high 计数寄存器高位
  DH: data high 数据寄存器高位
  BH: base high 基址寄存器高位
  EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI分别是是AX, CX, DX, BX, SP, BP, SI, DI的扩展，低16位即为对应寄存器
  ES: extra segment 附加段寄存器
  CS: code segment 代码段寄存器
  SS: stack segment 栈段寄存器
  DS: data segment 数据段寄存器
  FS: segment part 2 没有名称 （extra segment 附加段寄存器）
  GS: segment part 3 没有名称 （extra segment 附加段寄存器）

  - 版本变为helloos04，利用原有代码删除启动区以外部分，制作512字节的启动区ipl。利用磁盘映像管理工具eding.exe在空白的磁盘映像文件写入ipl.bin的内容。
  - 版本进入helloos05，利用Makefile替代批处理文件，Makefile比批处理文件更加智能，它能根据情况判断需要执行的指令，避免了每次需要重新生成已有的文件。这样内容得到了精简，功能反而更强大了，不过遗憾的是作者对Makefile和make.bat中的一些命令并未加以解释。

  ## 第3天
  - 由于操作系统过于简陋，名称被作者改为“harrib”(纸娃娃)，然后添加了汇编语言编写的读盘程序。将从软盘读取的数据填满了内存0x08200 ~ 0x34fff的地方，至此启动区的制作完成。

  > 新增汇编指令
    JC: jump if carry 有进位转移
    JNC: jump if not carry 无进位转移
    JAE: jump if above or equal 大于等于时转移
    JBE: jump if below or equal 小于等于时转移
    JB: jump if below 小于时转移
    EQU: equal 类似于C语言的#define
    RET: return 返回

  - 作者写了以一个非常简短的“操作系统”只有以下三行代码

  ```
  fin:
      HLT
      JMP fin
  ```

  用nask编译生成haribote.sys，然后利用edimg将其保存到磁盘映像里。使用BZ查看haribote.sys和haribote.img,可以发现
  >
    1. 文件名会写在0x002600以后的地方
    2. 文件的内容会写在0x004200以后的地方

  - 在进行到harib00g的时候发现错误，由于之前编写读取磁盘的代码时未将`JNC fin`改为`JNC next`，故运行时并不会出现全黑画面，经过与作者代码对比改正。

  - 切换到32位模式就无法调用BIOS功能，所以切换之前，需要修改haribote.nas,记录画面模式，取得键盘信息。
  - 切换到32位模式，开始使用C语言进行编程。为了实现C语言程序调用，作者在haribote.nas中添加了100行左右的汇编代码同时将其改名为asmhead.nas，不过由于程序很复杂作者会在后面的内容中在对其进行讲解。下面是作者在第三天编写的bootpack.c

  ```cpp
  void io_hlt(void);
  void HariMain(void)
  {

  fin:
        io_hlt();
  	  goto fin;

  }

  ```

  > 由bootpack生成机器语言的过程
    1. 使用cc1.exe从bootpack.c生成bootpack.gas。
    2. 使用gas2nask.exe从bootpack.gas生成bootpack.nas。
    3. 使用nask.exe从bootpack.nas生成bootpack.obj。
    4. 使用obi2bim.exe从bootpack.obj生成bootpack.bim。
    5. 使用bim2hrb.exe从bootpack.bim生成bootpack.hrb。
    6. 这样就做成了机器语言，再使用copy指令将asmhead.bin与bootpack.hrb单纯结合到起来，就成了haribote.sys。

- 为了让计算机处于HALT状态，作者用汇编语言写了io_hlt函数。至此，第三天结束。
## 第四天
- 向naskfun.nas中添加write_mem8函数，使得能够在C语言中使用此函数来向内存中的指定地址写入数据。在与C语言联合使用时，只有`EAX`,`ECX`,`EDX`三个寄存器可以自由使用。通过向VRAM中写入数据，可以显示出条纹图案来。这段代码中还增加了`INSTRSET`指令，是用来告诉nask这个程序是给486使用的。
- 下一节里，作者讲解了C语言中指针的多种用法来取代write_mem函数。
>
  char *p  用于BYTE类地址
  short *p 用于WORD类地址
  int *p   用于DWORD类地址

- 以下三种写法等价
```c
for (i = 0xa0000; i <= 0xaffff; i++) {
      p = (char *)i;
      *p = i & 0x0f;
}
```
```c
p = (char *) 0xa0000;
for (i = 0; i <= 0xaffff; i++) {
      *(p + i) = i & 0x0f;
}
```
```c
p = (char *) 0xa0000;
for (i = 0; i <= 0xffff; i++) {
      p[i] = i & 0x0f;
}
```
**小Tips**：加法运算可以交换顺序，于是(p+i)和(i+p)，p[i]和i[p]，a[2]和2[a]都是一个意思，这更能说明它们与数组没有关系。
- 在给颜色编号时，作者使用加了static的数组声明来节省赋值语句花费的空间
**情况一**
```c
char a[3];
a[0] = 1;
a[1] = 2;
a[2] = 3;
```
翻译成汇编如下：
```x86asm
a:
    RESB 3
    之后是赋值语句
```
**情况二**
```c
static char a[3] = {0x01, 0x02, 0x03};
```
翻译成汇编如下:
```x86asm
a:
    DB 0x01, 0x02, 0x03
```

- io_out8函数可向设备中发送信号，使用此函数设定调色板。设定调色板时，需要先屏蔽中断，再恢复中断。
> CLI clear interrupt 清中断允许位
  STI set interrupt 置中断允许位

- 此处介绍了`EFLAGS`寄存器
![](image\eflags.png)
利用io_load_eflags函数保存eflags的值, 处理结束后再调用io_store_eflags恢复。不过CPU中没有`MOV EAX, EFLAGS`之类的指令，能够用来读写EFLAGS的只有 **PUSHFD（push flags double-word，将标志位的值按双字压入栈）** 和 **POPFD(pop flags double-word，按双字长将标志位从栈弹出)**指令。因此以上两个函数的汇编代码是这样的
```x86asm
_io_load_eflags:        ; int io_load_eflags(void);
        PUSHFD          ; 指PUSH EFLAGS
        POP     EAX
        RET
_io_store_eflags:       ; void io_store_eflags(int eflags);
        MOV     EAX, [ESP+4]
        PUSH    EAX
        POPFD           ; 指POP EFLAGS
        RET
```
**注意**：io_load_eflags的返回值从何得到？根据C语言的规约，执行RET时，EAX中的值就是返回值。

- 像素坐标(x,y)对应的VRAM地址按下式计算
  `0xa0000 + x + y * 320`
  根据上式我们可以在窗口320x200个像素中随意填充啦，用画矩形的函数得到的效果是这样的
  ![](image\rectangle.png)
  最终的界面如下
  ![](image\final.png)

## 第五天
- 从asmhead.nas中取vram等信息，将其放到一个结构体中
```c
struct BOOTINFO {
	char cyls, leds, vmode, reserve;
	short scrnx, scrny;
	char *vram;
};
```
将显示画面背景的部分独立出来，单独做成一个函数init_screen。
- 导入字体数据hankaku.txt,然后用makefont.exe将此文本输出成16x256=4096字节的文件。编译后生成hankaku.bin文件，加上连接所必须的接口信息变成目标文件（由bin2obj.exe来完成），相当于以下汇编语言
```x86asm
_hankaku:
    DB 各种数据（共4096字节）
```
这些操作需要在MAKEFILE中增加代码
在C语言中使用字体时，需要写上`extern char hankaku[4096];`，像这样在源程序之外的数据都需要加上`extern`。
> OSASK 的 字 体 数 据 ， 依 照 一 般 的 ASCⅡ 字 符 编 码 ， 含 有 256 个 字 符 。 A 的 字 符 编 码 是 0x41, 所以 A 的 字 体 数 据 ， 放 在 “hankaku + 0x41 * 16 ” 开 始 的 16 字 节 里 。 C 语 言 中 A 的 字 符 编 码 可 以 用 ’ A’ 来 表 示 ， 正 好 可 以 用 它 来 代 替 0x41 ， 所 以 也 可 以 写 成 “hankaku+’A’* 16 ” 。

显示如下
![](image\ABC123.png)
- C语言中字符串都是以0x00结尾的，所以我们可以方便的编写一个显示字符串的函数
```c
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	extern char hankaku[4096];
	for(; *s != 0x00; s++) {
		putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
		x += 8;
	}
	return;
}
```
- 使用`sprintf`函数输出变量的值，`sprintf`和`printf`最大的区别是在于它不是按指定格式输出，只是将输出内容作为字符串写在内存中，它在制作者的精心设计下能够不使用操作系统的任何功能，使用时需要在程序开头写上`#include<stdio.h>`，格式类似printf
![](image\format.png)
- 现实鼠标指针的方法，类似于显示字符。
- 为了让CPU动起来，我们需要GDT 和 IDT的初始化。它们都是有关CPU的设定，为了让操作系统能够使用32位模式，需要对CPU进行各种设定。
> GDT global (segment) descriptor table 全局段号记录表
  GDTR global (segment) descriptor table register 全局段号记录表寄存器
  IDT interrupt descriptor table  中断记录表
- 因为段寄存器是16位，所以可以处理0~65535范围的数，但由于CPU设计上的问题，段寄存器的低3位不能使用。故可以定义8192个段，设定这些段需要8192x8=65536字节。这64KB被写入内存中，称为GDT，然后将内存的起始地址和有效设定个数放在CPU内被称作GDTR的特殊寄存器中，设定就完成了。使用鼠标需要中断机制，为了实现中断机制，必须设定IDT，其记录了0~255的终端号码与函数调用的关系，设定方法与GDT很相似。

## 第六天
- 首先分割源文件，整理Makefile，整理头文件。
- 对第五天中naskfunc.nas新增函数的解释
```x86asm
_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT 	[ESP+6]
		RET
```
这个函数用来将指定的段上限和地址赋值给名为GDTR的48位寄存器，其并不能用MOV来赋值，只能给它指定一个内存地址，让它从指定的地址读取6个字节，然后赋值给GDTR寄存器。
> 该寄存器的低16位（即内存的最初2个字节）是段上限，它等于“GDT的有效字节数 - 1”。剩下的高32位（即剩余的4个字节），代表GDT的开始地址。在最初执行这个函数的时候，DWORD[ESP + 4]里存放的是段上限，DWORD[ESP+8]里存放的是地址。具体到实际的数值，就是0x0000ffff和0x00270000。把它们按字节写出来的话，就成了[FF FF 00 00 00 00 27 00]（要注意低位放在内存地址小的字节里 ）。为了执行LGDT，笔者希望把它们排列成[FF FF 00 00 27 00]的样子，所以就先用“MOV AX,[ESP + 4]”读取最初的0xffff，然后再写到[ESP + 6]里。这样，结果就成了[FF FF FF FF 00 00 27 00]，如果从[ESP + 6]开始读6字节的话，正好是我们想要的结果。

  dsctbl.c里的set_segmdesc函数
```c
struct SEGMENT_DESCRIPTOR {
short limit_low, base_low;
char base_mid, access_right;
char limit_high, base_high;
};
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar)
{
if (limit > 0xfffff) {
ar |= 0x8000; /* G_bit = 1 */
limit /= 0x1000;
}
sd->limit_low = limit & 0xffff;
sd->base_low = base & 0xffff;
sd->base_mid = (base >> 16) & 0xff;
sd->access_right = ar & 0xff;
sd->limit_high = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
sd->base_high = (base >> 24) & 0xff;
return;
}
```
  这个函数是按照CPU的规格要求，将段的信息归结成8个字节写入内存。

  首先是段的地址。地址用32位表示，称为段的基址（base）。在这里分为low（2字节），mid（1字节）和high（1字节）3段，合起来32位，是为了与80286时代的程序兼容才分为三段。

  段上限最大4GB即32位，但只能使用20位，指定到1MB为止。为了扩大指定段的大小，段属性里设置了一个标位Gbit（G：granularity）。当这个标志位为1时，limit的单位不解释成字节（byte），而解释成页（page，4KB），这样4KBx1M=4GB。20位的段上限分别写到limit_low和limit_high里。

  段属性，又称为“段的访问控制权属性”，在程序中用变量名access_right或ar来表示。因为12位段属性中的高4位放在limit_high的高4位里，所以程序里有意把ar当做如下的16位构成来处理：
  `xxxx0000xxxxxxxx`(其中x是1或1)

  ar的高4位被称为“扩展访问权”，由“GD00”构成，G指G bit，D指段的模式，1时指32位模式，0是指16位模式，通常使用D=1的模式。

  低8位简单介绍
  > 00000000 （ 0x00 ） ：未使用的记录表（descriptor table ）。
    10010010 （ 0x92 ） ：系统专用，可读写的段。不可执行。
    10011010 （ 0x9a ） ：系统专用，可执行的段。可读不可写。
    11110010 （ 0xf2 ） ：应用程序用，可读写的段。不可执行。
    11111010 （ 0xfa ） ：应用程序用，可执行的段。可读不可写。

  CPU是处于系统模式还是应用模式，取决于执行中的应用程序是位于访问权位为0x9a的段，还是位于访问权位0xfa的段。

- 初始化PIC(programmable interrupt controller)。PIC是将8个中断信号集合成一个中断信号的装置，为了处理更多的中断信号增加了另一个PIC。与CPU直接相连的PIC称为主PIC（master PIC），与主PIC相连的PIC称为从PIC（slave PIC）。主PIC负责处理第0到第7号中断信号，从PIC负责处理第8到第15号中断信号。
![](image\pic.png)
```c
void init_pic(void)
/* PIC的初始化 */
{
io_out8(PIC0_IMR, 0xff ); /* 禁止所有中断 */
io_out8(PIC1_IMR, 0xff ); /* 禁止所有中断 */
io_out8(PIC0_ICW1, 0x11 ); /* 边沿触发模式（edge trigger mode） */
io_out8(PIC0_ICW2, 0x20 ); /* IRQ0-7由INT20-27接收 */
io_out8(PIC0_ICW3, 1 << 2); /* PIC1由IRQ2连接 */
io_out8(PIC0_ICW4, 0x01 ); /* 无缓冲区模式 */
io_out8(PIC1_ICW1, 0x11 ); /* 边沿触发模式（edge trigger mode） */
io_out8(PIC1_ICW2, 0x28 ); /* IRQ8-15由INT28-2f接收 */
io_out8(PIC1_ICW3, 2 ); /* PIC1由IRQ2连接 */
io_out8(PIC1_ICW4, 0x01 ); /* 无缓冲区模式 */
io_out8(PIC0_IMR, 0xfb ); /* 11111011 PIC1以外全部禁止 */
io_out8(PIC1_IMR, 0xff ); /* 11111111 禁止所有中断 */
return;
}
```
**IMR（interrupt mask register）**“是中断屏蔽寄存器”，8位对应8路信号，如果某一位是1则对应IRQ（interrupt request）信号被屏蔽。**ICW(initial control word)**是“初始化控制数据”，有4个。ICW1和ICW4与PIC主板配线方式、中断信号的电气特性有关，ICW3是有关主-从连接的设定，对主PIC而言，第几号IRQ与从PIC相连，是用8位来设定的，最多驱动8个从PIC，对从PIC而言，该从PIC与主PIC的第几号相连，用3位来设定。因此不同的操作系统可以进行独特设定的就只有ICW2了，它决定了IRQ以哪一号中断通知CPU，我们以INT 0x20~0x2f接收中断号IRQ0~15。INT 0x00~0x1f用于CPU内部自动产生的保护通知。
- 鼠标是IRQ12，键盘是IRQ1，所以编写了用于INT 0x2c和INT 0x21的中断处理程序。由于中断处理完成之后，还需要返回，即执行`IRETD（interrupt return DWORD）`指令，所以中断返回程序的正常运行还需要借助汇编语言。
```c
/*int.c的节选*/
void inthandler21(int *esp)
/* 来自PS/2键盘的中断 */
{
struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 21 (IRQ-1) : PS/2 keyboard");
for (;;) {
io_hlt();
}
}
```
```x86asm
; naskfunc.nas节选
EXTERN _inthandler21, _inthandler2c
_asm_inthandler21:
PUSH ES
PUSH DS
PUSHAD
MOV EAX,ESP
PUSH EAX
MOV AX,SS
MOV DS,AX
MOV ES,AX
CALL _inthandler21
POP EAX
POPAD
POP DS
POP ES
IRETD
```
其中`PUSHAD（push all data）`相当于
> PUSH EAX
PUSH ECX
PUSH EDX
PUSH EBX
PUSH ESP
PUSH EBP
PUSH ESI
PUSH EDI

`POPAD（pop all data）`相当于相反的操作
`CALL`是调用函数的指令
- 最后将asm_inthandler21注册到IDT中去
```c
/* IDT的设定 */
set_gatedesc(idt + 0x21, (int) asm_inthandler21, 2 * 8, AR_INTGATE32);
set_gatedesc(idt + 0x2c, (int) asm_inthandler2c, 2 * 8, AR_INTGATE32);
```
> asm_inthandler21注册在idt的第0x21号。这样，如果发生中断了，CPU就会自动调用asm_inthandler21。这里的2 * 8表示的是asm_inthandler21属于哪一个段，即段号是2，乘以8是因为低3位有着别的意思，这里低3位必须是0。
  ```c
  set_segmdesc(gdt + 2, LIMIT_BOTPAK, ADR_BOTPAK, AR_CODE32_ER);
  ```
  程序中的以上语句说明号码为2的段正好涵盖了整个bootpack.hrb.
  最后的AR_INTGATE32将IDT的属性，设定为0x008e。它表示这是用于中断处理的有效设定.

在HariMain的最后，修改了PIC的IMR，这样就能接受来自鼠标和键盘的中断了。

## 第七天
- 为了获取按键编码，需要设置缓冲区。按下或松开键盘时，会产生两个中断，分别发送两个按键码。一开始，由于只设置了一个字符的缓冲区。在程序处理第一个中断时，第二个按键码就被发送了，由于还未开中断，所以第二个中断产生的按键码被丢弃。再扩大缓冲区，同时使用FIFO队列后，可以记录大量的数据，执行速度也得到了提高。
- 由于鼠标只要一动就会产生中断，所以主板上虽然有鼠标用的电路，但只要不执行激活鼠标的指令，就不会产生鼠标的中断信号。想要使用鼠标，要想让鼠标控制电路有效，然后再让鼠标有效。事实上，鼠标控制电路包含在键盘控制电路里，如果键盘控制电路的初始化正常完成，鼠标控制电路的激活也就完成了。键盘控制电路没有CPU电路快，所以我们需要一个`wait_KBC_sendready`函数来确认是否可以往键盘控制电路传送信息。如果键盘控制电路可以接受CPU指令了，CPU从设备号码0x0064处所得的数据的倒数第二位应该是0。将模式设置为鼠标模式后，鼠标控制电路就准备完成，然后发送鼠标激活指令，待鼠标收到指令后，就会回复信息0xfa，这样鼠标中断就可以收到了。
- 鼠标数据的取得方法与键盘完全相同，要靠中断号码来区分数据来自键盘还是鼠标。由于鼠标往往会更快地送出大量数据，所以我们将它的FIFO缓冲区增加到128字节，编译运行后，可以显示出来自键盘和鼠标的数据。

    ![](image\mousedata.png)

## 第八天
- 鼠标移动每次会产生三个字节的数据，第八天一开始的工作是将鼠标的数据构建了一个结构体，然后整理了有关代码，将数据显示在屏幕上，如下图

    ![](image\mousedata1.png)
第一位会在0~3的范围内变化，第二位会在点击鼠标时发生变化，三四位与鼠标的左右移动有关，五六位与鼠标的上下移动有关。
- 然后向鼠标结构体中增加几个变量用于存放解读结果，x，y，btn，分别用于存放移动信息和鼠标按键状态。加上了对第一字节的判断，以防止鼠标出现接触不良等情况导致数据出问题。鼠标按键的状态，存放在buf[0]的低三位。x和y的第八位及以后位全部设成1或全部都保留为0，就能正确的解读x和y。鼠标与屏幕的y方向正好相反，对y进行取反操作。通过如下代码，鼠标就能移动了
    ```c
    } else if (fifo8_status(&mousefifo) != 0) {
        i = fifo8_get(&mousefifo);
        io_sti();
        if (mouse_decode(&mdec, i) != 0) {
        /* 数据的3个字节都齐了，显示出来 */
        sprintf(s, "[lcr %4d %4d]", mdec.x, mdec.y);
        if ((mdec.btn & 0x01) != 0) {
            s[1] = 'L';
        }
        if ((mdec.btn & 0x02) != 0) {
            s[3] = 'R';
        }
        if ((mdec.btn & 0x04) != 0) {
            s[2] = 'C';
        }
        boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 32, 16, 32 + 15 * 8 - 1, 31);
        putfonts8_asc(binfo->vram, binfo->scrnx, 32, 16, COL8_FFFFFF, s);
        /* 鼠标指针的移动 */
        boxfill8(binfo->vram, binfo->scrnx, COL8_008484, mx, my, mx + 15, my + 15); /* 隐藏鼠标 */
        mx += mdec.x;
        my += mdec.y;
        if (mx < 0) {
            mx = 0;
        }
        if (my < 0) {
            my = 0;
        }
        if (mx > binfo->scrnx - 16) {
            mx = binfo->scrnx - 16;
        }
        if (my > binfo->scrny - 16) {
            my = binfo->scrny - 16;
        }
        sprintf(s, "(%3d, %3d)", mx, my);
        boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 0, 79, 15); /* 隐藏坐标 */
        putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, s); /* 显示坐标 */
        putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16); /* 描画鼠标 */
        }
    }
    ```
    但这时还有一个小问题，由于没考虑到叠加处理，鼠标移动时，会将任务栏消除，这个问题会在后面解决。

    ![](image\mousemove.png)

- 作者终于要把前面挖的坑给填了，asmhead.nas中的代码，在此节中给出了解释。

```x86asm
; PIC关闭一切中断
;   根据AT兼容机的规格，如果要初始化PIC，
;   必须在CLI之前进行，否则有时会挂起，
;   随后进行PIC的初始化

        MOV     AL,0xff
        OUT     0x21,AL
        NOP                     ; 如果连续执行OUT指令，有些机种会无法正常运行
        OUT     0xa1,AL

        CLI                     ; 禁止CPU级别的中断
```

上面这段汇编代码功能相当于下面的C语言代码

```c
io_out8(PIC0_IMR,  0xff  ); /* 禁止主PIC的全部中断 */
io_out8(PIC1_IMR,  0xff  ); /* 禁止从PIC的全部中断 */
Io_cli(); /*禁止CPU级别的中断*/
```

为了让CPU能够访问1MB以上的内存空间，设定A20GATE

```x86asm
        CALL    waitkbdout
        MOV     AL,0xd1
        OUT     0x64,AL
        CALL    waitkbdout
        MOV     AL,0xdf         ; enable A20
        OUT     0x60,AL
        CALL    waitkbdout
```

等同于下面的代码

```c
#define KEYCMD_WRITE_OUTPORT     0xd1
#define KBC_OUTPORT_A20G_ENABLE  0xdf

    /* A20GATE的设定*/
    Wait_KBC_sendready();
    Io_out8(PORT_KEYCMD, KEYCMD_WRITE_OUTPORT);
    Waite_KBC_sendready();
    Io_out8(PORT_KEYDAT, KBC_OUTPORT_A20G_ENABLE);
    Waite_KBC_sendready();     /*这句话是为了等待完成执行指令*/
```

这样A20GATE信号线变成ON的状态，内存的1MB以上的部分就变成可用状态了。

接下来通过LGDT设定临时GDT，将CR0的值代入EAX切换到保护模式，这里牵扯到的CR0(control register 0)是一个特殊的寄存器，只有操作系统才能操作它

![](image\cr0.jpg)

当我们将它的最高位置为0，最低位置为1时，就进入到了不分页的保护模式。

这之后的代码基本都在执行memcpy的功能，在内存中传送各种数据，系统的内存分布图如下:

![](image\memdtr.png)

- 这里作者讲的有点不清不楚的，有几个地方没搞懂，所以梳理了一下流程及各文件的作用，以期对内容有更深入的理解

![](image\order.png)

ipl10.nas负责读盘到内存，读完盘之后跳转到0xc200处，即asmhead.nas。  
asmhead.nas负责显示模式及分辨率设置即从16位转换为32位保护模式，是C语言和汇编语言的桥梁，执行到最后就转到bootpack.c的主函数。  
bootpack.c包含主函数，做好初始化各种操作然后接收键盘和鼠标信息，并对信息进行解析，呈现在屏幕上，此外还有字库，鼠标显示，鼠标位置计算等等。这里用到了GDT和IDT，对消息的接收要用中断，对消息的保存要用FIFO缓冲区。  
dsctbl.c与GDT、IDT分段及中断相关。  
graphic.c负责设置调色板，显示字符串，绘制矩形，绘制鼠标，绘制窗口，大部分与VRAM相关的函数都在这里。  
int.c包括一些中断处理函数，但是这些并不是中断的真正入口，真正入口是对应的汇编函数，这里的函数是对应的汇编函数要调用的。这些函数处理键盘中断和鼠标中断，为了加速处理，要把收到的数据放进FIFO缓冲区，防止因为在中断中进行大量的处理而影响速度。  
fifo.c这是一个简单的用数组形式建立的FIFO循环链表。  
hankaku.txt是一个字库。  
naskfunc.nas有一些不能用C语言写的函数，必须用汇编语言，都放在这个文件里。

**ipl10.nas**

这部分主要负责先载入 启动扇区（0x7c00 ~ 0x7dff）
启动扇区载入完毕之后，将 IPL 软盘中除了启动扇区的那一块）载入到内存的 0x8200 ~ 0x34fff 区域（这里实际上只是载入了 10 个柱面 180KB）
跳转到 0xc200 处运行（这个地方实际就是 asmhead 的开始位置）

**asmhead.nas**

这部分的主要工作在之前已经提到过了
他将0x7c00 ~ 0x7dff 处的512byte的启动扇区数据拷贝到 0x100000 处（1MB 空间开始位置）
然后将 0x8200 ~ 0x34fff 处的IPL数据拷贝到 0x100200 处
同时将bootpack.hrb 的数据拷贝到0x280000位置处
最后跳转到 bootpack.hrb 处开始执行

**bootpack.c**

这部分实际就是我们 C 语言部分的操作， 用于处理键盘，鼠标中断响应， 图形绘制等操作

## 第九天
- 因为bootpack.c的代码又变的复杂了，再次整理代码，将设计键盘和鼠标的函数分别放到keyboard.c和mouse.c里。
- 进行内存管理首先要搞清楚内存有多大，需要写代码去检查内存。在检查内存的时候要先检测是否是486以后的CPU，如果是就暂时让CPU的高速缓存功能无效，否则写入和读出都是在缓存中进行的，结果所有的“内存”一切正常。可是在用如下函数去检查内存时，得到的内存容量是3072MB而不是设定的32MB

```c
unsigned int memtest_sub(unsigned int start, unsigned int end)
{
	unsigned int i, *p, old, pat0= 0xaa55aa55, pat1 = 0x55aa55aa;
	for(i = start; i <= end; i += 0x1000) {
		p = (unsigned int *) (i + 0xffc);
		old = *p;  //先记住修改前的值
		*p = pat0; //试写
		*p ^= 0xffffffff; //反转
		if (*p != pat1) { //检查反转结果
not_memory:
			*p = old;
			break;
		}
		*p ^= 0xffffffff; //再次反转
		if (*p != pat0) { //检查值是否恢复
			goto not_memory;
		}
		*p = old;  //恢复为修改前的值
	}
	return i;
}
```

这是什么原因造成的呢？原来是由于编译器太“聪明”了，在编译时它进行了最优化处理，结果我们的代码相当于下面这样子

```c
unsigned int memtest_sub(unsigned int start, unsigned int end)
{
unsigned int i;
for (i = start; i <= end; i += 0x1000) { }
return i;
}
```

也就是说，根本没有进行内存检查。

- 如果更改编译选项，停止最优化处理，是可以解决上述问题的，但我们在其他地方还是需要最优化处理的。这时候就需要我们的好帮手汇编语言出场了，将memtest_sub函数用汇编语言改写，这次可以得出正确的结果了。

- 作者采用了一种列表结构去存储内存管理信息，构建可用内存空间管理表，有关功能的编写类似于链表。这种方式占用内存少，分配和释放都很迅速，虽然管理程序变复杂了，但相比于效率的提高，这是值得的。还有一个缺点是，当可用空间变得零零散散，怎么都归纳不到一块儿时，会将空间管理信息全部用完。这时就要割舍掉小块内存（本节中是割舍掉了新来的信息），为了不在这个问题上纠结太久，作者采用了一种做法，即“割舍掉的东西，只要以后还能找回来，就暂时不去管它。”，留待以后解决。  

## 第十天
- 如果反复使用`memman_alloc`和`memman_free`进行以一字节为单位的内存管理，内存中就会出现很多不连续的小段未使用空间，这样就回吧man->frees耗尽。此次增加了两个以4KB为单位进行内存分配和释放的函数`memman_alloc_4k`和`memman_free_4k`来方便内存管理，使用了 **i = (i + 0xfff) & 0xfffff000** 的巧妙方法来进行向上舍入。
- 为了实现窗口的叠加，引入了图层（sheet）的概念，通过移动图层的方法实现鼠标指针和窗口的移动。新增函数位于sheet.c中，一开始的程序中，只要有移动就会对整个画面进行重绘，这无形中增加了许多不必要的开销（可能是由于电脑性能的提高，我并未观察到作者所说的现象）。通过增加一个`sheet_refreshsub`函数，我们可以指定重绘的范围从而提高速度。
## 第十一天
- 修改`sheet_refreshsub`功能，让它不刷新画面外的部分，使得鼠标能够移动到画面外。
- 精简函数，省略shtctl的指定。
- 描绘出一个窗口，构建方式与之前的方法相同，关闭按钮类似于鼠标指针，边框利用`boxfill8`函数画出，文字利用`putfont8_asc`放置。效果如下
![](image\myos.png)

- 将之前的窗口改成一个计数器，但在计数器工作时，会出现闪烁的现象。这是由于在刷新的时候，总是先刷新refresh范围内的背景图层，然后再刷新窗口图层，就会造成闪烁。如图
![](image\flash.png)
解决方法是仅对refresh对象及其以上的图层进行刷新，这样不仅避免了闪烁现象，还减小了刷新的开销。
- 但是我们的闪烁现象并未完全解决，将鼠标放在计数器的数字上面时，鼠标又闪烁起来了。想要解决此问题，需要在刷新窗口时避开鼠标所在的地方对VRAM进行写入处理，开辟一块内存，大小和VRAM一样，称之为map。之所以叫map是因为这块内存用来表示画面上的点是哪个图层的像素，所以它就相当于是图层的地图。
![](image\map.png)
在map写入信息，今后程序会对照map内容来向VRAM中写入。
## 第十二天
- 定时器对于操作系统非常重要，它的原理是每隔一段时间就发送一个中断信号给CPU，只要在中断处理程序中数一数定时器中断发生的次数就可以计算出经过了多长时间。
- 要在电脑中管理定时器，需要设定PIT（Programmable Interval Timer），PIT连接着IRQ0，所以设定了PIT就可以设定IRQ0的中断间隔。
    > IRQ0的中断周期变更:  
    AL=0x34:OUT(0x43,AL);  
    AL=中断周期的低8位; OUT(0x40,AL);  
    AL=中断周期的高8位; OUT(0x40,AL);  
    到这里告一段落。  
    如果指定中断周期为0，会被看作是指定为65536。实际的中断产生的频率是单位时间时钟周期数（即主频）/设定的数值。比如设定值如果是1000，那么中断产生的频率就是1.19318KHz。设定值是10000的话，中断产生频率就是119.318Hz。再比如设定值是11932的话，中断产生的频率大约就是100Hz了，即每10ms发生一次中断。

    下面的C语言代码即用于进行中断周期的变更
    ```c
    #define PIT_CTRL 0x0043
    #define PIT_CNT0 0x0040
    void init_pit(void)
    {
    io_out8(PIT_CTRL, 0x34);
    io_out8(PIT_CNT0, 0x9c);
    io_out8(PIT_CNT0, 0x2e);
    return;
    }
    ```
    同时我们还需要中断处理程序:
    ```c
    void inthandler20(int *esp)
    {
    io_out8(PIC0_OCW2, 0x60); /* 把IRQ-00信号接收完了的信息通知给PIC */
    /* 暂时什么也不做 */
    return;
    }
    ```
    ```x86asm
    _asm_inthandler20:
    PUSH ES
    PUSH DS
    PUSHAD
    MOV EAX,ESP
    PUSH EAX
    MOV AX,SS
    MOV DS,AX
    MOV ES,AX
    CALL _inthandler20
    POP EAX
    POPAD
    POP DS
    POP ES
    IRETD
    ```
    为了把这个中断处理程序注册到IDT，init_gdtidt函数中也要加上
    ```c
    set_gatedesc(idt + 0x20, (int) asm_inthandler20, 2 * 8, AR_INTGATE32);
    ```
    处理方法基本与之前的键盘和鼠标中断相同。

- 作者定义了一个名为TIMERCTL的结构体（目测以后还会对其进行扩展），改写init_pit和inthandler20增加了计数功能，数字以每秒钟100的速度增加，效果(不过看不出)如下
![](image\counter.png)
- 果不其然现在TIMERCTL变成了如下的样子
    ```c
    struct TIMERCTL {
    unsigned int count;
    unsigned int timeout;
    struct FIFO8 *fifo;
    unsigned char data;
    };
    ```
    至于为什么要这样做，是因为我们想要实现“超时”（timeout）功能。以上结构体中的timeout用来记录离超时还有多长时间，一旦这个剩余时间达到0，程序就往FIFO缓冲区里发送数据，定时器就是通过这种方法通知HariMain时间到了。
    写个程序试一下，在其中设定10秒钟以后向tiemerfifo写入“1”这个数据，而timerfifo收到数据时，就会在屏幕上显示“10[sec]"。
    ![](image\timeout.png)

- 操作系统中的很多地方，例如光标闪烁、时间显示等等都需要定时器，所以我们要设定多个定时器，定义如下结构体，修改相关函数
    ```c
    #define MAX_TIMER 500
    struct TIMER {
    unsigned int timeout, flags;
    struct FIFO8 *fifo;
    unsigned char data;
    };
    struct TIMERCTL {
    unsigned int count;
    struct TIMER timer[MAX_TIMER];
    };
    ```
    编写程序测试一下效果，如下:
    ![](image\manycounters.png)

- 由于inthandler20花费了很长时间来进行中断处理，妨碍了其它中断处理程序的执行，使得操作系统反应迟钝，所以作者用了很大的篇幅来讲怎么对其进行优化，从而加快中断处理。

## 第十三天
- 简化字符串显示，将一些常用的功能整理为一个函数。
- 重新调整FIFO缓冲区，将多个定时器的缓冲区合并为一个，通过往FIFO内写入不同数据来分辨出是哪个定时器超时。
- 性能真的提高了吗？-性能测试

    设置count变量，完全不显示计数，全力执行“cout++”，通过cout的值来测试系统性能。需要注意，必须在启动3秒后把count复位为0一次。

    > **起动 3 秒后，将 count 置为 0 的原因**  
    首先考虑一下这个命令的意思吧。起动3秒后把count复位至0，这与从3秒后开始计数是一样的。画面上要到10秒以后才显示，这样测试的时间就是7秒钟。  
    事实上，笔者最初并没有加入“count=0;”语句。但那样做的结果是，在真机上测定harib10d时，最高值和最低值的差值竟然达到了150054。这可了不得呀。差值这么大，即使我们比较harib10c和harib10d，也不知道哪个更快。  
    对于这样的结果，笔者曾茫然不知所措，差一点要放弃性能比较。但后来笔者忽然想起，只要某些条件稍微有些变化，电脑初始化所花费的时间就会有很大变化。这就是为什么我们在起动后3秒钟之内不进行测试的原因。这样做之后，误差急剧减小，终于可以比较结果了，真是太好了。 

    通过测试可以看出，系统的性能的确得到了提高。

- 再次调整FIFO缓冲区，将从键盘和鼠标输入的数据也归入计时器的FIFO中
    > 0～ 1 ………………… 光标闪烁用定时器  
    3 ………………… 3秒定时器  
    10 ………………… 10秒定时器  
    256～ 511 ………………… 键盘输入（从键盘控制器读入的值再加上256）  
    512～ 767 …… 鼠标输入（从键盘控制器读入的值再加上512）

    这样用一个FIFO缓冲区就可以正常进行处理了。

- 加快中断处理，之前的inthandler20和timer_settime中引入了太多的移位处理，浪费时间。此次修改通过链表来管理定时器，消除了移位处理。添加“哨兵”元素，简化链表插入时的复杂情况。

## 第十四天
- 继续测试性能
    添加以下函数
    ```c
    void set490(struct FIFO32 *fifo, int mode)
    {
        int i;
        struct TIMER *timer;
        if (mode != 0) {
            for (i = 0; i < 490; i++) {
                timer = timer_alloc();
                timer_init(timer, fifo, 1024 + i);
                timer_settime(timer, 100 * 60 * 60 * 24 * 50 + i * 100);
            }
        }
        return;
    }
    ```
    这样我们就设定了490个定时器。
    > 真机上运行结果的比较  
    (1) 追加490个定时器时的值 set490（&fifo, 1）;  
    harib11a:0096521077 ……harib10g  里加入 set490  的时候( 有移位)  
    harib11b:0096522038 ……harib10h  里加入 set490  的时候( 没有移位、没有哨兵)  
    harib11c:0096522097 ……harib10i  里加入 set490  的时候( 没有移位、有哨兵)  
    (2) 不追加490个定时器时的值 set490（&fifo, 0）;  
    harib11a:0096522095 ……harib10g  里加入 set490  的时候( 有移位)  
    harib11b:0096522038 ……harib10h  里加入 set490  的时候( 没有移位、没有哨兵)  
    harib11c:0096522101 ……harib10i  里加入 set490  的时候( 没有移位、有哨兵)  
    (3) 参考：不加入set490语句时的值  
    harib10g:0099969263 ……( 有移位)  
    harib10h:0099969184 ……( 没有移位、没有哨兵)  
    harib10i:0099969264 ……( 没有移位、有哨兵)  

    这里有一个有趣的现象，对于（2)和（3），处理上虽然完全相同，而结果却相差了345万左右，原因是什么呢？
    >这不是程序内容的问题，而是C编译器的问题。实际
    上，由于跳转目标地址不同，CPU的JMP指令执行的时钟周期数也不相同。在HariMain中，循环执行“count++；”的for语句虽然最终被编译为JMP指令执行，但如果前面加上“set490（&fifo,0）；”语句，那么以后各个指令的地址也都会相应地错开几个字节，结果造成JMP指令的地址也略有变化。因此执行时间也稍稍延迟，执行结果大约变差了3%.
    ```c
    for（;;） ｛                L2：
        count++;                count++;
        任意语句;          ->    任意语句;
    }                      JMP     L2
            这个L2的地址一旦变化，JMP的执行时间就变化！
    ```

- 提高分辨率

    使用VBE（VESA BIOS extension）可以切换到新画面模式，切换时要用"**AX = 0x4f02；BX = 画面模式号码**", VBE的的画面模式号码如下:
    >0x101 ……640× 480× 8bit  彩色  
    0x103 ……800× 600× 8bit  彩色  
    0x105 ……1024× 768× 8bit  彩色  
    0x107 ……1280× 1024× 8bit
    
    实际指定的时候，要将以上的画面模式号码值加上0x4000，再赋值到BX中去

    如果想在真机上运行我们则需要进行一系列的检查
    ```x86asm
    ; 确认VBE是否存在

		MOV		AX,0x9000
		MOV		ES,AX
		MOV		DI,0
		MOV		AX,0x4f00
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320

    ; 检查VBE的版本

		MOV		AX,[ES:DI+4]
		CMP		AX,0x0200
		JB		scrn320			; if (AX < 0x0200) goto scrn320

    ; 取得画面模式信息

		MOV		CX,VBEMODE
		MOV		AX,0x4f01
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320

    ; 画面模式信息的确认

		CMP		BYTE [ES:DI+0x19],8
		JNE		scrn320
		CMP		BYTE [ES:DI+0x1b],4
		JNE		scrn320
		MOV		AX,[ES:DI+0x00]
		AND		AX,0x0080
		JZ		scrn320		; 模式属性的bit7是0，所以放弃

    ; 画面模式的切换

		MOV		BX,VBEMODE+0x4000
		MOV		AX,0x4f02
		INT		0x10
		MOV		BYTE [VMODE],8	; 记录画面模式
		MOV		AX,[ES:DI+0x12]
		MOV		[SCRNX],AX
		MOV		AX,[ES:DI+0x14]
		MOV		[SCRNY],AX
		MOV		EAX,[ES:DI+0x28]
		MOV		[VRAM],EAX
		JMP		keystatus

    scrn320:
		MOV		AL,0x13			; VGA图、320x200x8bit彩色
		MOV		AH,0x00
		INT		0x10
		MOV		BYTE [VMODE],8	; 记录画面模式
		MOV		WORD [SCRNX],320
		MOV		WORD [SCRNY],200
		MOV		DWORD [VRAM],0x000a0000
    ```
    >在画面模式信息中，重要的信息有如下6个。
    WORD [ES : DI+0x00] :  模式属性……bit7  不是 1  就不好办( 能加上 0x4000)  
    WORD [ES : DI+0x12] : X  的分辨率  
    WORD [ES : DI+0x14] : Y  的分辨率  
    BYTE  [ES : DI+0x19] :  颜色数……必须为 8  
    BYTE  [ES : DI+0x1b] :  颜色的指定方法……必须为 4 (4  是调色板模式)  
    DWORD [ES : DI+0x28] : VRAM  的地址  
    在这6项信息当中，我们来确认如下3项：  
      颜色数是否为8  
      是否为调色板模式  
      画面模式号码可否加上0x4000再进行指定  

- 键盘输入

    查询下面的表，表里的值是按下键时的数值，在表里数值的基础上加0x80就可以得到键弹起时的数值。
    ![](image\keyboardtable.png)
    在HariMain中添加一些代码我们甚至已经可以实现下面的效果了
    ![](image\hellobupt.png)

- 窗口移动
  ```c
  if ((mdec.btn & 0x01) != 0) {
  /* 按下左键、移动sht_win */
  sheet_slide(sht_win, mx - 80, my - 8);
  }
  ```

  只用了短短几行代码我们就实现了窗口移动的功能，不过，它也有缺点，比如窗口会移动到鼠标点击的地方，而不只是拖动。

## 第十五天

- 任务切换，向CPU发出任务切换指令时，CPU会先把寄存器中的值全部写入内存中，这样做是为了当以后切换回这个程序的时候可以继续运行。为了把寄存器中的内容写入内存里去，我们需要任务状态段，即TSS（task status segment），TSS也是内存段的一种，需要在GDT中进行定义后使用
  ```c
  //TSS的结构
  struct TSS32 {
  int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
  int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
  int es, cs, ss, ds, fs, gs;
  int ldtr, iomap;
  };
  ```
  从开头的backlink起，到cr3为止的几个成员，保存的不是寄存器的数据，而是与人物设置相关的信息，在执行任务切换的时候这些成员不会被写入（backlink除外，某些情况下是会被写入的）  
  第二行的成员是32位寄存器，第三行是16位寄存器  
  EIP用来记录下一条需要执行的指令位于内存中哪个地址，当下次再返回这个任务的时候，CPU就可以明白应该从哪里读取程序来运行了。  
  ldtr和iomap也和第一行的成员一样，是有关人物设置的部分。

  JMP指令分为两种，只改写EIP的称为near模式，同时改写EIP和CS的称为far模式，在此之前我们使用的JMP指令基本上都是near模式的。下面这条之前写过的就是far模式的JMP指令
  ```c
  JMP  DWORD 2*8:0x0000001b
  ```
  这条指令在向EIP存入0x1b的同时，将CS置为2*8（=16）。

  创建TSS，将他们加入GDT，通过编写相关函数我们就能进行任务的切换了。

  ## 第十六天
- 由于多任务是一个操作系统非常重要的一部分，所以我们继续对多任务进行优化，仿造struct SHTCTL增添了一个struct TASKCTL。
```c
struct TASKCTL {
    int running; /*正在运行的任务数量*/
    int now; /*这个变量用来记录当前正在运行的是哪个任务*/
    struct TASK *tasks[MAX_TASKS];
    struct TASK tasks0[MAX_TASKS];
};
```
新增函数
> task_init: 将一个程序变成任务  
task_alloc: 初始化一个任务结构  
task_run: 将task添加到tasks的末尾，然后使running加一  
task_swicth: 当running为1时，不需要进行任务切换，函数直接结束。当running大于等于2时，先把now加1，然后把now所代表的任务切换成当前任务，最后再将末尾的任务移动到开头。  
task_sleep: 让任务休眠，如果要唤醒任务只需执行task_run。

- 在任务优先级的处理上，作者使用了多级队列调度的方式，如下图
![](image\multiplequeue.png)
这样在多任务的处理上，高优先级的任务，会得到更多的资源，避免了卡顿现象
![](image\multiplewindow.png)
## 第十七天
- 在之前的多任务管理中，如果任务A下面的LEVEL中有任务B0~B2，在任务A进入休眠状态后，mtask.c将自动寻找下层LEVEL中的任务，但如果我们没有启动B0~B2，程序就找不到其他的任务而导致运行出现异常。因此我们引入了一个"idle"(闲置)任务，并把它一直放在最下层的LEVEL中，这样一来，即便任务A进入休眠状态，系统也会自动切换到这个闲置任务，问题就得到解决了。而且闲置任务也很简单，它的功能只是执行HTL，代码如下
```c
void task_idle(void)
{
    for (;;) {
        io_hlt();
    }
}
```

- 创建命令行窗口，首先做一个新的任务命名为console_task，保留原先的task_a，结果如下
![](image\console1.png)

- 上面只是做了个窗口雏形，因此我们得让它接受字符输入，现在我们输入的字符都会跑到任务A的窗口中去，为了能够往命令行窗口中输入字符，我们要让系统在按下"Tab"键的时候，将输入窗口切换到命令行窗口上去。首先实现窗口标题栏颜色的切换，效果如下
![](image\console2.png)

- 想要向console_task的FIFO发送数据，需要知道FIFO的内存地址，基本上没有什么任务是完全用不到FIFO的，因此我们在struct TASK中加入struct FIFO
```c
struct TASK {
    int sel, flags; /* sel代表GDT编号 */
    int level, priority;
    struct FIFO32 fifo; /*这里！ */
    struct TSS32 tss;
};
```
功能的实现参照原先的代码，运行效果如下
![](image\console3.png)

- 经过之前的努力，我们可以输入英文、数字和符号了，但还无法输入“！”和“%”。为了能够输入这两个符号，我们必须要处理Shift键，Shift编码如下

    |       |按下|抬起|
    |-------|----|----|
    |左Shift|0x2a|0xaa|
    |右Shift|0x36|0xb6|
    准备一个key_shift变量，当左Shift按下时置为1，右Shift按下时置为2，两个都不按时置为0，两个都按下时置为3。使用两个不同的字符集，当key_shift为0时，我们用keytable0[]将按键编码转换为字符编码，而当key_shift不为0时，则使用keytable1[]进行转换。

运行结果如下  
![](image\console4.png)

- 接下来要实现区分大小写字母的输入，我们要同时判断Shift键和CapsLock键的状态
    >CapsLock 为 为 OFF & Shift  键为 OFF  → 小写英文字母  
    CapsLock 为 为 OFF & Shift  键为 ON  → 大写英文字母  
    CapsLock 为 为 ON & Shift  键为 OFF  → 大写英文字母  
    CapsLock 为 为 ON & Shift  键为 ON  → 小写英文字母

    在asmhead.nas中，我们已经从BIOS获取到了键盘的状态，保存到了binfo->leds中
    >binfo->leds  的第 4  位 → ScrollLock  状态  
    binfo->leds  的第 5  位  → NumLock  状态  
    binfo->leds  的第 6  位  → CapsLock  状态

    只要使用上述数据，我们就可以处理大小写字母的输入了。

    运行一下，效果如图
    ![](image\console5.png)

- 通过编码表，我们可以得到：
    >0x3a: CapsLock  
    0x45: NumLock  
    0x46: ScrollLock

    当我们接收到上述按键编码时，只要将binfo->leds中对应的位置改写就可以了。到这里我们已经实现了锁定键模式的切换，不过键盘上的指示灯却不会变化，下面是关于键盘指示灯的控制
    > 关于LED的控制  
     对于NumLock和CapsLock等LED的控制，可采用下面的方法向键盘发送指令和数据。  
     读取状态寄存器，等待 bit 1的值变为 0。  
     向数据输出（0060）写入要发送的 1个字节数据。  
     等待键盘返回 1个字节的信息，这和等待键盘输入所采用的方法相同（用 IRQ等待或者用轮询状态寄存器 bit 1的值直到其变为 0都可以）。  
      返回的信息如果为 0xfa，表明 1个字节的数据已成功发送给键盘。如为 0xfe则表明发送失败，需要返回第 1步重新发送。  
      要控制LED的状态，需要按上述方法执行两次，向键盘发送EDxx数据。其中，xx的bit0代表ScrollLock，bit 1代表NumLock，bit 2代表CapsLock（0表示熄灭，1表示点亮）。bit 3～7为保留位，置0即可。

    通过合理的编写程序就可以对键盘指示灯进行控制了。

## 第十八天
- 控制光标闪烁，将按下Tab键时的处理以及光标闪烁的处理改写了，当不想显示光标的时候，使cursor_c为负值。
- 使命令行窗口支持回车和滚动
- 命令行支持mem命令，用来显示内存使用情况；cls命令，清空屏幕内容；dir命令，显示文件信息。
- 以上除dir命令外涉及到的内容都不多，对于dir命令，要显示文件名信息，我们需要读取磁盘的内容，这得借助BIOS的帮助。在进入32位模式之前，我们已经从磁盘中读了10个柱面的内容。
    >那么已经读出来的这些数据，存放在内存中的什么地方呢？在8.5节中写得很清楚，是 0x00100000～0x00267fff。其中存放文件名的地方又在哪里呢？其实我们也已经说过了，不过大家可能都不记得了吧，参考3.5节从0柱面、0磁头、1扇区开始的0x002600之后，也就是内存地址的0x00102600开始写入。

做个试验，在磁盘映像中加入haribote.sys、ipl10和make.bat三个文件。
磁盘映像中0x002600字节以后的部分，内容如下
```
       +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F 0123456789ABCDEF
-------------------------------------------------------------------------
002600 48 41 52 49 42 4F 54 45 53 59 53 20 00 00 00 00 HARIBOTESYS ....
002610 00 00 00 00 00 00 18 74 FF 32 02 00 68 6B 00 00 .......t.2..hk..
002620 49 50 4C 31 30 20 20 20 4E 41 53 20 00 00 00 00 IPL10 NAS ....
002630 00 00 00 00 00 00 59 7A 42 41 38 00 95 0B 00 00 ......YzB58.....
002640 4D 41 4B 45 20 20 20 20 42 41 54 20 00 00 00 00 MAKE BAT ....
002650 00 00 00 00 00 00 F6 10 81 30 3E 00 2E 00 00 00 .........0>.....
002660 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
```

这里的内容是以32字节为单位循环的，这32个字节的结构如下
```c
struct FILEINFO {
    unsigned char name[8], ext[3], type;
    char reserve[10];
    unsigned short time, date, clustno;
    unsigned int size;
};
```

开始的8个字节是文件名，文件名不足8个字节时用空格补足，如果文件名的第一个字节为0xe5，代表这个文件已经被删除了；文件名第一个字节为0x00，代表这一段不包含任何文件名信息。接下来的3个字节是扩展名，和文件名一样不足3个字节时用空格不足。后面1个字节存放文件的属性信息，一般的文件不是0x20就是0x00。

属性信息说明如下
```
0x01······只读文件（不可写入）
0x02······隐藏文件
0x04······系统文件
0x08······非文件信息（比如磁盘名称等）
0x10······目录
```

当一个文件既是只读文件又是隐藏文件时，将上面的对应值加算即可，即0x03。
接下来的10个字节为保留，下面2个字节为WORD整数，存放文件的时间。接下来的2个字节也是WORD整数，代表这个文件的内容从磁盘上的哪个扇区开始存放，是簇号（cluster number），最后的4个字节为DWORD整数，存放文件的大小。

执行一下我们写好的命令，结果如图
![](image\cmd1.png)
![](image\cmd2.png)
![](image\cmd3.png)

## 第十九天
- 实现type命令，首先我们有一个公式 **磁盘映像中的地址 = clustno * 512 + 0x003e00**，有了这个公式就可以找到文件存放的位置，接下来只要将文件的内容逐字节读取出来并显示在屏幕上就可以了。在输出时，要注意对制表符，换行符和回车符的处理，否则输出就会错位。特殊字符编码如下
```
0x09 ……制表符：显示空格直到 x  被 4  整除为止
0x0a ……换行符：换行
0x0d ……回车符：忽略
```
- 对FAT的支持，按照Windows管理磁盘的方法，保存大于512字节的文件时，有时候并不是存入连续的扇区中。对于文件的下一段存放在哪里，磁盘是有记录的，这个记录存储在从0柱面、0磁头、2扇区开始的9个扇区中，在磁盘映像中相当于0x000200 ~ 0x0013ff，这个记录被称为FAT（file allocation table）。不过FAT使用了微软的算法进行压缩，需要用下面的方法进行解压缩

    F0 FF FF → FF0 FFF  
    ab cd ef → dab efc


```
       +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F 0123456789ABCDEF
-----------------------------------------------------------------------
000200 F0 FF FF 03 40 00 05 60 00 70 80 00 09 A0 00 0B ....@..`........
000210 C0 00 0D E0 00 0F 00 01 11 20 01 13 40 01 15 60 ......... ..@..`
000220 01 17 80 01 19 A0 01 1B C0 01 1D E0 01 1F 00 02 ................
000230 21 20 02 23 40 02 25 60 02 27 80 02 29 A0 02 2B .......&.29.....
000240 C0 02 2D E0 02 2F 00 03 31 20 03 33 40 03 35 60 ..-../..1 .3@5.`
000250 03 37 80 03 39 F0 FF 3B C0 03 3D E0 03 3F F0 FF .7..9..;..=..?..
000260 FF 0F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
000270 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
```

对上面的FAT进行解压缩的话就会得到如下的数据
```
    +0  +1  +2  +3  +4  +5  +6  +7  +8  +9
------------------------------------------
 0 FF0 FFF 003 004 005 006 007 008 009 00A
10 00B 00C 00D 00E 00F 010 011 012 013 014
20 015 016 017 018 019 01A 01B 01C 01D 01E
30 01F 020 021 022 023 024 025 026 027 028
40 029 02A 02B 02C 02D 02E 02F 030 031 032
50 033 034 035 036 037 038 039 FFF 03B 03C
60 03D 03E 03F FFF FFF 000 000 000 000 000
```

以haribote.sys为例，已知clustno = 2，因此读取0x004200 ~ 0x0045ff这512个字节，接下来看FAT的第2号记录，其值为003，也就是说下面的部分存放在clustno = 3这个位置，以此类推，我们一直读取到clustno = 57，而在FAT的第57号记录中存储的是FF
F，也就是说57号之后已经没有数据了，即这里就是文件的末尾。**一般来说，如果遇到FF8 ~ FFF的值，就代表文件数据到此结束**。

由于FAT文件非常重要，微软在磁盘中存放了2份FAT，第一份位于0x000200 ~ 0x0013ff，第二份位于0x001400 ~ 0x0025ff，其中第二份是备份，内容与第一份完全相同。

- 整理一下bootpack.h开始写第一个应用程序
```x86asm
[BITS 32]
    CLI
fin:
    HLT
    JMP fin
```
将上面这段代码保存为hlt.nas，然后用nask进行汇编，生成hlt.hrb。这个扩展名hrb就相当于Windows下的exe。将文件的内容读到内存，我们为其创建一个内存段，接下来只要goto到该段中的程序，程序就会开始运行了。

## 第二十天
- 实现API
![](image\api.png)
在应用程序对API执行CALL的时候，要使用far-CALL，如下
```x86asm
CALL    2*8:0xbe3
```
因此返回的时候必须使用相应的far-RET，即RETF指令
```x86asm
_asm_cons_putchar:
    （中略）
        RETF ; 这里！
```
- 可是如果我们修改了操作系统的代码，_asm_cons_putchar的地址会随着改变，这样我们的应用程序也要重写，这显然是不现实的。IDT中最多可以设置256个函数，IRQ使用了16个，CPU用于通知异常状态的中断最多也只有32种，因此还剩下很多没有使用的项，所以我们就从IDT中找一个空闲的项用一下，我们就选0x40号（0x30 ~ 0xff均可），并将_asm_cons_putchar注册在这里。这样用INT 0x40就可代替原来的地址调用，返回指令也要相应的改成IRETD。用INT调用时，对于CPU来说相当于执行了中断处理程序，因此在调用的同时CPU会自动执行CLI指令来禁止中断请求，但我们并不需要禁止中断，因此需要在开头添加一条STI指令。
- 增加两个用于显示字符串的API，如果一直用之前的方式将他们设置为中断函数的话256个项目的IDT很快就会被用光。这里借鉴BIOS的调用方式，在寄存器中存入功能号，用EDX来存放功能号，就可以设置最多达42亿个API函数。
```
功能号 1……显示单个字符（AL = 字符编码）  
功能号 2……显示字符串 0（EBX = 字符串地址）  
功能号 3……显示字符串 1（EBX = 字符串地址，ECX = 字符串长度）
```
改写_asm_cons_putchar
```x86asm
_asm_hrb_api:
        STI
        PUSHAD ; 用于保存寄存器值的PUSH
        PUSHAD ; 用于向hrb_api传值的PUSH
        CALL _hrb_api
        ADD ESP,32
        POPAD
        IRETD
```
用C语言编写的API处理程序如下
```c
void hrb_api(int edi, int esi, int ebp, int esp, int ebx, int edx, int ecx, int eax)
{
    struct CONSOLE *cons = (struct CONSOLE *) *((int *) 0x0fec);
    if (edx == 1) {
        cons_putchar(cons, eax & 0xff, 1);
    } else if (edx == 2) {
        cons_putstr0(cons, (char *) ebx);
    } else if (edx == 3) {
        cons_putstr1(cons, (char *) ebx, ecx);
    }
    return;
}
```
## 第二十一天
- 在显示单个字符时，我们用[CS:ECX]的方式特意指定了CS，因此可以成功读取msg的内容，但在显示字符串时，由于无法指定段地址，程序误以为是DS而从完全错误的内存地址中读取了内容，所以显示就会出错。hrb_api并不知道代码段的起始位置位于内存的哪个地址，但cmd_app应该知道，因为代码段是由cmd_app设置。由于没有办法从cmd_app向hrb_api直接传递数据，所以我们像之前保存cons的地址一样，将它放在0xfe8，然后用类似与**cons_putstr0(cons, (char *) ebx + cs_base);**这样的调用方法就能正确读取内容了。
- 我们希望做到用C语言编写应用程序，例如
```c
a.c

void api_putchar(int c);
void HariMain(void)
{
    api_putchar('A');
    return;
}
```
api_putchar函数，需要用汇编语言编写，功能是向EDX和AL赋值，并调用INT 0x40，
```x86asm
a_nask.nas

[FORMAT "WCOFF"] ; 生成对象文件的模式
[INSTRSET "i486p"] ; 表示使用486兼容指令集
[BITS 32] ; 生成32位模式机器语言
[FILE "a_nask.nas"] ; 源文件名信息
            GLOBAL _api_putchar
[SECTION .text]
_api_putchar:   ; void api_putchar(int c);
        MOV     EDX,1
        MOV     AL,[ESP+4] ; c
        INT     0x40
        RET
```
这里的api_putchar需要与a.c的编译结果进行连接。

不过这样生成的hrb还是不能运行，我们需要将a.hrb开头的6个字节替换成“E8 16 00 00 00 CB”。这六个字节就相当于下面三行代码汇编之后的结果
```x86asm
[BITS 32]
        CALL 0x1b
        ETF
```
也就是先调用0x1b这个地址的函数，从函数返回后再执行far-RET，这里的0x1b就是.hrb文件中HariMain的地址。

添加如下的代码就可让我们在读取数据到内存中时，先修改前六个字节再运行
```c
if (finfo->size >= 8 && strncmp(p + 4, "Hari", 4) == 0) { /*从此开始*/
    p[0] = 0xe8;
    p[1] = 0x16;
    p[2] = 0x00;
    p[3] = 0x00;
    p[4] = 0x00;
    p[5] = 0xcb;
} /*到此结束*/
```
- 保护操作系统，有一些电脑病毒或恶意软件，或者是应用程序的bug，可能会对操作系统造成破坏，造成死机等情况，所以我们需要操作系统的保护机制。

例如
```c
void HariMain(void)
{
    *((char *) 0x00102600) = 0;
    return;
}
```
上述程序擅自访问了本该由操作系统来管理的内存空间，对于这种情况，我们需要为应用程序提供专用的内存空间，要做到这一点，我们可以创建应用程序专用的数据段，并在应用程序运行期间，将DS和SS指向该段地址。
```
操作系统用代码段……2 * 8
操作系统用数据段……1 * 8
应用程序用代码段……1003 * 8
应用程序用数据段……1004 * 8
（3 * 8～1002 * 8为TSS所使用的段）
```
接着作者写了很多用于段切换的函数，看的云里雾里的，结果到最后说CPU其实提供了这种进行段切换的功能，无需我们自己去写。
- 接下来实现强制结束程序，只要在中断号0x0d中注册一个函数即可，因为在x86架构规范中，当应用程序试图破坏操作系统时或者试图违背操作系统的设置时，就会自动产生0x0d中断，因此该中断也被称为“异常”。
- 操作系统会指定应用程序用的DS，因此破坏行为会发生异常，那么如果应用程序忽略操作系统指定的DS，用汇编语言直接将操作系统用的段地址存入DS的话，就可以破坏操作系统。我们要对这种情况进行处理，在段定义的地方，如果将访问权限加上0x60的话，就可以将段设置为应用程序用，当CS中的段地址为应用程序用段地址时，CPU会认为“当前正在运行应用程序”，这时如果存入操作系统用的段地址就会产生异常。如果使用这样的方法，就必须在TSS中注册操作系统用的段地址和ESP，这样才能实现内核态和用户态之间的切换，我们自己写的段切换也就不需要了。IDT中的INT 0x40的访问权限也要加上0x60，让它成为“可供应用程序作为API调用的中断”。

## 第二十二天
- 经过我们之前的一番设置，操作系统得到了很好的保护，一般的攻击手段已经不起作用了。

    在操作系统的内存空间里搞破坏行不通了，在定时器上动手脚呢？
```x86asm
[INSTRSET "i486p"]
[BITS 32]
        MOV AL,0x34
        OUT 0x43,AL
        MOV AL,0xff
        OUT 0x40,AL
        MOV AL,0xff
        OUT 0x40,AL
; 上述代码的功能与下面代码相当
; io_out8(PIT_CTRL, 0x34);
; io_out8(PIT_CNT0, 0xff);
; io_out8(PIT_CNT0, 0xff);
        MOV EDX,4
        INT 0x40
```
然而上述代码并不会对我们的操作系统造成破坏，当以应用模式运行时，执行IN指令和OUT指令都会产生一般保护异常。

如果执行CLI再HLT呢？
```x86asm
[INSTRSET "i486p"]
[BITS 32]
        CLI
fin:
        HLT
        JMP     fin
```
依旧不能破坏系统，当以应用程序模式运行时，执行CLI、STI和HLT这些指令都会产生异常。

操作系统里有一个用来CLI的函数，far-CALL这个函数呢？
```x86asm
[INSTRSET "i486p"]
[BITS 32]
        CALL 2*8:0xac1
        MOV EDX,4
        INT 0x40
```
还是不能对操作系统进行破坏，如果应用程序可以CALL任意地址的话，像这样就能破坏操作系统了，因此CPU规定除了设置好的地址以外，禁止应用程序CALL其他的地址。

但是如果我们对API进行修改的话还是可以破坏操作系统的，如果操作系统内部的API本身就有BUG，那我们用户就无能为力了，想要防止这种问题的发生，我们只能“不安装不可靠的操作系统”了。
- 帮助发现BUG
    如果我们写出了有bug的代码，如
    ```c
    void api_putchar(int c);
    void api_end(void);

    void HariMain(void)
    {
        char a[100];
        a[10] = 'A'; /*这句当然没有问题*/
        api_putchar(a[10]);
        a[102] = 'B'; /*这句就有问题了*/
        api_putchar(a[102]);
        a[123] = 'C'; /*这句也有问题了*/
        api_putchar(a[123]);
        api_end();
    }
    ````
    由于a是保存在栈中的，因此这次会产生栈异常。我们需要一个函数来处理栈异常，栈异常的中断号为0x0c。PS：根据CPU说明书，从0x00到0x1f都是异常所使用的中断。仿照inthandler0d函数，编写inthandler0c函数，然后将其注册到IDT中。在真机环境下，显示出“AB”之后才产生异常，为什么写入“B”不会产生异常呢？是因为虽然a[102]超出了数组的边界，但却没有超出为应用程序分配的数据段边界，而a[123]所在的地址已经超出了数据段的边界，因此写入“C”会产生异常。

    在异常处理程序中，加入如下代码
    ```c
    sprintf(s, "EIP = %08X\n", esp[11]);
    cons_putstr0(cons, s);
    ```
    其功能是，将esp的11号元素（即EIP）显示出来，这样我们就能知道引发异常的指令的地址。
    
    其他元素有
    ```x86asm
    esp[ 0] : EDI
    esp[ 1] : ESI esp[0 ～7] 为_asm_inthandler 中PUSHAD 的结果
    esp[ 2] : EBP
    esp[ 4] : EBX
    esp[ 5] : EDX
    esp[ 6] : ECX
    esp[ 7] : EAX
    esp[ 8] : DS esp[8 ～9] 为_asm_inthandler 中PUSH 的结果
    esp[ 9] : ES
    esp[10] : 错误编号（基本上是0，显示出来也没什么意思）
    esp[11] : EIP
    esp[12] : CS esp[10 ～15] 为异常产生时CPU 自动PUSH 的结果
    esp[13] : EFLAGS
    esp[14] : ESP （应用程序用ESP ）
    esp[15] : SS （应用程序用SS
    ```
- 强制结束应用程序

    如果我们运行一个，类似于下面这样的程序
```c
void HariMain(void)
{
    for (;;) { }
}
```
它将永远循环下去而无法结束，中断并没有被禁用，因此其他的任务还可以照常工作，不过这个任务总归要消耗一定的CPU运行时间，系统整体的速度就会变慢，因此我们需要强制结束应用程序的方法。

```C
if (i == 256 + 0x3b && key_shift != 0 && task_cons->tss.ss0 != 0) { /* Shift+F1 */
    cons = (struct CONSOLE *) *((int *) 0x0fec);
    cons_putstr0(cons, "\nBreak(key):\n");
    io_cli(); /*不能在改变寄存器值时切换到其他任务*/
    task_cons->tss.eax = (int) &(task_cons->tss.esp0);
    task_cons->tss.eip = (int) asm_end_app;
    io_sti();
```
上述程序的工作原理是，当按下强制结束键（shift + F1）时，改写命令行窗口任务的的寄存器值，并goto到asm_end_app。
```c
_asm_end_app:
; EAX为tss.esp0的地址
        MOV ESP,[EAX]
        MOV DWORD [EAX+4],0 ; 这里！
        POPAD
        RET ; 返回cmd_app
```
这样一来程序会被强制结束，但也有个问题，那就是当应用程序没有在运行的时候，按下强制结束键会发生误操作。所以我们必须要确认task_cons->tss.ss0不为0时才能继续进行处理。我们还得进行一些修改，使得当应用程序运行时，该值一定不为0；而当应用程序没有运行时，该值一定为0，为此，我们在在task_alloc加入`task->tss.ss0 = 0;`。

- 我们写好了显示字符串的API，然后要写一个C语言调用该API的函数，如下
```
_api_putstr0: ; void api_putstr0(char *s);
        PUSH EBX
        MOV EDX,2
        MOV EBX,[ESP+8] ; s
        INT 0x40
        POP EBX
        RET
```
但是我们在程序中使用该函数的时候，却不能显示字符。这是为什么呢?

在API函数中，显示字符串时需要字符串的首地址，若应用程序是通过bim2hrb生成的话，程序还会链接obj文件，生成的hrb文件分为两个部分——代码部分和数据部分。不过我们之前一直没有考虑数据部分，当程序中没有字符串和外部变量时，就会生成不含数据部分的hrb文件，因此之前的程序都不会出现问题。

.hrb文件的数据部分会在应用程序启动时被传送到应用程序用的数据段中，而.hrb文件中数据部分的位置则存放在代码部分的开头一块区域中，由bim2hrb生成的.hrb文件，开头的36个字节不是程序，而是存放了下列这些信息
```
0x0000 (DWORD)  …… 请求操作系统为应用程序准备的数据段的大小
0x0004 (DWORD)  …… “Hari”（.hrb 文件的标记）
0x0008 (DWORD)  …… 数据段内预备空间的大小
0x000c (DWORD)  …… ESP 初始值& 数据部分传送目的地址
0x0010 (DWORD)  …… hrb 文件内数据部分的大小
0x0014 (DWORD)  …… hrb 文件内数据部分从哪里开始
0x0018 (DWORD)  …… 0xe9000000
0x001c (DWORD)  …… 应用程序运行入口地址 - 0x20
0x0020 (DWORD)  …… malloc空间的起始地址
```
E9是JMP指令的机器码，跳转的偏移地址为应用程序入口地址 - 0x20。

当启动应用程序时，操作系统为应用程序分配了代码段和数据段，启动前应该先将hrb文件中的数据复制到数据段再执行应用程序。
- 显示窗口/在窗口中描绘字符和方块，操作系统已经有了显示窗口以及显示字符和方块的函数，可以在hrb_api中调用，只需要确定一下功能号并且正确传参即可实现显示窗口以及在窗口上显示文字的功能。
## 第二十三天
- 编写malloc函数，我们已经有了一个memman_alloc函数，但是因为memman_alloc函数分配的内存空间并不位于应用程序的数据段范围内，所以我们需要编写一个为应用程序分配内存空间的API函数，然后将malloc的内存空间首地址写到hrb文件中去，这样可以有效减小hrb文件的大小。

malloc函数要想实现分配内存的功能，那么操作系统就需要提前给应用程序预留一定大小的空间用于malloc，因为应用程序所需要的内存空间不好预测，最好的做法是用于给malloc分配的内存空间可以调整大小，但是在我们自制的操作系统中暂时没有实现这样的功能，只能给malloc函数预留一定大小的空间，这应该是一个需要优化的地方。实现malloc函数的同时，我们当然也要实现一个free函数

编写API函数malloc和free的思路：  
1. 使用memman_init函数初始化一个memman，参数中的地址要写应用程序段的地址
2. 使用memman_free函数释放空间，为应用程序的malloc预留这段空间
3. 使用memman_alloc函数可以为应用程序分配空间
4. 使用memman_free函数可以释放分配的内存空间
- 画点/画线，想要在任务的界面上显示一些内容，只需要在对应图层的buf中填写相应的内容，然后刷新图层即可。之前的图层SHEET定义如下：
```c
struct SHEET
{
	unsigned char *buf;  /*用来记录图层上所描画内容的地址*/
	int xsize,ysize,x,y,color,height,flags;
	struct SHTCTL *ctl;	
};
```
1. 若要在窗口上显示一个点，那么就在对应图层的buf中修改一个单元的内容即可。
2. 若要在窗口上显示多个点，那么就在对应图层的buf中修改多个单元的内容即可。
3. 可以画点就可以画线，只要确定了线的起点坐标和终点坐标，就可以画出这条线。

画线方法：

设起点（x0，y0），终点（x1，y1）

1. 求横纵坐标差值的绝对值：dx = |x1 - x0|，dy = |y1 - y0|
2. 比较dx和dy，确定点的个数len：  
        a. 若dx >= dy, len = dx + 1, dx = 1, dy = dy / len, 若x0 < x1, dy = -dy, 若y0 < y1, dy = -dy
        b. 若dx < dy, len = dy + 1, dy = 1, dx = dx / len, 若x0 < x1, dx = -dx, 若y0 < y1, dy = -dy

3. x = x0, y = y0
        按如下方式画len个点：  
        画点（x，y），x += dx, y += dy
- 现在我们可以在窗口上画点、画线了，不过一个问题出现了，在应用程序结束之后，窗口依然还留在画面上。这是因为系统为留在画面上的这个窗口分配了应用程序的数据段作为存放窗口图层的内存空间，当应用程序活动时没有任何问题，但当应用程序运行结束后，其数据段的内存空间就被释放出来，供操作系统及其他应用程序来使用，所以这样不行。在应用程序结束之前，我们需要先关闭窗口。该功能的实现也很简单，我们只需要在API函数api_closewin中调用sheet_free函数即可。
- 想要实现按下某个按键关闭窗口的功能，我们需要API实现函数get_key，其返回值为按键编码。我们通过循环不断从FIFO中取出数据直到FIFO为空或成功接收到键盘输入后循环结束，在此过程中FIFO还会收到诸如计时器光标ON、OFF之类的数据，我们需要简单地将它们处理掉。为了设置定时器，我们需要timer的地址，不过这是console_task中的变量，hrb_api是无法获取的，所以我们将定时器加入到struct CONSOLE中了，因为这个定时器是用来控制光标闪烁的，对于命令行窗口来说是必需的。这里的键盘输入API提供了阻塞和非阻塞两种方式，如果是非阻塞，则当FIFO为空时立即返回-1；如果是阻塞，则当FIFO为空时任务休眠并等待。
- 如果我们按Shift + F1强制结束程序的话，窗口还是会残留在界面上，所以我们在struct SHEET中添加一个用来存放task的成员，当应用程序结束时，查询所有的图层，如果图层的task为将要结束的应用程序，则关闭该图层。
## 第二十四天
- 首先实现用键盘鼠标切换窗口、用鼠标移动窗口和用鼠标关闭窗口的功能，这几个功能实现起来很简单。我们使用按键F11来作为窗口切换的按键，按下F11，我们就将从下面数第2个图层的高度提升为shtctl->top-1，shtctl->top这个高度存放的是最上面一个图层的高度，这个图层永远是绘制鼠标用的。使用鼠标切换时，我们按照图层从上到下的顺序寻找鼠标所指向的图层，最先找到的图层就是鼠标指向的图层，找到之后，将该图层的高度提升为shtctl->top-1。窗口移动是基于鼠标移动进行的，当鼠标按下鼠标左键时，鼠标位于所选中图层的标题栏，那么此时该图层跟随鼠标一起移动。只要记录下鼠标移动的横坐标以及纵坐标，就可以计算出其移动的距离，图层也移动同样的距离即可。
对于用鼠标关闭窗口的功能，只要判断鼠标按下时其坐标是否是在“X”按钮上，就可以实现。
- 虽然我们已经实现了让应用程序接受键盘输入的功能，不过仔细看画面发现，处于输入状态的其实是命令行窗口，我们想要让应用程序处于输入状态。之前我们所使用的Tab键切换很简单，只能在两个窗口之间交替进行，但是现在我们有3个以上的窗口，我们这样规定，按下Tab键时将键盘输入切换到当前输入窗口下面一层的窗口中，若当前窗口为最下层则切换到最上层窗口。当应用程序窗口处于输入模式时被关闭，我们让系统自动切换到最上层的窗口。  
我们用SHEET结构中的task成员来判断数据发送对象的FIFO，因此在sht_cons->task中也加入了TASK结构的地址，这样的话我们就无法分辨窗口是不是由应用程序生成的，于是我们需要通过SHEET结构中的flags成员的0x10比特位来分辨窗口是不是由应用程序生成的，当该比特位为1时，表示该窗口是应用程序窗口；使用SHEET结构中的flags成员的0x20比特位来分辨该窗口是否需要光标控制。
- 刚才为了实现用Tab键切换，我们修改了很多代码，这次我们只需要添加一点点代码就可以实现用鼠标切换输入窗口了，如下
```c
if (sht != key_win) {
        cursor_c = keywin_off(key_win, sht_win, cursor_c, cursor_x);
        key_win = sht;
        cursor_c = keywin_on(key_win, sht_win, cursor_c);
 }
```
- 关于定时器的部分，操作系统已经实现了定时器的功能，但是应用程序没有办法使用，我们可以使用API的方式使得应用程序也可以使用定时器。首先我们需要确定关于定时器操作的API的功能号以及参数，然后按照以前编写API的方式编写API。要注意应用程序结束之后一定要取消应用程序中的定时器，要不然该定时器就会给命令行本身发送数据，而命令行本身肯定是一头雾水。

## 第二十五天
### 增加蜂鸣器发声的功能
- 蜂鸣器的发声和定时器一样，是由PIT来控制的
>   蜂鸣器发声的控制  
　　  音高操作  
　　  AL = 0xb6; OUT(0x43, AL);  
　　  AL = 设定值的低位 8bit; OUT(0x42, AL);  
　　  AL = 设定值的高位 8bit; OUT(0x42, AL);  
　　  设定值为 0时当作 65536来处理。  
　　  发声的音高为时钟除以设定值，也就是说设定值为 1000时相当于发出 1.19318KHz的声音；设定值为 10000时相当于 119.318Hz。因此设定 2712即可发出约 440Hz的声音  
  蜂鸣器ON/OFF  
　　  使用 I/O端口 0x61控制。  
　　  ON：IN(AL, 0x61); AL |= 0x03; AL &= 0x0f; OUT(0x61, AL);  
　　  OFF：IN(AL, 0x61); AL &= 0xd; OUT(0x61, AL);

- 这里的时钟是PIT时钟而不是CPU时钟，频率恒定为1.19318MHz。API设计如下
> 蜂鸣器发声
EDX=20  
EAX=声音频率（单位是mHz，即毫赫兹）  
例如当EAX=4400000时，则发出440Hz的声音  
频率设为0则表示停止发声

- 程序的编写很简单，不过遗憾的是我们用的模拟器并没有模拟蜂鸣器发声的功能

- 到目前为止，我们的操作系统只用了16种颜色，但我们已经用上了256色的显示模式，还有240种颜色可以用，所以我们要修改一下操作系统，增加可以显示的颜色。为三原色中每种颜色赋予6个色阶，这样就可以定义出216种颜色，没定义的就只剩下24种颜色了，不过这样会产生8种重复的颜色，编写程序显示效果如下  
![](image\color.png)

- 我们还有一种方式可以让颜色变得更加丰富，使用颜色交替排列的方式，将两种颜色混合在一起，混合方式有3种，这样虽然只有6级色阶，但却可以显示出21阶色阶  
![](image\mix.png)

- 显示效果更加平滑  
![](image\color2.png)

- 迄今为止我们还不能实现同时启动两个以上的应用程序，如果修改命令行窗口使其在应用程序运行中就可以输入下一条命令的修改量太大，所以作者打算同时启动两个命令行窗口，同时删掉task_a，这样我们的操作系统更像一个真正的操作系统了
![](image\morelikerealos.png)

## 第二十六天
- 窗口移动的速度很慢，所以打算对窗口移动进行提速。窗口移动慢的原因之一就是sheet_refreshmap的速度太慢。此函数中有如下语句
```c
for (by = by0; by < by1; by++) {
    vy = sht->vy0 + by;
    for (bx = bx0; bx < bx1; bx++) {
        vx = sht->vx0 + bx;
        if (buf[by * sht->bxsize + bx] != sht->col_inv) {
            map[vy * ctl->xsize + vx] = sid;
        }
    }
}
```
其中的if语句位于三层for循环中被执行了很多次，因此如果能去掉这个if语句的话，速度应该会有不小的提高。

- 这个if语句的功能是判断图层是否为透明部分，如果强行去掉它鼠标指针就会变成一个方块，不过我们的窗口基本都是矩形的，没有透明部分，如果仅去掉窗口部分的if判断是没有影响的。所以我们将程序修改成如下的样子
```c
if (sht->col_inv == -1) {
    /*无透明色图层专用的高速版*/
    for (by = by0; by < by1; by++) {
        vy = sht->vy0 + by;
        for (bx = bx0; bx < bx1; bx++) {
            vx = sht->vx0 + bx;
            map[vy * ctl->xsize + vx] = sid;
        }
    }
} else {
    /*有透明色图层用的普通版*/
    for (by = by0; by < by1; by++) {
        vy = sht->vy0 + by;
        for (bx = bx0; bx < bx1; bx++) {
            vx = sht->vx0 + bx;
            if (buf[by * sht->bxsize + bx] != sht->col_inv) {
                map[vy * ctl->xsize + vx] = sid;
            }
        }
    }
}
```

- 在sheet_refreshmap中有这样一句
```c
map[vy * ctl->xsize + vx] = sid;
```
这个命令的功能是向内存中某个地址写入sid的值，它也位于for循环中会被反复执行，而且这个地址后面以及再后面的地址也要写入sid的值。在汇编语言中，如果我们用32位寄存器来执行MOV指令的话，仅一条指令就可以向相邻的4个地址写入值，只要指定地址是4的整数倍，指令的执行速度就和一个字节的MOV是相同的，也就是说速度可以提高4倍。因此，为了让这次的修改发挥最大的效果，我们需要使窗口在x方向上的大小为4的倍数，而且窗口的x坐标也要为4的倍数，目前所有的窗口大小都是4的倍数，所以不需要修改，而对于窗口坐标，我们需要做AND运算来取整，使打开窗口时的显示位置为4的倍数，使用鼠标拖动窗口时的目的地坐标保证是4的倍数。一次性写入4个字节的办法可以有效地提高速度，所以其它函数中的有关语句我们也可以修改，例如sheet_refreshsub。

- 窗口移动的速度变快了，但是还是跟不上鼠标的移动速度，在放开鼠标键之后窗口还在移动。这是因为伴随图层移动所进行的绘图错做非常消耗时间，导致系统来不及处理FIFO中的鼠标移动数据，所以我们等FIFO为空时再进行绘图操作。

- 添加使用Shift+F2启动命令行窗口的功能，同时我们增加可以启动多个命令行窗口的功能。

- 我们实现了开启多个命令行窗口的功能，自然而然的我们就要实现关闭命令行窗口的功能。

- 增加两个新的命令start（打开一个新的命令行窗口并运行指定的应用程序）和ncst（不打开新的命令行窗口而运行指定的应用程序）。

## 第二十七天
- 继续修补小细节，实现在启动应用程序后可以关闭命令行窗口的功能。
- 现在我们可以阻止应用程序访问操作系统用的段，但是一个应用程序访问其他应用程序的段，所以我们要使用LDT（local descriptor table）。我们通过GDTR寄存器将GDT的内存地址告知CPU，而LDT的内存地址则是通过在GDT中创建LDT段来告知CPU的。
- 由于创建应用程序时所引用的a_nask.nas变大了，在应用程序中我们没有使用的函数也被包含了进去。因此我们选择将函数拆开，从而在链接阶段，只将每个应用程序必需的.obj文件链接上去，这样可以有效减小应用程序的大小。
- 但是拆开会产生过多的.obj文件，想要解决这个问题，我们可以使用“库”，通过作者提供的库管理器，我们可以将.obj打包成一个.lib文件，这样文件的数量就减少了
- 最后就是整理Makefile使其更精简整洁