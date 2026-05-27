#ifndef FUNZIONI_H
#define FUNZIONI_H

#include <stdio.h>
#include <limits.h>
#include <sys/inotify.h>

// Dichiarazione struct

struct Entrata_Move {
	uint32_t cookie;
	char name[NAME_MAX + 1];

};

#define MAX_MOVES 1024
extern struct Entrata_Move moves[MAX_MOVES];

struct Entrata_Watch {
	int wd;
	char dir[PATH_MAX];	

};

#define MAX_WATCHES 1024
extern struct Entrata_Watch watches[MAX_WATCHES];

extern int move_count;
extern int watch_count;

// Prototipi delle funzioni

void stampa_evento(struct inotify_event *event, const char *dir, FILE *report);
int aggiungi_watch(int fd, const char *path);
void timestamp(char *buf);

void salva_moved_from(uint32_t cookie, const char *name);
char *trova_nome_da_cookie(uint32_t cookie);
void rimuovi_cookie(uint32_t cookie);
const char *trova_dir_da_wd(int wd);

#endif

