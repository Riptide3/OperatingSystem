; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; 制作目标文件的模式
[INSTRSET "i486p"]				; 使用486的指令
[BITS 32]						; 制作32位模式用的机械语言
[FILE "naskfunc.nas"]			; 源文件名信息

		GLOBAL	_io_hlt

[SECTION .text]

_io_hlt:	; void io_hlt(void);
		HLT
		RET
