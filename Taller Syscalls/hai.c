#include <signal.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char* argv[]) {
  	int status;
	pid_t child;
	int repeticiones = 0;

	if (argc <= 1) {
		fprintf(stderr, "Uso: %s commando [argumentos ...]\n", argv[0]);
		exit(1);
	}

	/* Fork en dos procesos */
	child = fork();
	if (child == -1) { perror("ERROR fork"); return 1; }
	if (child == 0) {
		if (ptrace(PTRACE_TRACEME, 0, NULL, NULL)) {
			perror("ERROR child ptrace(PTRACE_TRACEME, ...)"); exit(1);
		}
		if(repeticiones == 5){
			/* S'olo se ejecuta en el Hijo */
			execvp(argv[1], argv+1);
			/* Si vuelve de exec() hubo un error */
			perror("ERROR child exec(...)"); exit(1);
		}else{
			kill(getpid(), SIGURG);
		}
		
	} else {
		/* S'olo se ejecuta en el Padre */
		if(ptrace(PTRACE_ATTACH, child, NULL, NULL)) {
				perror("ERROR child ptrace(PTRACE_TRACEME, ...)"); exit(1);
		}
		while(1){
			if (wait(&status) < 0) { perror("waitpid"); break; }
			repeticiones++;
			if (repeticiones < 6)
			{
				ptrace(PTRACE_SYSCALL, child, NULL, NULL);
				printf("sup!\n");
				printf("ya va!\n");
			}else{
				ptrace(PTRACE_KILL, child, NULL, NULL);	
			}

		}
		ptrace(PTRACE_DETACH, child, NULL, NULL); /* Liberamos al hijo */
	}
	return 0;
}
