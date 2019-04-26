#include <unistd.h>
#include <stdio.h>
int main()
{
	printf("I was born by C printf\n");
	execl("/bin/echo", "/bin/echo", "I was born by exec echo", NULL);
	return 0;
}