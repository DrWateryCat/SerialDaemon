#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char** argv)
{
	FILE *fp = NULL;
	pid_t process_id = 0;
	pid_t sid = 0;

	process_id = fork();
	if(process_id < 0){
		printf("Fork Failure!");
		exit(1);
	}
	if(process_id > 0){
		printf("PID of child process %d\n", process_id);
		exit(0);
	}

	umask(0);

	sid = setsid();
	if(sid < 0){
		exit(1);
	}

	chdir("/");

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	fp = fopen("log.txt", "w+");

	FILE *uart = fopen("/dev/ttyAMA0", "rw+");
	void* buffer = NULL;
	while(1){
		sleep(1);
		fprintf(fp, "Logging... \n");
		fflush(fp);

		ssize_t input = read(uart, buffer, 4);

		if(buffer == "init"){
			system("python /root/PySerialProject/src/__init__.py");
			break;
		}
	}
	fclose(fp);
	fclose(uart);
	exit(0);
}
