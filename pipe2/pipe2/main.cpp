#include <unistd.h>
#include <stdio.h>
int main()
{
	int pipefd[2];
	pipe(pipefd);
	int proc = fork();
	if (proc == 0)
	{
		printf("Child process started\n");
		char string[5] = "bsbd";
		size_t size = write(pipefd[1], string, 5);
		printf("String %s writed to pipefd\n", string);
	}
	if (proc > 0)
	{
		printf("Parent process continued\n");
		char newstring[5];
		size_t size = read(pipefd[0], newstring, 5);
		printf("String %s readed from pipefd\n", newstring);
	}
	close(pipefd[1]);
	close(pipefd[0]);
	return 0;
}