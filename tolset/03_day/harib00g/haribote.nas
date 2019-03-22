; haribote-os
; TAB=4

		ORG 	0xc200				; 程序装载的位置

		MOV 	AL,0x13				; VGA显卡，320x300x8位彩色
		MOV 	AH,0x00
		INT 	0x10

fin:
		HLT
		JMP		fin
