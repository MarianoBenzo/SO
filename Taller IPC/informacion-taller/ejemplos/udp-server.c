#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKPATH "socket-mensajes"
#define CMDEND "chau\n"


int
main(int argc, char **argv)
{
	char buf[1024];
	struct sockaddr_un local;
	ssize_t r;
	int sfd;

	/* crea socket */
	if ((sfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	local.sun_family = AF_UNIX;
	strncpy(local.sun_path, SOCKPATH, sizeof(local.sun_path) - 1);

	//remover el nombre del fs si lo estaban usando
	unlink(SOCKPATH);

	/* linkea socket con dirección */
	if (bind(sfd, (struct sockaddr *)&local, sizeof(local)) == -1) {
		perror("bind");
		exit(1);
	}

	/* Escucho paquetes de clientes */
	for (;;) {
		r = read(sfd, buf, sizeof(buf) - 1);
		if (r == -1) {
			perror("read");
			continue;
		}
		/* transforma en string de C */
		buf[r] = '\0';
		if (strncmp(buf, CMDEND, sizeof(CMDEND)) == 0)
			break;
		printf("< %s", buf);
	}

	/* cierra sockets */
	close(sfd);

	return 0;
}
