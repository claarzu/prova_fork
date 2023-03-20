#include "area_gioco.h"

Marciapiede init_marciapiede(WINDOW *marciapiede)
{
    Marciapiede m;
    m.l = MAX_X;
    m.h = H_RANA;
    m.marciapiede = marciapiede;
    m.x[0] = 0;
    m.x[1] = MAX_X - 1;
    m.y[0] = MARCIAPIEDE_Y_MIN;
    m.y[1] = MARCIAPIEDE_Y_MAX;
    return m;
}

Prato init_prato(WINDOW *prato)
{
    Prato p;
    p.l = MAX_X;
    p.h = H_RANA;
    p.prato = prato;
    p.x[0] = 0;
    p.x[1] = MAX_X - 1;
    p.y[0] = PRATO_Y_MIN;
    p.y[1] = PRATO_Y_MAX;
    return p;
}

Tana init_sprite_tana(int xi, int xf)
{
    Tana t;
    int i, j;
    char sprite[H_RANA][L_RANA] = {
        "|\"\"\"|",
        "|   |"};

    t.x[0] = xi;
    t.x[1] = xf;
    t.y[0] = TANA_Y_MIN + 1;
    t.y[1] = TANA_Y_MAX;
    for (i = 0; i < H_RANA; ++i)
    {
        for (j = 0; j < L_RANA; ++j)
            t.sprite[i][j] = sprite[i][j];
    }
    return t;
}

Tane init_tane(WINDOW *tane)
{
    Tane tn;
    int i, j, h = 3;
    tn.l = MAX_X;
    tn.h = h;
    tn.tane = tane;

    for (i = 0; i < NUM_TANE; ++i)
    {
        tn.t[i].h = H_RANA;
        tn.t[i].l = L_RANA;
        tn.t[i].pieno = false;

        if (i == 0)
            tn.t[i] = init_sprite_tana(2, 6);
        else if (i == 1)
            tn.t[i] = init_sprite_tana(25, 29);
        else if (i == 2)
            tn.t[i] = init_sprite_tana(48, 52);
        else if (i == 3)
            tn.t[i] = init_sprite_tana(71, 75);
        else
            tn.t[i] = init_sprite_tana(95, 99);
    }
    return tn;
}

Area init_area(WINDOW *marciapiede, WINDOW *autostrada, WINDOW *prato, WINDOW *fiume, WINDOW *tane)
{
    Area area;
    Tane t;
    Autostrada a;
    Fiume f;
    Prato p;
    Marciapiede m;
    int i;

    a = init_autostrada(autostrada);
    f = init_fiume(autostrada);
    m = init_marciapiede(marciapiede);
    p = init_prato(prato);
    t = init_tane(tane);

    area.t = t;
    area.m = m;
    area.p = p;
    area.f = f;
    area.a = a;

    return area;
}

void stampa_tane(WINDOW *tane, Tane t)
{
    int i, j;
    init_pair(13, COLOR_LBROWN, COLOR_WHITE);
    for (i = 0; i < NUM_TANE; ++i)
    {
        stampa_sprite(tane, H_RANA, L_RANA, t.t[i].sprite, t.t[i].y[0], t.t[i].x[0], 13);
    }
}
void gestione_area(WINDOW *game, WINDOW *fiume, WINDOW *autostrada, Area area, int pipeAreaW, int pipeT[2])
{
    Tronco t_aux;
    Veicolo vl;
    int i, j;
    Area t_area = area;

    for (i = 0; i < N_CORSIE_FLUSSI; i++)
    {
        t_aux = readTPipe(pipeT[READ]);
        if (t_area.f.f[i].t.y[0] == t_aux.y[0])
            t_area.f.f[i].t = t_aux;
    }

    writeAreaPipe(t_area, pipeAreaW);
    //_exit(EXIT_SUCCESS);
}

_Bool check_id(int dim, int array[dim], int id)
{
    int i;
    for (i = 0; i < dim; i++)
    {
        if (array[i] == id)
            return true;
    }

    return false;
}

