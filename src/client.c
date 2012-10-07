#include "../include/client.h"

int suricata_connect() {
	int socketfd;
	struct sockaddr_un addr;

	socketfd = socket(AF_UNIX, SOCK_STREAM, 0);
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SOCKET_PATH);

	if (connect(socketfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		fprintf(stderr, "connection socket error %i: %s\n", errno, strerror(errno));
	return socketfd;
}

int suricata_close(int socket) {
	if (socket != -1)
		return close(socket);
	return 1;
}

int suricata_send(char *cmd, int socket) {
	int len;

	len = strlen(cmd);
	if (send(socket, cmd, len, 0) == -1) {
		fprintf(stderr, "send socket error %i: %s\n", errno, strerror(errno));
		return 1;
	}
	return 0;
}

int suricata_read(int socket) {
	int ret;
	int cpt;
	int len;
	char *buf;
	char *offset;

	len = RCV_MAX;
	cpt = 0;
	buf = (char *) malloc(RCV_MAX);

	if (buf == NULL)
		return 1;

	offset = buf;
	memset(buf, '\0', RCV_MAX);
	while ((ret = recv(socket, offset, len, MSG_DONTWAIT))) {
		if (ret == -1) {
			if (errno == EAGAIN) {
				cpt++;
				if (cpt > 10000)
					break;
				else
					continue;
			}
			else {
				fprintf(stderr, "read socket error %i: %s\n", errno, strerror(errno));
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

	if (sizeof(buf) > 0)
		printf("suricata: %s\n", buf);
	free(buf);
	return 0;
}

char* get_cmdPcap(char *pcap, char *dir) {
	char *cmd;

	cmd = (char *) malloc(sizeof(CMD_PCAP) + strlen(pcap) + strlen(dir) + 1);
	if (cmd == NULL)
		return NULL;

	snprintf(cmd, sizeof(CMD_PCAP) + strlen(pcap) + strlen(dir), CMD_PCAP, pcap, dir);
	return cmd;
}


