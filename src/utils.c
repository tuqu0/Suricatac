#include "../include/utils.h"

int  myopt(int argc, char **argv) {
	int i;
	int ret;
	int opt;

	ret = OPT_NONE;
	while ((opt = getopt(argc, argv, "v:d:f:h")) != -1) {
		switch (opt) {
			case 'h':
				printf("Suricatac - HELP\n");
				ret = OPT_H;
				break;

			case 'v':
				if (argc > 2) {
					for (i=2; i < argc; i++)
						printf("arg%d: %s - length: %zu\n", i-1, argv[i], strlen(argv[i]));
				}
				else
					printf("usage: ./Suricatac -v [args]");
				ret = OPT_V;	
				break;

			case 'd':
				if (argc > 3) {
					for (i=3; i < argc; i++) {
						printf("arg%d: %s\n", i-2, argv[i]);
						sleep(atoi(optarg));
					}
				}
				else
					printf("usage: ./Suricatac -d [timeout] [args]\n");
				ret = OPT_D;
				break;

			case 'f':
				if (argc != 3)
					printf("usage: -f [file]\n");
				ret = OPT_F;
				break;
		}
	}
	return ret;
}

int is_readable(char *file) {
	if (access(file, R_OK) == -1) {
		fprintf(stderr, "error: %s is not readable\n", file);
		return 1;
	}
	return 0;
}

int is_directory(char *dir) {
	struct stat st;

	stat(dir, &st);
	if (!S_ISDIR(st.st_mode)) {
		fprintf(stderr, "error: %s is not a directory\n", dir);
		return 1;
	}
	return 0;
}

int is_empty(char *file) {
	struct stat st;

	stat(file, &st);
	if (st.st_size == 0)
		return 0;
	return 1;
}

PCAP* build_list(char* file) {
	FILE *fd;
	size_t len = 0;
	ssize_t read;
	char *line;
	char *pcapfile;
	char *dir;
	PCAP *list = NULL;

	if (is_readable(file) != 0)
		return NULL;
	if ((fd = fopen(file, "r")) == NULL)
		return NULL;
	if (is_empty(file) == 0)
		fprintf(stderr, "error: %s is empty\n", file);

	while ((read = getline(&line, &len, fd)) != -1) {
		pcapfile = strtok(line, DELIMITOR);
		dir = strtok(NULL, DELIMITOR);

		if (pcapfile == NULL || dir == NULL) {
			fclose(fd);
			fprintf(stderr, "error: unable to parse the line \"%s\"\n", line);
			return NULL;
		}

		if (dir[strlen(dir) -1] == '\n')
			dir[strlen(dir) -1] = '\0';
		list = push_list(pcapfile, dir, list);
	}
	fclose(fd);
	return list;
}

PCAP* push_list(char *file, char *dir, PCAP *list) {
	PCAP *tmp;
	PCAP *elt;

	elt = (PCAP *) malloc(sizeof(PCAP));
	elt->file = (char *) malloc(256);
	elt->file = (char *) malloc(strlen(file) + 1);
	elt->dir = (char *) malloc(strlen(dir) + 1);
	strcpy(elt->file, file);
	strcpy(elt->dir, dir);

	if (list == NULL)
		return elt;
	else {
		tmp = list;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = elt;
		return list;
	}
}

int check_list(PCAP *list) {
	PCAP *tmp;
	tmp = list;
	
	if (is_readable(tmp->file) == 1 || is_directory(tmp->dir) == 1)
		return 1;

	while (tmp->next != NULL) {
		tmp = tmp->next;
		if (is_readable(tmp->file) == 1 || is_directory(tmp->dir) == 1)
			return 1;
	}
	return 0;
}

void free_list(PCAP *list) {
	PCAP *tmp;
	PCAP *del;

	tmp = list;
	while(tmp->next != NULL) {
		del = tmp;
		tmp = tmp->next;

		if (del->file != NULL)
			free(del->file);
		if (del->dir != NULL)
			free(del->dir);
		free(del);
	}

	if (tmp->file != NULL)
		free(tmp->file);
	if (tmp->dir != NULL)
		free(tmp->dir);
	free(tmp);
}

