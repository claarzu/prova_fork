#include "autostrada.h"


Autostrada init_autostrada(WINDOW *autostrada)
{
    Autostrada a;
    int i, j, verso_corsia = -1, tipo_veicoli = -1, xi = 0, xf = 0;

    a.x[0] = MIN_X;
    a.x[1] = MAX_X;
    a.y[0] = STRADA_Y_MIN;
    a.y[1] = STRADA_Y_MAX;
    a.h = H_STRADA;
    a.l = L_STRADA;
    a.autostrada = autostrada;
    for (i = 0; i < N_VEICOLI; ++i){
        if (i == 0)
        {
            if (verso_corsia == -1)
                verso_corsia = rand() % 2;
        }
        else if (i == 5 || i == 10)
        {
            if (verso_corsia == 0)
                verso_corsia = 1;
            else
                verso_corsia = 0;
        }
        if (i < 5)
        {
            a.veicoli[i].y[0] = 20;
            a.veicoli[i].y[1] = 21;
            a.veicoli[i].corsia = 1;
        }
        else if (i < 10)
        {
            a.veicoli[i].y[0] = 18;
            a.veicoli[i].y[1] = 19;
            a.veicoli[i].corsia = 2;
        }
        else
        {
            a.veicoli[i].y[0] = 16;
            a.veicoli[i].y[1] = 17;
            a.veicoli[i].corsia = 3;
        }
        a.veicoli[i].h = H_RANA;
        a.veicoli[i].vr = verso_corsia;
        a.veicoli[i].num_veicolo = i + 1;
        tipo_veicoli = rand() % 2;
        if (i % 5 == 0)
        {
            xf = 0;
            xi = 0;
        }

        xi = xf + DISTANZA;
        if (tipo_veicoli == 0)
        {
            xf = xi + L_RANA;
            a.veicoli[i].l = L_RANA + 1;
            a.veicoli[i].tipo = 'a';
        }
        else
        {
            xf = xi + (L_RANA + 3);
            a.veicoli[i].l = L_RANA + 4;
            a.veicoli[i].tipo = 'c';
        }
        a.veicoli[i].x[0] = xi;
        a.veicoli[i].x[1] = xf;
        //a.veicoli[i] = sprite_veicolo(a.veicoli[i]);
    }
    return a;
}

void stampa_veicolo(WINDOW *game, Veicolo v)
{
    int i, j;
    int y_row, x_column;
    char s_auto1[H_RANA][L_RANA + 1] = {
        "|o--o\\",
        "|o--o/"};
    char s_auto2[H_RANA][L_RANA + 1] = {
        "/o--o|",
        "\\o--o|"};
    char s_camion1[H_RANA][L_RANA + 4] = {
        "|oo---o-\\",
        "|oo---o-/"};
    char s_camion2[H_RANA][L_RANA + 4] = {
        "/-o---oo|",
        "\\-o---oo|"};

    init_pair(11, COLOR_RED, COLOR_GREY);    // colore fanali
    init_pair(8, COLOR_YELLOW, COLOR_GREY);  // colore fari
    init_pair(12, COLOR_BLACK, COLOR_LRED);  // colore auto
    init_pair(10, COLOR_BLACK, COLOR_LCYAN); // colore camion

    y_row = v.y[0];
    for (i = 0; i < v.h; ++i)
    {
        x_column = v.x[0];
        for (j = 0; j < v.l; ++j)
        {
            if (v.tipo == 'a')
            {
                wattron(game, COLOR_PAIR(12));
                if(v.vr == 0){
                    mvwprintw(game, y_row, x_column, "%c", s_auto1[i][j]);
                    if (s_auto1[i][j] == '|')
                    {
                        wattron(game, COLOR_PAIR(11));
                        mvwprintw(game, y_row, x_column, "%c", '|');
                        wattroff(game, COLOR_PAIR(11));
                    }else if (s_auto1[i][j] == '\\' || s_auto1[i][j] == '/')
                    {
                        wattron(game, COLOR_PAIR(8));
                        if(s_auto1[i][j] == '\\')
                            mvwprintw(game, y_row, x_column, "%c", '\\');
                        else if(s_auto1[i][j] == '/')
                            mvwprintw(game, y_row, x_column, "%c", '/');
                        wattroff(game, COLOR_PAIR(8));
                    }
                }                 
                else{
                    mvwprintw(game, y_row, x_column, "%c", s_auto2[i][j]);
                    if (s_auto2[i][j] == '|')
                    {
                        wattron(game, COLOR_PAIR(11));
                        mvwprintw(game, y_row, x_column, "%c", '|');
                        wattroff(game, COLOR_PAIR(11));
                    }else if (s_auto2[i][j] == '\\' || s_auto2[i][j] == '/')
                    {
                        wattron(game, COLOR_PAIR(8));
                        if(s_auto2[i][j] == '\\')
                            mvwprintw(game, y_row, x_column, "%c", '\\');
                        else if(s_auto2[i][j] == '/')
                            mvwprintw(game, y_row, x_column, "%c", '/');
                        wattroff(game, COLOR_PAIR(8));
                    }
                }                    
                wattroff(game, COLOR_PAIR(12));               
            }
            else
            {
                wattron(game, COLOR_PAIR(10));
                if(v.vr == 0){
                    mvwprintw(game, y_row, x_column, "%c", s_camion1[i][j]);
                    if (s_camion1[i][j] == '|')
                    {
                        wattron(game, COLOR_PAIR(11));
                        mvwprintw(game, y_row, x_column, "%c", '|');
                        wattroff(game, COLOR_PAIR(11));
                    }else if (s_camion1[i][j] == '\\' || s_camion1[i][j] == '/')
                    {
                        wattron(game, COLOR_PAIR(8));
                        if(s_camion1[i][j] == '\\')
                            mvwprintw(game, y_row, x_column, "%c", '\\');
                        else if(s_camion1[i][j] == '/')
                            mvwprintw(game, y_row, x_column, "%c", '/');
                        wattroff(game, COLOR_PAIR(8));
                    }

                }                    
                else{
                    mvwprintw(game, y_row, x_column, "%c", s_camion2[i][j]);
                    if (s_camion2[i][j] == '|')
                    {
                        wattron(game, COLOR_PAIR(11));
                        mvwprintw(game, y_row, x_column, "%c", '|');
                        wattroff(game, COLOR_PAIR(11));
                    }else if (s_camion2[i][j] == '\\' || s_camion2[i][j] == '/')
                    {
                        wattron(game, COLOR_PAIR(8));
                        if(s_camion2[i][j] == '\\')
                            mvwprintw(game, y_row, x_column, "%c", '\\');
                        else if(s_camion2[i][j] == '/')
                            mvwprintw(game, y_row, x_column, "%c", '/');
                        wattroff(game, COLOR_PAIR(8));
                    }
                }
                    
                wattroff(game, COLOR_PAIR(10));
            }
            x_column++;
        }
        y_row++;
    }
}

void stampa_veicoli(WINDOW *game, Autostrada a)
{
    int j;
    for (j = 0; j < N_VEICOLI; ++j)
        stampa_veicolo(game, a.veicoli[j]);
}
Veicolo readVPipe(int p_in)
{
    Veicolo v;
    read(p_in, &v, sizeof(v));
    return v;
}

void writeVPipe(Veicolo v, int p_out)
{
    write(p_out, &v, sizeof(v));
}

Autostrada readSPipe(int p_in){
    Autostrada strada;
    read(p_in, &strada, sizeof(strada));
    return strada;
}

void writeSPipe(Autostrada strada, int p_out){
    write(p_out, &strada, sizeof(strada));
}

