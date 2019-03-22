void io_hlt(void);

void HariMain(void)
{
	int i;  //变量声明。变量i是32位整数
	char *p;  //变量p，用于BYTE型地址

	for (i = 0xa0000; i <= 0xaffff; i++) {
			p = (char *)i;
			*p = i & 0x0f;
	}

	for (;;) {
		io_hlt();
	}
}
