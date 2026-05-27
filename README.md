Il progetto "PROJECT" che ho realizzato monitora in tempo reale quello che succede dentro una o più cartelle del sistema. 
Si usa **inotify** che è un meccanismo di Linux che permette di ricevere notifiche ogni volta che un file o una directory viene creata, eliminata, spostata o rinominata. 
In questo modo il programma tiene traccia di tutte le modifiche e le salva in un file di report.

Il codice è diviso in più file per essere più ordinato.
Nel file **funzioni.h** ci sono le strutture e le dichiarazioni delle funzioni, mentre in **funzioni.c** ci sono le funzioni vere e proprie.  Qui vengono gestiti i timestamp, l’aggiunta delle directory da monitorare e la stampa degli eventi. C’è anche tutta la parte degli spostamenti: dato che inotify divide lo spostamento in due eventi separati, il programma salva temporaneamente il nome del file e poi lo collega al nuovo nome quando arriva la seconda parte dell’evento.

Il file **main.c** è il programma principale. Qui vengono controllati gli argomenti passati dall’utente, viene aperto il file **report.txt** e viene inizializzato inotify. Dopo aver aggiunto tutte le directory da monitorare, il programma entra in un ciclo continuo in cui legge gli eventi e li gestisce uno per uno. Se viene creata una nuova cartella, questa viene aggiunta automaticamente al monitoraggio. Se un file viene spostato, il programma mostra sia il vecchio che il nuovo nome. Ogni evento viene stampato sia sul terminale che nel report.

Per compilare tutto è stato usato un **Makefile**: basta eseguire **make** e viene generato l’eseguibile **main**. Inoltre il comando **make clean** per cancellare i file compilati. Una volta compilato, il programma si avvia digitando **./main** seguito dalle cartelle che si vogliono monitorare.

