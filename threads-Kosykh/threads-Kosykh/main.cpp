#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdexcept>
// ������� ���������� ������������ ������ ������ ������� � ������
std::string exec(const char* cmd) {
	char buffer[128];
	std::string result = "";
	FILE* pipe = popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof buffer, pipe) != NULL) {
				result += buffer;
		}
	}
	catch (...) {
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return result;
}
int main(int argc, char* argv[]) {
	// �������� �� ���������� ����������
	if (argc != 4) {
		std::cout << "format: <command> <stdout_file or terminal> <stderr_file or terminal > " << std::endl;
			return -666;
	}
	// ������ ��������� � ����������
	std::string argv2 = argv[2];
	std::string argv3 = argv[3];
	std::string exitCode;
	const char* PROGRAM_NAME = argv[1];
	char* args[] = { NULL };
	// ������������� ��������� pipe
	int pipeOut[2], pipeErr[2];
	pipe(pipeOut);
	pipe(pipeErr);
	// ���������� �������� �������
	std::string strOut, strErr;
	char buf[32] = { 0 }; // �������� ������
	ssize_t bytesRead;
	pid_t childPid;
	childPid = fork(); // �������� ��������� ��������
	if (childPid == -1) {
		perror("fork");
		exit(-1);
	}
	else if (childPid == 0) {
		close(pipeOut[0]); //
		if (dup2(pipeOut[1], 1) < 0) { // �������� ������� �������� stdout
			perror("dup2.1");
			exit(-1);
		}
		close(pipeErr[0]);
		if (dup2(pipeErr[1], 2) < 0) { // �������� ������� �������� stderr
			perror("dup2.2");
			exit(-1);
		}
		// ������ �������
		if (execv(PROGRAM_NAME, args) == -1) {
			perror("execv");
			exit(-1);
		}
		exit(0);
	}
		wait(NULL);
	exitCode = exec("echo $?"); // ���������� ���� ����������
	// ��������� ������ ������������ ��� ������� �����������
	fcntl(pipeOut[0], F_SETFL, O_NONBLOCK | O_ASYNC);
	while (1) {
		bytesRead = read(pipeOut[0], buf, sizeof(buf) - 1);
		if (bytesRead <= 0)
			break;
		buf[bytesRead] = 0; //���������� /0
		strOut += buf; // ������ � ������ stdout
	}
	// ���� ����� � �������, �� ������� �����
	// ����� ����� � ���� � ����� ����������
	if (argv2 == "terminal") {
		std::cout << strOut << std::endl;
	}
	else {
		std::ofstream soFILE;
		soFILE.open(argv[2]);
		strOut += exitCode;
		soFILE << strOut << std::endl;
		soFILE.close();
	}
	//���������� ��� stdeer
	fcntl(pipeErr[0], F_SETFL, O_NONBLOCK | O_ASYNC);
	while (1) {
		bytesRead = read(pipeErr[0], buf, sizeof(buf) - 1);
		if (bytesRead <= 0)
			break;
		buf[bytesRead] = 0; // append null terminator
		strErr += buf; // append what wuz read to the string
	}
	if (argv3 == "terminal") {
		std::cout << strErr << std::endl;
	}
	else {
		std::ofstream seFILE;
		seFILE.open(argv[3]);
		strErr += exitCode;
		seFILE << strErr << std::endl;
		seFILE.close();
	}
	std::cout << exitCode << std::endl;
	close(pipeOut[0]);
	close(pipeErr[0]);
	return 0;
}