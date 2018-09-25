#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "config.h"

int
main(int argc, char **argv)
{
	char buf[MSGLEN + 1];
	struct sockaddr_in local, remote;
	int s, remotelen, cfd0;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	local.sin_family = AF_INET;
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(PORT);

	if (bind(s, (struct sockaddr *)&local, sizeof(local)) == -1) {
		perror("bind");
		exit(1);
	}

	//TODO: Completar conexión del socket para TCP
	if (listen(s, 10) == -1){
		perror("listen");
		exit(1);
	}

	remotelen = sizeof(remote);
	if ((cfd0 = accept(s, (struct sockaddr *)&remote, (socklen_t *)&remotelen))
	    == -1) {
		perror("accept cfd0");
		exit(1);
	}

	for (;;) {
		memset(&buf, 0, MSGLEN+1);

		//TODO: Recibir mensaje en buf
		recv(cfd0, buf, sizeof(buf) - 1, 0);

		//Imprimir comando
		printf("comando: %s", buf);

		if (strncmp(buf, ENDMSG, sizeof(ENDMSG)) == 0)
			break;
		//Ejectuar comando
		if(system(buf) == -1){
			perror("error ejecución comando");
			break;
		}
	}

	close(s);
	return 0;
}
