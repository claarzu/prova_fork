#ifndef VERSIONE_PROCESSI_RANA_H
#define VERSIONE_PROCESSI_RANA_H



#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define N_CORSIE_FLUSSI 3
#define N_VEICOLI 15
#define H_RANA 2
#define L_RANA 5
#define V_RANA 3
#define L_NEMICO 7
#define ESCI 113
#define MAX_Y 27
#define MAX_X 102
#define MIN_Y 0
#define MIN_X 0
#define READ 0
#define WRITE 1
#define COLOR_GREY 8
#define COLOR_BROWN 9
#define COLOR_LBROWN 10
#define COLOR_LYELLOW 11
#define COLOR_LGREY 12
#define COLOR_LRED 13
#define COLOR_LCYAN 14

#define BLOCK 0
#define UNBLOCK 1
#define UNBLOCK_R 2
#define UNBLOCK_W 3
#define UNBLOCK_RW 4

typedef enum
{
    sn_dx = 0,
    dx_sn = 1
} Verso;

typedef struct
{
    int num_proiettile;
    int x; // Posizione
    int y; // Posizione
    char sprite;
} Proiettile;

typedef struct
{
    int num_vite;
    int x; // Posizione
    int y; // Posizione
    int l; // Lunghezza rana
    int h; // Altezza rana
    char sprite[H_RANA][L_RANA];
    Proiettile p;
} Rana;


Rana init_rana(int maxy);
void gestione_rana(Rana rana, int maxx, int maxy, int pipeout, _Bool *go);
Rana *copia_rana(Rana *r);
void stampa_sprite(WINDOW *w, int row, int column, char s[row][column], int y, int x, int color_pair);
_Bool countdown(unsigned int sec);
void wait_p(WINDOW *w, int seconds);
Proiettile cancella_Pro(Proiettile p, int num_proiettile);
void writePPipe(Proiettile p, int p_out);
Proiettile readPPipe(int p_in);
int createPipe(int fd[2], int mode);

#endif // VERSIONE_PROCESSI_RANA_H
