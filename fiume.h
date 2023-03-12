#ifndef VERSIONE_PROCESSI_FIUME_H
#define VERSIONE_PROCESSI_FIUME_H

#include "rana.h"


#define L_TRONCO 15
#define L_FIUME 102
#define H_FIUME 6
#define FIUME_Y_MIN 8
#define FIUME_Y_MAX 13
#define MAXX_TRONCHI 84

typedef struct
{
    int x[2];
    int y[2];
    int l; // Lunghezza nemico
    int h; // Altezza nemico
    char sprite[H_RANA][L_NEMICO];
    Proiettile p;
    _Bool attivo;
} Nemico;

typedef struct
{
    int id;
    int x[2];
    int y[2];
    int l; // Lunghezza tronco
    int h; // Altezza tronco
    char sprite[H_RANA][L_TRONCO];
    _Bool pieno;
    Nemico n;
    Verso v;
} Tronco;

typedef struct
{
    int num_flusso;
    int x[2];
    int y[2];
    int l; // Lungheza flusso
    int h; // Altezza flusso
    Verso vr;
    Tronco t;
} Flusso;

typedef struct
{
    int x[2];
    int y[2];
    int l; // Lunghezza autosatrada
    int h; // Altezza autostrada
    WINDOW *fiume;
    Flusso f[N_CORSIE_FLUSSI];
} Fiume;

Fiume init_fiume(WINDOW *fiume);
void river(int pipeout, Rana *r);
Tronco creazione_tronchi(Tronco t, int verso_flusso);
Tronco init_Tronco(int *y, Verso v, int id);
void stampa_tronco(WINDOW *game, Tronco t);
void stampa_tronchi(WINDOW *game, Fiume f);
void sposta_tronco(int pipeT[2], Tronco t);
void sposta_p(int pipeP, Proiettile P, int pid);
void manage_pn(int pipePW, int pipeTR, Tronco t);
void stampa_Pro(WINDOW *game, Proiettile p, int color_Pair);
Tronco readTPipe(int p_in);
void writeTPipe(Tronco t, int p_out);

void stampa_nemico(WINDOW *game, Nemico n);
#endif // VERSIONE_PROCESSI_FIUME_H
