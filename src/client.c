#include "../include/client.h"

int suricata_connect() {
	int socketfd;
	struct sockaddr_un addr;

	socketfd = socket(AF_UNIX, SOCK_STREAM, 0);
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SOCKET_PATH);

	if (connect(socketfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		fprintf(stderr, "error %i: %s\n", errno, strerror(errno));
		return 1;
	}
	return socketfd;
}

int suricata_close(int socket) {
	if (socket != -1)
		return close(socket);
	return 1;
}

int suricata_send(char *cmd, int socket) {
	int ret;
	int cpt = 0;
	int len = strlen(cmd);
	char *buf, *offset;

	if (send(socket, cmd, len, 0) == -1) {
		fprintf(stderr, "error %i: %s\n", errno, strerror(errno));
		return 1;
	}


	len = RCV_MAX;
	if ((buf = (char *) malloc(RCV_MAX)) == NULL)
		return 1;
	offset = buf;
	memset(buf, '\0', RCV_MAX);
	while ((ret = recv(socket, offset, len, MSG_DONTWAIT))) {
		if (ret == -1) {
			if (errno == EAGAIN) {
				cpt++;
				if (cpt > TIMEOUT)
					break;
				else
					continue;
			}
			else {
				fprintf(stderr, "error %i: %s\n", errno, strerror(errno));
				return 1;
			}
		}
		else {
			if (strlen(offset) + ret < RCV_MAX - 1) {
				offset += ret;
				len = RCV_MAX - (offset - buf);
				continue;
			}
			else
				break;
		}
	}

	printf("command  : %s\n", cmd);
	if (strlen(buf) > 0)
		printf("suricata : %s\n\n", buf);
	free(buf);

	return 0;
}

char* suricata_cmd_pcaps(char *pcap, char *dir) {
	char *cmd = (char *) malloc(sizeof(CMD_PCAP) + strlen(pcap) + strlen(dir) + 1);

	if (cmd == NULL)
		return NULL;

	snprintf(cmd, sizeof(CMD_PCAP) + strlen(pcap) + strlen(dir), CMD_PCAP, pcap, dir);
	return cmd;
}


