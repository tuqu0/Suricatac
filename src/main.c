#include "../include/main.h"

int main(int argc, char **argv) {
	int socket;
	char opt;
	char *cmd;
	PCAP *list = NULL;
	PCAP *tmp;

	opt = myopt(argc, argv);

	if (opt == OPT_NONE && argc == 3) {
		if (is_readable(argv[1]) == 1 || is_directory(argv[2]) == 1)
			return 1;

		cmd = get_cmdPcap(argv[1], argv[2]);
		if (cmd == NULL)
			return 1;

		if ((socket = suricata_connect()) != -1) {
			suricata_send(VERSION, socket);
			suricata_read(socket);
			suricata_send(cmd, socket);
			suricata_read(socket);
			suricata_close(socket);
		}
		free(cmd);
	}
	else if (opt == OPT_F && argc == 3) {
		if ((list = build_list(argv[2])) == NULL)
			return 1;

		if (check_list(list) == 0) {
			tmp = list;

			if ((socket = suricata_connect()) != -1) {
				cmd = get_cmdPcap(tmp->file, tmp->dir);
				suricata_send(VERSION, socket);
				suricata_read(socket);
				suricata_send(cmd, socket);
				suricata_read(socket);
				
				while (tmp->next != NULL) {
					tmp = tmp->next;
					cmd =get_cmdPcap(tmp->file, tmp->dir);
					suricata_send(cmd, socket);
					suricata_read(socket);
				}
				suricata_close(socket);
			}
		}
		free_list(list);
	}
	else 
		usage();
	return 0;
}
