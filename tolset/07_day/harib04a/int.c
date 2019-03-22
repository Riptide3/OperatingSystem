/* 有关中断 */

#include "bootpack.h"

void init_pic(void)
/* PIC的初始化 */
{
  io_out8(PIC0_IMR,  0xff  ); // 禁止所有中断
  io_out8(PIC1_IMR,  0xff  ); // 禁止所有中断

  io_out8(PIC0_ICW1, 0x11  ); // 边沿触发模式（edge trigger mode）
  io_out8(PIC0_ICW2, 0x20  ); // TRQ0-7由INT20-27接受
  io_out8(PIC0_ICW3, 1 << 2); // PIC1由IRQ2连接
  io_out8(PIC0_ICW4, 0x01  ); // 无缓冲区模式

  io_out8(PIC1_ICW1, 0x11  ); // 边沿触发模式（edge trigger mode）
  io_out8(PIC1_ICW2, 0x28  ); // IRQ8-15由INT28-2f接收
  io_out8(PIC1_ICW3, 2     ); // PIC1由IRQ2连接
  io_out8(PIC1_ICW4, 0x01  ); // 无缓冲区模式

  io_out8(PIC0_IMR,  0xfb  ); // 11111011 PIC1以外全部禁止
  io_out8(PIC1_IMR,  0xff  ); // 11111111 禁止所有中断

  return;
}


#define PORT_KEYDAT     0x0060

void inthandler21(int *esp)
/* 来自PS/2键盘的中断 */
{
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  unsigned char data, s[4];
  io_out8(PIC0_OCW2, 0x61); // 通知PIC"IRQ-01"已经受理完毕
  data = io_in8(PORT_KEYDAT);

  sprintf(s, "%02X", data);
  boxfill8(binfo->vram, binfo->scrnx, COL8_008484, 0, 16, 15, 31);
  putfonts8_asc(binfo->vram, binfo->scrnx, 0, 16, COL8_FFFFFF, s);

  return;
}

void inthandler2c(int *esp)
/* 来自PS/2鼠标的中断 */
{
  struct BOOTINFO *binfo = (struct BOOTINFO *) ADR_BOOTINFO;
  boxfill8(binfo->vram, binfo->scrnx, COL8_000000, 0, 0, 32 * 8 - 1, 15);
  putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, COL8_FFFFFF, "INT 2C (IRQ-12) : PS/2 mouse");
  for (;;) {
    io_hlt();
  }
}

void inthandler27(int *esp)

/* 针对来自PIC0的不完全中断的措施
    在Athlon64X2等机器中，由于芯片组的便利性，该中断仅在PIC初始化时发生一次。
    该中断处理功能在不中断中断的情况下传递
    为什么这个函数什么都不做？
    由于此中断是由PIC初始化时的电噪声引起的，因此无需认真处理。*/
{
  io_out8(PIC0_OCW2, 0x67); // IRQ-07通知PIC验收（见7-1）
  return;
}
