#include <unistd.h>
int main()
{
	char string[] = "dup example\n";
	size_t size = write(1, string, 13);
	int newfd = dup(1);
	size = write(newfd, string, 13);
	close(1);
	close(newfd);
	return 0;
}