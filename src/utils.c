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
				return ret;

			case 'v':
				if (argc > 2) {
					for (i=2; i < argc; i++)
						printf("arg%d: %s - length: %zu\n", i-1, argv[i], strlen(argv[i]));
				}
				else
					printf("usage: ./Suricatac -v [args]");
				ret = OPT_V;	
				return ret;

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
				return ret;

			case 'f':
				if (argc != 3)
					printf("usage: -f [file]\n");
				ret = OPT_F;
				return ret;
		}
	}
	return ret;
}

void usage() {
	printf("usage: ./Suricatac [option] [args]\n \
			options: -h HELP\n \
			-v [args]\n \
			-d [timeout] [args]\n \
			-f [list_pcaps]\n \
			[pcapfile] [outputdir]\n");
}

int is_readable(char *file) {
	if (access(file, R_OK) == -1) {
		fprintf(stderr, "error: %s is not readable\n", file);
		return -1;
	}
	return 0;
}

int is_directory(char *dir) {
	struct stat st;

	stat(dir, &st);
	if (!S_ISDIR(st.st_mode)) {
		fprintf(stderr, "error: %s is not a directory\n", dir);
		return -1;
	}
	return 0;
}

int is_empty(char *file) {
	struct stat st;

	stat(file, &st);
	if (st.st_size == 0)
		return 0;
	return -1;
}

char* get_realpath(char *file) {
	char real_path[PATH_MAX];
	char *path;

	realpath(file, real_path);
	if ((path = (char *) malloc(strlen(real_path) + 1)) == NULL)
		return NULL;

	strcpy(path, real_path);
	return path;
}

PCAP* build_list(char* file) {
	FILE *fd;
	size_t len = 0;
	ssize_t read;
	char *line = NULL;
	char *token1;
	char *token2;
	PCAP *list = NULL;

	if ((fd = fopen(file, "r")) != NULL && is_empty(file) != 0 && is_readable(file) == 0) {
		while ((read = getline(&line, &len, fd)) != -1) {
			token1 = strtok(line, DELIMITOR);
			token2 = strtok(NULL, DELIMITOR);

			if (token1 == NULL || token2 == NULL) {
				fclose(fd);
				fprintf(stderr, "error: unable to parse the line \"%s\"\n", line);
				return NULL;
			}

			if (token2[strlen(token2) -1] == '\n')
				token2[strlen(token2) -1] = '\0';

			list = push_list(get_realpath(token1), get_realpath(token2), list);
		}

		if (line != NULL)
			free(line);
		fclose(fd);
	}
	else
		fprintf(stderr, "error: unable to read %s\n", file);
	return list;
}

PCAP* push_list(char *file, char *dir, PCAP *list) {
	PCAP *tmp;
	PCAP *elt;

	elt = (PCAP *) malloc(sizeof(PCAP));
	elt->file = file;
	elt->dir = dir; 

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
	if (is_readable(tmp->file) == -1 || is_directory(tmp->dir) == -1)
		return -1;

	while (tmp != NULL && tmp->next != NULL) {
		tmp = tmp->next;
		if (is_readable(tmp->file) == -1 || is_directory(tmp->dir) == -1)
			return -1;
	}
	return 0;
}

void free_list(PCAP *list) {
	PCAP *tmp = NULL;
	PCAP *del = NULL;

	if (list != NULL) {
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
}
