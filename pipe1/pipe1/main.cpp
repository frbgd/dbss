#include <unistd.h>
#include <stdio.h>
int main()
{
	int pipefd[2];
	char string[] = "not from the internet(:";
	printf("Initial string: %s\n", string);
	pipe(pipefd);
	size_t size = write(pipefd[1], string, 25);
	char newstring[25];
	size = read(pipefd[0], newstring, 25);
	printf("Final string: %s\n", newstring);
	close(pipefd[0]);
	close(pipefd[1]);
	return 0;
}