#include "../include/main.h"

int main(int argc, char **argv) {
	int socket;
	char opt;
	char *cmd, *file, *dir;
	PCAP *list = NULL;
	PCAP *tmp;

	opt = myopt(argc, argv);
	
	if (opt == OPT_H || opt == OPT_V || opt == OPT_D)
		return 0;
	else if (opt == OPT_NONE && argc == 3) {
		if (is_readable(argv[1]) == -1 || is_directory(argv[2]) == -1)
			return -1;

		if ((socket = suricata_connect()) != 1) {
			suricata_send(VERSION, socket);
			file = get_realpath(argv[1]);
			dir = get_realpath(argv[2]);
			cmd = suricata_cmd_pcaps(file, dir);
			suricata_send(cmd, socket);
			suricata_close(socket);
			free(file);
			free(dir);
			free(cmd);
		}
	}
	else if ((opt == OPT_F && argc == 3)) {
		if ((list = build_list(argv[2])) == NULL)
			return -1;

		if (check_list(list) == 0) {
			tmp = list;

			if ((socket = suricata_connect()) != 1) {
				//suricata_send(VERSION, socket);
				//
				cmd = json_cmd("version", "0.1");
				suricata_send(cmd, socket);
				cmd = suricata_cmd_pcaps(tmp->file, tmp->dir);
				suricata_send(cmd, socket);
				free(cmd);

				while (tmp->next != NULL) {
					tmp = tmp->next;
					cmd = suricata_cmd_pcaps(tmp->file, tmp->dir);
					suricata_send(cmd, socket);
					free(cmd);
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