void aggiornaArea(WINDOW *game, WINDOW *fiume, WINDOW *autostrada, Area area, int pipeVR, int pipeVAW)
{
    Area t_Area = area;
    Autostrada strada;
    int i, j, cont = 0, cont_v[N_VEICOLI], corsiaM[2][3], corsia = 0, time_read, time_write;
    _Bool check = false;
    Tronco t_aux;
    Veicolo v, v_aux;
    time_read = 100002;
    v.num_veicolo = 0;
    while (1)
    {
        cont = 0;
        check = false;
        for (i = 0; i < N_VEICOLI; i++)
            cont_v[i] = 0;
        for (i = 0; i < N_CORSIE_FLUSSI; i++)
        {
            corsiaM[0][i] = 0;
            corsiaM[1][i] = 0;
        }

        //usleep(1515350);
        while (check == false) // ciclo per ottenere tutti i veicoli da visualizzare
        {
            for (j = 0; j < N_VEICOLI; j++)
            {
                /* if (t_Area.a.veicoli[j].num_veicolo == 1){
                    time_write = 60000;
                    usleep(time_write);
                } else{
                    time_write = time_read + 1;
                    usleep(time_write);
                }
                writeVPipe(t_Area.a.veicoli[j], pipeVAW);  */

                if (v.num_veicolo == 0){
                    time_read = 66666;
                    usleep(time_read);
                } else{
                    time_read = time_read + 66666;
                    usleep(time_read);
                }
                           
                v = readVPipe(pipeVR);
                if (!check_id(N_VEICOLI, cont_v, v.num_veicolo))
                {
                    for (j = 0; j < N_VEICOLI; j++)
                    {
                        if (v.num_veicolo == t_Area.a.veicoli[j].num_veicolo)
                        {
                            cont_v[j] = v.num_veicolo;
                            t_Area.a.veicoli[j] = v;
                            cont++;
                        }
                    }
                    if (cont == 15)
                        check = true;
                }
            }
            
            
        }

        cont = 0;
        // se tutti i veicoli sono stati caricati
        if (check)
        {
            for (i = 0; i < N_VEICOLI; i++)
            {
                if (check_corsia(t_Area.a.veicoli[i]))
                {
                    cont++;
                    if (corsiaM[0][0] == 0)
                        corsiaM[0][0] = t_Area.a.veicoli[i].num_veicolo;
                    else if (corsiaM[0][1] == 0)
                        corsiaM[0][1] = t_Area.a.veicoli[i].num_veicolo;
                    else
                        corsiaM[0][2] = t_Area.a.veicoli[i].num_veicolo;
                }
            }

            if (cont > 0)
            {
                for (i = 0; i < N_CORSIE_FLUSSI; i++)
                {
                    do
                    {
                        corsia = 1 + rand() % 3;
                    } while (corsiaM[1][0] == corsia || corsiaM[1][1] == corsia || corsiaM[1][2] == corsia);

                    corsiaM[1][i] = corsia;
                }

                for (i = 0; i < N_CORSIE_FLUSSI; i++)
                {
                    attron(COLOR_PAIR(2));
                    mvwprintw(game, 2, i + 1, "%d", corsiaM[0][i]);
                    mvwprintw(game, 3, i + 1, "%d", corsiaM[1][i]);
                    attroff(COLOR_PAIR(2));
                }
                attron(COLOR_PAIR(2));
                mvwprintw(game, 4, 3, "%d", cont);
                attroff(COLOR_PAIR(2));

                if (cont == 1)
                {
                    for (i = 0; i < N_VEICOLI; i++)
                    {
                        if (t_Area.a.veicoli[i].num_veicolo == corsiaM[0][0])
                        {
                            v_aux = t_Area.a.veicoli[i];
                            if (!check_coordinata(corsiaM[1][0], t_Area, v_aux))
                            {
                                v = cambio_corsia(pipeVAW, v_aux, t_Area, corsiaM[1][0]);
                                t_Area.a.veicoli[i] = v;
                                // usleep(100000);
                                // writeVPipe(v, pipeVAW);
                            }
                        }
                    }
                }
                else if (cont == 2)
                {
                    for (i = 0; i < N_VEICOLI; i++)
                    {
                        for (j = 0; j < (N_CORSIE_FLUSSI - 1); j++)
                        {
                            if (t_Area.a.veicoli[i].num_veicolo == corsiaM[0][j])
                            {
                                v_aux = t_Area.a.veicoli[i];
                                if (!check_coordinata(corsiaM[1][j], t_Area, v_aux))
                                {
                                    v = cambio_corsia(pipeVAW, v_aux, t_Area, corsiaM[1][j]);
                                    t_Area.a.veicoli[i] = v;
                                    // usleep(100000);
                                    // writeVPipe(v, pipeVAW);
                                }
                            }
                        }
                    }
                }
                else
                {
                    for (i = 0; i < N_VEICOLI; i++)
                    {
                        for (j = 0; j < N_CORSIE_FLUSSI; j++)
                        {
                            if (t_Area.a.veicoli[i].num_veicolo == corsiaM[0][j])
                            {
                                v_aux = t_Area.a.veicoli[i];
                                if (!check_coordinata(corsiaM[1][j], t_Area, v_aux))
                                {
                                    v = cambio_corsia(pipeVAW, v_aux, t_Area, corsiaM[1][j]);
                                    t_Area.a.veicoli[i] = v;
                                    // usleep(100000);
                                    // writeVPipe(v, pipeVAW);
                                }
                            }
                        }
                    }
                }
                // aggiorna_autostrada(game, autostrada, t_Area);
                // wrefresh(game);
            }

            // usleep(50000);

            /* aggiorna_autostrada(game, autostrada, t_Area);
            wrefresh(game);
             */
        }
        strada = t_Area.a;
        // close(pipeArea[WRITE]);
        //  usleep(100001);
        aggiorna_autostrada(game, autostrada, t_Area, pipeVAW);
        // close(pipeArea[WRITE]);
        // aggiorna_fiume(game, fiume, t_Area, pipeArea);
        wrefresh(game);
        // for (i = 0; i < N_VEICOLI; i++){
        //     usleep(1000+t_Area.a.veicoli[i].num_veicolo);
        //     writeVPipe(t_Area.a.veicoli[i], pipeVAW);
        // }
            
    }
    _exit(EXIT_SUCCESS);
}
void aggiorna_fiume(WINDOW *game, WINDOW *fiume, Area a, int pipeTR[2])
{
    Area t_Area = a;
    int i, j;
    Area a_aux;

    for (i = 0; i < N_CORSIE_FLUSSI; i++)
    {

        a_aux = readAreaPipe(pipeTR[READ]);
        if (t_Area.f.f[i].t.id == a_aux.f.f[i].t.id)
        {
            t_Area.f.f[i].t = a_aux.f.f[i].t;
            stampa_tronco(game, t_Area.f.f[i].t);
        }
        if (t_Area.f.f[i].t.v == 0)
        { // sn_dx
            if (t_Area.f.f[i].t.x[0] > (MIN_X + 1))
            {
                attron(COLOR_PAIR(4));
                mvwaddch(game, t_Area.f.f[i].t.y[0], (t_Area.f.f[i].t.x[0] - 1), ' ');
                mvwaddch(game, t_Area.f.f[i].t.y[1], (t_Area.f.f[i].t.x[0] - 1), ' ');
                attroff(COLOR_PAIR(4));
            }
            else if (t_Area.f.f[i].t.x[0] == (MIN_X + 1))
            {
                attron(COLOR_PAIR(4));
                mvwaddch(game, t_Area.f.f[i].t.y[0], t_Area.f.f[i].t.x[1], ' ');
                mvwaddch(game, t_Area.f.f[i].t.y[1], t_Area.f.f[i].t.x[1], ' ');
                attroff(COLOR_PAIR(4));
            }
        }
        else
        {
            if (t_Area.f.f[i].t.x[1] < (MAX_X - 1))
            {
                attron(COLOR_PAIR(4));
                mvwaddch(game, t_Area.f.f[i].t.y[0], t_Area.f.f[i].t.x[1], ' ');
                mvwaddch(game, t_Area.f.f[i].t.y[1], t_Area.f.f[i].t.x[1], ' ');
                attroff(COLOR_PAIR(4));
            }
            else if (t_Area.f.f[i].t.x[1] == (MAX_X - 1))
            {
                attron(COLOR_PAIR(4));
                mvwaddch(game, t_Area.f.f[i].t.y[0], t_Area.f.f[i].t.x[0] - 1, ' ');
                mvwaddch(game, t_Area.f.f[i].t.y[1], t_Area.f.f[i].t.x[0] - 1, ' ');
                attroff(COLOR_PAIR(4));
            }
        }
    }
    // close(pipeTR[READ]);
    wbkgd(fiume, COLOR_PAIR(4));
    // wrefresh(game);
}
void gestione_fiume(int pipeAF[2], Area area)
{
    pid_t pidT[3];
    int i, pipeT[2];
    Tronco t;

    if (createPipe(pipeT, UNBLOCK_W) != 1)
    {
        perror("Errore creazione pipe area");
        _exit(-1);
    }

    for (i = 0; i < N_CORSIE_FLUSSI; i++)
    {
        switch (pidT[i] = fork())
        {
        case -1:
            perror("Errore fork tronco");
            _exit(-1);
        case 0:
            sposta_tronco(pipeT, area.f.f[i].t);
            _exit(EXIT_SUCCESS);
        }
    }
    // close(pipeT[WRITE]);
    while (1)
    {
        t = readTPipe(pipeT[READ]);
        for (i = 0; i < N_CORSIE_FLUSSI; i++)
        {
            if (t.id == area.f.f[i].t.id)
                area.f.f[i].t = t;
        }
        usleep(1000);
        writeAreaPipe(area, pipeAF[WRITE]);
    }
}
_Bool check_corsia(Veicolo v)
{
    if (v.vr == 0)
    {
        if (v.x[1] == MAX_X - 1)
            return true;
    }
    else
    {
        if (v.x[0] == MIN_X + 1)
            return true;
    }
    return false;
}
void aggiorna_autostrada(WINDOW *game, WINDOW *strada, Area a, int pipeVAW)
{
    Area t_Area = a;
    int i, j, k;
    _Bool check = false;
    

    for (i = 0; i < N_VEICOLI; i++)
    {
        
        stampa_veicolo(game, t_Area.a.veicoli[i]);
        if (t_Area.a.veicoli[i].vr == 0 && t_Area.a.veicoli[i].num_veicolo == a.a.veicoli[i].num_veicolo)
        { // sn_dx
            if (t_Area.a.veicoli[i].x[1] < (MAX_X - 1))
            {
                attron(COLOR_PAIR(2));
                mvwaddch(game, t_Area.a.veicoli[i].y[0], (t_Area.a.veicoli[i].x[0] - 1), ' ');
                mvwaddch(game, t_Area.a.veicoli[i].y[1], (t_Area.a.veicoli[i].x[0] - 1), ' ');
                attroff(COLOR_PAIR(2));
            }
            else if (t_Area.a.veicoli[i].x[1] == (MAX_X - 1))
            {
                for (j = t_Area.a.veicoli[i].x[0]; j < (t_Area.a.veicoli[i].x[0] + t_Area.a.veicoli[i].l); j++)
                {
                    attron(COLOR_PAIR(2));
                    mvwaddch(game, t_Area.a.veicoli[i].y[0], j, ' ');
                    mvwaddch(game, t_Area.a.veicoli[i].y[1], j, ' ');
                    attroff(COLOR_PAIR(2));
                }
            }
        }
        else if (t_Area.a.veicoli[i].vr == 1 && t_Area.a.veicoli[i].num_veicolo == a.a.veicoli[i].num_veicolo)
        {
            if (t_Area.a.veicoli[i].x[0] > (MIN_X + 2))
            {
                attron(COLOR_PAIR(2));
                mvwaddch(game, t_Area.a.veicoli[i].y[0], t_Area.a.veicoli[i].x[1] + 1, ' ');
                mvwaddch(game, t_Area.a.veicoli[i].y[1], t_Area.a.veicoli[i].x[1] + 1, ' ');
                attroff(COLOR_PAIR(2));
            }
            else if (t_Area.a.veicoli[i].x[0] == (MIN_X + 2))
            {
                for (j = t_Area.a.veicoli[i].x[0]; j <= (t_Area.a.veicoli[i].x[0] + t_Area.a.veicoli[i].l); j++)
                {
                    attron(COLOR_PAIR(2));
                    mvwaddch(game, t_Area.a.veicoli[i].y[0], j, ' ');
                    mvwaddch(game, t_Area.a.veicoli[i].y[1], j, ' ');
                    attroff(COLOR_PAIR(2));
                }
            }
        }
        a.a.veicoli[i] = t_Area.a.veicoli[i];
    }
    wbkgd(strada, COLOR_PAIR(2));
}

