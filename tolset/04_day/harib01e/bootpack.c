void io_hlt(void);

void HariMain(void)
{
	int i;  //变量声明。变量i是32位整数
	char *p;  //变量p，用于BYTE型地址
	p = (char *) 0xa0000;

	for (i = 0; i <= 0xffff; i++) {
			p[i] = i & 0x0f;
	}

	for (;;) {
		io_hlt();
	}
}
