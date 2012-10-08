#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>

#define OPT_F 1
#define OPT_V 2
#define OPT_H 3
#define OPT_D 4
#define OPT_NONE 5
#define DELIMITOR ";"

/* Element of a list with a .pcap filename and an output directory */
typedef struct PCAP_ {
	char *file;
	char *dir;
	struct PCAP_ *next;
} PCAP;

/* Options parser - Returns the selected option */
int myopt (int argc, char **argv); 

/* Explain how to use Suricatac */
void usage();

/* Check if the file is readable */
int is_readable(char *file);

/* Check if the file is a directory */
int is_directory(char *dir);

/* Check if the file is empty */
int is_empty(char *file);

/* Parse a file in tokens separated by a delimitor and build a list of PCAP elements - Returns the adress list */
PCAP* build_list(char* file);

/* Check for each element of the list if the file is readable and if the directory name is a directory */
int check_list(PCAP *list);

/* Create a new PCAP element and push it into the list - Returns the adress list */
PCAP* push_list(char *file, char *dir, PCAP *list);

/* Free each element of the list */
void free_list(PCAP *list);

#endif /* !UTILS_H */
