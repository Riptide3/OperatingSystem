OBJS_BOOTPACK = bootpack.obj naskfunc.obj hankaku.obj graphic.obj dsctbl.obj \
		int.obj fifo.obj keyboard.obj mouse.obj memory.obj sheet.obj timer.obj \
		mtask.obj window.obj console.obj file.obj

TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# 设置默认命令为产生img

default :
	$(MAKE) img

# 正常执行过程

# 将ipl10.nas编译为ipl10.bin文件
ipl10.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl10.bin ipl10.lst

# 将asmhead.nas编译为asmhead.bin
asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

# 将字库hankaku.txt编译为hankaku.bin
hankaku.bin : hankaku.txt Makefile
	$(MAKEFONT) hankaku.txt hankaku.bin

# 将hankaku.bin编译为hankaku.obj
hankaku.obj : hankaku.bin Makefile
	$(BIN2OBJ) hankaku.bin hankaku.obj _hankaku

# 将OBJ_BOOTPACK的obj编译为bim映像文件，这里的前提条件是有OBJS_BOOTPACK，所以\
会自动执行把naskfunc.nas和其它c文件编译为obj文件
bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

# 将bootpack.bim映像编译为hrb文件
bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0

# 将hrb映像和asmhead.bin合并做成haribote.sys系统文件
haribote.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb haribote.sys

hello.hrb : hello.nas Makefile
	$(NASK) hello.nas hello.hrb hello.lst

hello2.hrb : hello2.nas Makefile
	$(NASK) hello2.nas hello2.hrb hello2.lst

a.bim : a.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:a.bim map:a.map a.obj a_nask.obj

a.hrb : a.bim Makefile
	$(BIM2HRB) a.bim a.hrb 0

hello3.bim : hello3.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello3.bim map:hello3.map hello3.obj a_nask.obj

hello3.hrb : hello3.bim Makefile
	$(BIM2HRB) hello3.bim hello3.hrb 0

hello4.bim : hello4.obj a_nask.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello4.bim stack:1k map:hello4.map \
		hello4.obj a_nask.obj

hello4.hrb : hello4.bim Makefile
	$(BIM2HRB) hello4.bim hello4.hrb 0

hello5.bim : hello5.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:hello5.bim stack:1k map:hello5.map hello5.obj

hello5.hrb : hello5.bim Makefile
	$(BIM2HRB) hello5.bim hello5.hrb 0

bug1.bim : bug1.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bug1.bim map:bug1.map bug1.obj a_nask.obj

bug1.hrb : bug1.bim Makefile
	$(BIM2HRB) bug1.bim bug1.hrb 0

bug2.bim : bug2.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bug2.bim map:bug2.map bug2.obj

bug2.hrb : bug2.bim Makefile
	$(BIM2HRB) bug2.bim bug2.hrb 0

bug3.bim : bug3.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bug3.bim map:bug3.map bug3.obj a_nask.obj

bug3.hrb : bug3.bim Makefile
	$(BIM2HRB) bug3.bim bug3.hrb 0

# 把ipl10.bin和haribote.sys结合做成操作系统文件haribote.img
haribote.img : ipl10.bin haribote.sys Makefile \
		hello.hrb hello2.hrb a.hrb hello3.hrb bug1.hrb bug2.hrb bug3.hrb \
		hello4.hrb hello5.hrb
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:haribote.sys to:@: \
		copy from:ipl10.nas to:@: \
		copy from:make.bat to:@: \
		copy from:hello.hrb to:@: \
		copy from:hello2.hrb to:@: \
		copy from:a.hrb to:@: \
		copy from:hello3.hrb to:@: \
		copy from:bug1.hrb to:@: \
		copy from:bug2.hrb to:@: \
		copy from:bug3.hrb to:@: \
		copy from:hello4.hrb to:@: \
		copy from:hello5.hrb to:@: \
		imgout:haribote.img

# 对c文件进行编译
# 把c文件编译为gas文件，需要.c bootpack.h 和 Makefile文件，然后调用cc1.exe进行处理
%.gas : %.c bootpack.h Makefile
	$(CC1) -o $*.gas $*.c

# 把gas文件转换为nas文件，其他类似上面的说明
%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

# 把nas文件转换为obj文件并输出lst文件
%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

# 根据不同的命令执行下面的情况，默认为img

img :
	$(MAKE) haribote.img

run :
	$(MAKE) img
	$(COPY) haribote.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: haribote.img

clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.obj
	-$(DEL) *.map
	-$(DEL) *.bim
	-$(DEL) *.hrb
	-$(DEL) haribote.sys

src_only :
	$(MAKE) clean
	-$(DEL) haribote.img
