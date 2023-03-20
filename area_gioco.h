#ifndef VERSIONE_PROCESSI_AREA_GIOCO_H
#define VERSIONE_PROCESSI_AREA_GIOCO_H

#include "autostrada.h"
#include "fiume.h"

#define NUM_TANE 5

#define TANA_Y_MIN 5
#define TANA_Y_MAX 7
#define MARCIAPIEDE_Y_MIN 22
#define MARCIAPIEDE_Y_MAX 23
#define PRATO_Y_MIN 14
#define PRATO_Y_MAX 15

typedef struct
{
    int x[2];
    int y[2];
    int l; // Lunghezza prato
    int h; // Altezza prato
    WINDOW *prato;
} Prato;

typedef struct
{
    int x[2];
    int y[2];
    int l; // Lunghezza marciapiede
    int h; // Altezza marciapiede
    WINDOW *marciapiede;
} Marciapiede;

typedef struct
{
    int x[2];
    int y[2];
    int l; // Lunghezza tana
    int h; // Altezza tana
    char sprite[H_RANA][L_RANA];
    _Bool pieno;
} Tana;

typedef struct
{
    int x[2];
    int y[2];
    int l; // Lunghezza tane
    int h; // Altezza tane
    Tana t[NUM_TANE];
    WINDOW *tane;
} Tane;

typedef struct
{
    Autostrada a;
    Fiume f;
    Marciapiede m;
    Prato p;
    Tane t;
} Area;



Area init_area(WINDOW *marciapiede, WINDOW *autostrada, WINDOW *prato, WINDOW *fiume, WINDOW *tane);
Tana init_sprite_tana(int xi, int xf);
Tane init_tane(WINDOW *tane);
Marciapiede init_marciapiede(WINDOW *marciapiede);
Prato init_prato(WINDOW *prato);
void stampa_tane(WINDOW *w, Tane t);
void gestione_area(WINDOW *game, WINDOW *fiume, WINDOW *autostrada, Area area, int pipeAreaW, int pipeT[2]);
void writeAreaPipe(Area area, int p_out);
Area readAreaPipe(int p_in);
void aggiornaArea(WINDOW *game, WINDOW *fiume, WINDOW *autostrada, Area area, int pipeVR, int pipeVAW);
void aggiorna_fiume(WINDOW *game, WINDOW *fiume, Area a, int pipeTR[2]);
void aggiorna_autostrada(WINDOW *game, WINDOW *strada, Area a, int pipeVAW);
void gestione_fiume(int pipeAF[2], Area area);
void gestione_strada(int pipeVAuxR, int pipeVW, Veicolo v);
Veicolo sposta_veicolo(int pipeVAuxR, int pipeVW, Veicolo v);
_Bool check_id(int dim, int array[dim], int id);
_Bool check_coordinata(int corsia, Area strada, Veicolo v);
Veicolo cambio_corsia(int pipeVAuxW, Veicolo v, Area a, int corsia);
_Bool check_corsia(Veicolo v);

#endif // VERSIONE_PROCESSI_AREA_GIOCO_H