void gestione_strada(int pipeVAuxR, int pipeVW, Veicolo v)
{
    _Bool check = false;
    int time_write, time_read;
    Veicolo aux;
    

    while (true)
    {
        if (v.num_veicolo == 1)
            time_write = 66665;
        else
            time_write = 66665 + time_write;
            
        /* if(check){
            if (v.num_veicolo == 1)
                time_read = 60001;
            else
                time_read = time_write - 3;        
        
            usleep(time_read);
            aux = readVPipe(pipeVAuxR);
            if (aux.num_veicolo == v.num_veicolo)
                v = aux;
        } */
            
        
        v = sposta_veicolo(pipeVAuxR, pipeVW, v);
        check = true;

        usleep(time_write);
        writeVPipe(v, pipeVW);
    }
    _exit(EXIT_SUCCESS);
}

Veicolo sposta_veicolo(int pipeVAuxR, int pipeVW, Veicolo v)
{
    Veicolo veicolo, aux = v;
    int corsia, i;
    veicolo = v;
    Autostrada strada;
    _Bool check = false;

    /* while (true)
    {
        
    } */
    if (veicolo.vr == 0)
        { // verso sn-dx

            if (veicolo.x[1] < (MAX_X - 1))
            {
                veicolo.x[0]++;
                veicolo.x[1]++;
            } /*  else {
                 aux = readVPipe(pipeVAuxR);
                 if(aux.num_veicolo == veicolo.num_veicolo)
                     veicolo = aux;
             } */
            
        }
        else
        { // verso dx-sn

            if (veicolo.x[0] > MIN_X + 1)
            {
                veicolo.x[0]--;
                veicolo.x[1]--;
            } /* else {
                aux = readVPipe(pipeVAuxR);
                if(aux.num_veicolo == veicolo.num_veicolo)
                    veicolo = aux;
            }  */
        }
        /* usleep(100000);
        writeVPipe(veicolo, pipeVW); */
    //_exit(EXIT_SUCCESS);
        return veicolo;
}

