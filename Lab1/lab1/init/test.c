#include "test.h"

void putChar(const char* msg)
{
	while (*msg != '\0')
	{
		*UART16550A_DR = (unsigned char)(*msg);
		msg++;
	}
}

int os_test()
{
	const char *msg = "Hello RISC-V!\n";

    putChar(msg);

    return 0;
}
