#include "funzioni.h"
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

struct Entrata_Move moves[MAX_MOVES];
struct Entrata_Watch watches[MAX_WATCHES];
int move_count = 0;
int watch_count = 0;

// Corpo delle funzioni

void timestamp(char *buf) {
	time_t now = time(NULL);
    	struct tm *t = localtime(&now);
    	strftime(buf, 32, "%Y-%m-%d %H:%M:%S", t); // Formatta la data e l'ora in una stringa

}

int aggiungi_watch(int fd, const char *path) {

	char abs_path[PATH_MAX];
    	if (!realpath(path, abs_path)) {
        	strncpy(abs_path, path, PATH_MAX);

    	}

    	int wd = inotify_add_watch(fd, abs_path,
        				IN_CREATE | IN_DELETE |
                               		IN_MOVED_FROM | IN_MOVED_TO |
                               		IN_ISDIR);

    	if (wd == -1) {
        	printf("Errore in inotify_add_watch su %s\n", abs_path);

    	} 

	else {
        	printf("Monitoraggio attivo su %s (wd=%d)\n", abs_path, wd);

        	if (watch_count < MAX_WATCHES) {
            		watches[watch_count].wd = wd;
            		strncpy(watches[watch_count].dir, abs_path, PATH_MAX);
            		watch_count++;
        
		}
    	}	    	

    	return wd;

}

void stampa_evento(struct inotify_event *event, const char *dir, FILE *report) {

	char ts[32];
    	timestamp(ts);

    	// Ricostruzione del percorso completo del file/directory
    	
	char fullpath[PATH_MAX * 2];
    	if (event->len > 0){
		snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, event->name);
	
	}
	else {
		snprintf(fullpath, sizeof(fullpath), "%s", dir);

	}

    	char eventi[256] = "";

    	// Gli eventi possono essere combinati (es. IN_CREATE + IN_ISDIR)
    
	if (event->mask & IN_CREATE) strcat(eventi, "IN_CREATE ");
    	if (event->mask & IN_DELETE) strcat(eventi, "IN_DELETE ");
    	if (event->mask & IN_MOVED_FROM) strcat(eventi, "IN_MOVED_FROM ");
    	if (event->mask & IN_MOVED_TO) strcat(eventi, "IN_MOVED_TO ");
    	if (event->mask & IN_ISDIR) strcat(eventi, "IN_ISDIR ");

    	printf("[%s] %s (%u) - %s\n", ts, eventi, event->mask, fullpath);
    	fprintf(report, "[%s] %s (%u) - %s\n", ts, eventi, event->mask, fullpath); // Scrive in un file
    	fflush(report); // Forza la scrittura immediata nel file 

}	

const char *trova_dir_da_wd(int wd) {
	
	int i = 0;
	for (i = 0; i < watch_count; i++) {
        	if (watches[i].wd == wd)
            	return watches[i].dir;

    	}
    	return NULL;

}

void salva_moved_from(uint32_t cookie, const char *name) {
	
	if (move_count >= MAX_MOVES) return;
    	moves[move_count].cookie = cookie;
    	strncpy(moves[move_count].name, name, NAME_MAX);
    	moves[move_count].name[NAME_MAX] = '\0';
    	move_count++;

}


char *trova_nome_da_cookie(uint32_t cookie) {
	
	int i = 0;
	for (i = 0; i < move_count; i++) {
        	if (moves[i].cookie == cookie)
            	return moves[i].name;
    
	}
	return NULL;

}

void rimuovi_cookie(uint32_t cookie) {
	
	int i = 0;
	for (i = 0; i < move_count; i++) {
        	if (moves[i].cookie == cookie) {
            	moves[i] = moves[move_count - 1];
            	move_count--;
            	return;

        }
    }
}


