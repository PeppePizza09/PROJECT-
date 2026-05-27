#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include "funzioni.h"

// Dichiarazione del buff

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

// Programma principale

int main(int argc, char *argv[]){

	int i = 0;
	int fd;
    	char buf[BUF_LEN];
    	ssize_t numRead;
    	char *p;

    	if (argc < 2) {
        	printf("Monitoraggio: %s dir1 dir2 ...\n", argv[0]);
        	return 0;

    	}

    	FILE *report = fopen("report.txt", "w");
    	if (!report) {
        	printf("Errore apertura report.txt\n");
        	return 0;

    	}

    	fd = inotify_init();
    	if (fd == -1) {
        	printf("Errore in inotify_init\n");
		fclose(report);
        	return 0;

    	}	

    	// Aggiunta dei watch iniziali

    	for (i = 1; i < argc; i++) {
        	aggiungi_watch(fd, argv[i]);
    	
	}

    	// Ciclo infinito di lettura eventi

    	for (;;) {

		numRead = read(fd, buf, BUF_LEN);

        	if (numRead <= 0) {
            		printf("Errore in read\n");
            		return 0;
	
	        }

        	for (p = buf; p < buf + numRead; ) {

            		struct inotify_event *event = (struct inotify_event *) p;
			
			const char *dir = trova_dir_da_wd(event->wd);
			if (!dir){
				p += sizeof(struct inotify_event) + event->len;
				continue;

			}
	
			// Salvataggio IN_MOVED_FROM (cookie + nome)
			if (event->mask & IN_MOVED_FROM){
				salva_moved_from(event->cookie, event->name);

			}

			// Quando arriva IN_MOVED_TO si cerca la corrispondenza tra i due cookie
			
			if (event->mask & IN_MOVED_TO){
				char *oldname = trova_nome_da_cookie(event->cookie);
				
				if (oldname){
					printf("\nIl vecchio nome e\': %s\n", oldname);
					printf("\nIl nuovo nome e\': %s\n", event->name);
					fprintf(report, "Il vecchio nome e\': %s\n", oldname);
					fprintf(report, "Il nuovo nome e\': %s\n", event->name);
					fflush(report);

					rimuovi_cookie(event->cookie);

				}
			}

            		// Nuova directory + aggiungi watch

			if ((event->mask & IN_CREATE) && (event->mask & IN_ISDIR)) {
				char newdir[PATH_MAX * 2];
	    		        snprintf(newdir, sizeof(newdir), "%s/%s", dir, event->name);
				usleep(50000);
				aggiungi_watch(fd, newdir);
			
			}

			// Stampa eventi

			stampa_evento(event, dir, report);	
				
            		p += sizeof(struct inotify_event) + event->len;
        	
			}
    	}

    	fclose(report); // Chiude il file report aperto e salva tutto in quel file
	close(fd);
    	return 0;

}


