; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; 制作目标文件的模式
[INSTRSET "i486p"]				; 使用486的指令
[BITS 32]						; 制作32位模式用的机械语言
[FILE "naskfunc.nas"]			; 源文件名信息

		GLOBAL	_io_hlt,_write_mem8

[SECTION .text]

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_write_mem8:	; void write_mem8(int addr, int data);
		MOV		ECX,[ESP+4]		; [ESP+4]中存放的是地址，将其读入ECX
		MOV		AL,[ESP+8]		; [ESP+8]中存放的是数据，将其读入AL
		MOV		[ECX],AL
		RET