Veicolo cambio_corsia(int pipeVAuxW, Veicolo v, Area a, int corsia)
{
    Veicolo veicolo;
    int cont = 0, index;
    veicolo = v;

    if (check_coordinata(corsia, a, veicolo) == false)
    {
        if (veicolo.vr == 0)
        { // verso sn-dx
            if ((veicolo.corsia == 1) || (veicolo.corsia == 3))
            {
                if (corsia == 1 || corsia == 3)
                {
                    veicolo.x[0] = MIN_X + 2;
                    if (veicolo.tipo == 'a')
                        veicolo.x[1] = veicolo.x[0] + L_RANA;
                    else
                        veicolo.x[1] = veicolo.x[0] + (L_RANA + 3);
                    if (corsia == 1)
                    {
                        veicolo.y[0] = 20;
                        veicolo.y[1] = 21;
                    }
                    if (corsia == 3)
                    {
                        veicolo.y[0] = 16;
                        veicolo.y[1] = 17;
                    }
                }
                else
                {
                    veicolo.x[1] = MAX_X - 2;
                    if (veicolo.tipo == 'a')
                        veicolo.x[0] = veicolo.x[1] - L_RANA;
                    else
                        veicolo.x[0] = veicolo.x[1] - (L_RANA + 3);
                    veicolo.y[0] = 18;
                    veicolo.y[1] = 19;
                    veicolo.vr = 1;
                }
            }
            else
            {
                if (corsia == 2)
                {
                    veicolo.x[0] = MIN_X + 1;
                    if (veicolo.tipo == 'a')
                        veicolo.x[1] = veicolo.x[0] + L_RANA;
                    else
                        veicolo.x[1] = veicolo.x[0] + (L_RANA + 3);
                }
                else if (corsia == 1 || corsia == 3)
                {

                    veicolo.x[1] = MAX_X - 2;
                    if (veicolo.tipo == 'a')
                        veicolo.x[0] = veicolo.x[1] - L_RANA;
                    else
                        veicolo.x[0] = veicolo.x[1] - (L_RANA + 3);
                    if (corsia == 1)
                    {
                        veicolo.y[0] = 20;
                        veicolo.y[1] = 21;
                    }
                    else
                    {
                        veicolo.y[0] = 16;
                        veicolo.y[1] = 17;
                    }
                    veicolo.vr = 1;
                }
            }
        }
        else
        { // verso dx-sn

            if ((veicolo.corsia == 1) || (veicolo.corsia == 3))
            {
                if (corsia == 1 || corsia == 3)
                {
                    veicolo.x[1] = MAX_X - 2;
                    if (veicolo.tipo == 'a')
                        veicolo.x[0] = veicolo.x[1] - L_RANA;
                    else
                        veicolo.x[0] = veicolo.x[1] - (L_RANA + 3);
                    if (corsia == 1)
                    {
                        veicolo.y[0] = 20;
                        veicolo.y[1] = 21;
                    }
                    if (corsia == 3)
                    {
                        veicolo.y[0] = 16;
                        veicolo.y[1] = 17;
                    }
                }
                else
                {
                    veicolo.x[0] = MIN_X + 2;
                    if (veicolo.tipo == 'a')
                        veicolo.x[1] = veicolo.x[0] + L_RANA;
                    else
                        veicolo.x[1] = veicolo.x[0] + (L_RANA + 3);
                    veicolo.y[0] = 18;
                    veicolo.y[1] = 19;
                    veicolo.vr = 0;
                }
            }
            else
            {
                if (corsia == 2)
                {
                    veicolo.x[1] = MAX_X - 2;
                    if (veicolo.tipo == 'a')
                        veicolo.x[0] = veicolo.x[1] - L_RANA;
                    else
                        veicolo.x[0] = veicolo.x[1] - (L_RANA + 3);
                }
                else if (corsia == 1 || corsia == 3)
                {
                    veicolo.x[0] = MIN_X + 2;

                    if (veicolo.tipo == 'a')
                        veicolo.x[1] = veicolo.x[0] + L_RANA;
                    else
                        veicolo.x[1] = veicolo.x[0] + (L_RANA + 3);

                    if (corsia == 1)
                    {
                        veicolo.y[0] = 20;
                        veicolo.y[1] = 21;
                    }
                    else
                    {
                        veicolo.y[0] = 16;
                        veicolo.y[1] = 17;
                    }
                    veicolo.vr = 0;
                }
            }
        }
        veicolo.corsia = corsia;
        // usleep(125000);
        // writeVPipe(veicolo, pipeVAuxW);
    }
    return veicolo;
}

