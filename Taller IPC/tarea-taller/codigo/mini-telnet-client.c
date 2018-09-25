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
	char *buf = NULL;
	struct sockaddr_in name;
	ssize_t w, len, r;
	int s;
	len = 0;

	if (argc != 2) {
		fprintf(stderr, "uso: %s ip\n", argv[0]);
		exit(1);
	}

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	if (!inet_aton(argv[1], &name.sin_addr)) {
		fprintf(stderr, "%s: ip desconocida\n", argv[1]);
		exit(1);
	}
	name.sin_family = AF_INET;
	name.sin_port = htons(PORT);
	if (connect(s, (struct sockaddr *)&name, sizeof(name)) == -1) {
		perror("connect");
		exit(1);
	}

	for (;;) {
		printf("[%s]> ", argv[1]);

		if ((w = getline(&buf, &len, stdin)) == -1) {
			if (!feof(stdin))
				perror("getline");
			break;
		}
		if (strncmp(buf, ENDMSG, w) == 0)
			break;

		if (send(s, buf, w, 0) == -1) {
			perror("send");
			exit(1);
		}
	}
	free(buf);
	close(s);

	return 0;
}