_Bool check_coordinata(int corsia, Area strada, Veicolo v)
{
    int i, j, t_x;

    if (v.vr == 0)
    {
        if ((v.corsia == 2 && corsia != 2) || ((v.corsia == 1 || v.corsia == 3) && corsia == 2))
        {
            t_x = MIN_X + 1;
            for (i = 0; i < N_VEICOLI; i++)
            {
                if (strada.a.veicoli[i].corsia == corsia)
                {
                    if (strada.a.veicoli[i].x[1] > t_x)
                        t_x = strada.a.veicoli[i].x[1];
                }
            }
            if (t_x > ((MAX_X - 1) - (v.l + 1))) // non c'è abbastanza spazio
                return true;
            else
                return false;
        }
        else
        {
            t_x = MAX_X - 1;
            for (i = 0; i < N_VEICOLI; i++)
            {
                if (strada.a.veicoli[i].corsia == corsia)
                {
                    if (strada.a.veicoli[i].x[0] < t_x)
                        t_x = strada.a.veicoli[i].x[0];
                }
            }
            if (t_x < ((MIN_X + 1) + (v.l + 1)))
                return true;
            else
                return false;
        }
    }
    else
    {
        if ((v.corsia == 2 && corsia != 2) || ((v.corsia == 1 || v.corsia == 3) && corsia == 2))
        {

            t_x = MAX_X - 1;
            for (i = 0; i < N_VEICOLI; i++)
            {
                if (strada.a.veicoli[i].corsia == corsia)
                {
                    if (strada.a.veicoli[i].x[0] < t_x)
                        t_x = strada.a.veicoli[i].x[0];
                }
            }
            if (t_x > ((MAX_X - 1) - (v.l + 1)))
                return true;
            else
                return false;
        }
        else
        {
            t_x = MIN_X + 1;
            for (i = 0; i < N_VEICOLI; i++)
            {
                if (strada.a.veicoli[i].corsia == corsia)
                {
                    if (strada.a.veicoli[i].x[1] > t_x)
                        t_x = strada.a.veicoli[i].x[1];
                }
            }
            if (t_x < ((MIN_X + 1) + (v.l + 1)))
                return true;
            else
                return false;
        }
    }
}
void writeAreaPipe(Area area, int p_out)
{
    write(p_out, &area, sizeof(area));
}

Area readAreaPipe(int p_in)
{
    Area area;
    read(p_in, &area, sizeof(area));
    return area;
}
