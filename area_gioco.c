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

void aggiornaArea(WINDOW *game, WINDOW *fiume, WINDOW *autostrada, Area area, int pipeVR, int pipeVAW, int pid[15])
{
    Area t_Area = area, tmp_a = area;
    Autostrada strada;
    int i, j, cont = 0, cont_v[N_VEICOLI], corsiaM[2][3], corsia = 0, time_read = 16666, time_write = 100, aux = 2;
    _Bool check = false, check2 = false;
    int valueR = 105, valueW = 100;
    Tronco t_aux;
    Veicolo v, v_aux, veicoli[N_VEICOLI];

    while (1)
    {   
        usleep(1000000);
        v.num_veicolo = 0;
        cont = 0;
        check = false;
        for (i = 0; i < N_VEICOLI; i++)
            cont_v[i] = 0;
        for (i = 0; i < N_CORSIE_FLUSSI; i++){
            corsiaM[0][i] = 0;
            corsiaM[1][i] = 0;
        }
        
        
        // aggiorna_autostrada(game, autostrada, t_Area, pipeVAW);
        while (check == false) // ciclo per ottenere tutti i veicoli da visualizzare
        {
            
            v = readVPipe(pipeVR);
            if (!check_id(N_VEICOLI, cont_v, v.num_veicolo))
            {                
                for (j = 0; j < N_VEICOLI; j++)
                {                    
                    if (v.num_veicolo == t_Area.a.veicoli[j].num_veicolo)
                    {
                        cont_v[j] = v.num_veicolo;
                        t_Area.a.veicoli[j] = v;
                        veicoli[j] = v;
                        cont++;
                    }                    
                }
                if (cont == 15)
                    check = true;
            }
        }
        cont = 0;
       
        
        
        
        // se tutti i veicoli sono stati caricati
        if (check)
        {
            aggiorna_autostrada(game, autostrada, t_Area, pipeVAW);
            wrefresh(game);
            /* for (i = 0; i < N_VEICOLI; i++)
            {
                if (check_corsia(t_Area.a.veicoli[i]))
                {
                    if (t_Area.a.veicoli[i].vr == 0)
                    {
                        t_Area.a.veicoli[i].x[0] = 2;
                        if (t_Area.a.veicoli[i].tipo == 'a')
                            t_Area.a.veicoli[i].x[1] = 7;
                        else
                            t_Area.a.veicoli[i].x[1] = 10;
                    }
                    else{
                        t_Area.a.veicoli[i].x[1] = 100;
                        if (t_Area.a.veicoli[i].tipo == 'a')
                            t_Area.a.veicoli[i].x[0] = 95;
                        else
                            t_Area.a.veicoli[i].x[0] = 92;
                    }

                }

            } */
            for (i = 0; i < N_VEICOLI; i++) // riempimento della matrice [0] con gli id dei veicoli da spostare
            {
                if (check_fine_corsia(t_Area.a.veicoli[i]))
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

            for (i = 0; i < N_VEICOLI; i++)
            {
                for (j = 0; j < cont; j++)
                {

                    if (veicoli[i].num_veicolo == corsiaM[0][j])
                    {
                        v_aux = veicoli[i];
                        do
                        {
                            corsia = 1 + rand() % 3;
                        } while (check_Inserimento_corsia(corsia, t_Area, v_aux) == true && (corsia == corsiaM[1][0] || corsia == corsiaM[1][1] || corsia == corsiaM[1][2]));
                        corsiaM[1][j] = corsia;
                        v_aux = cambio_corsia(pipeVAW, veicoli[i], t_Area, corsiaM[1][j]);
                        veicoli[i] = v_aux;
                        t_Area.a.veicoli[i] = veicoli[i];
                        /* usleep(100000);*/
                        //writeVPipe(t_Area.a.veicoli[i], pipeVAW); 
                    }
                }
                
            }

            for (i = 0; i < N_VEICOLI; i++){
                t_Area.a.veicoli[i] = veicoli[i];
            }
            
                
        }
          
        // close(pipeArea[WRITE]);
        // usleep(100001);

        // close(pipeArea[WRITE]);
        // aggiorna_fiume(game, fiume, t_Area, pipeArea);

        //usleep(1000000);
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
_Bool check_fine_corsia(Veicolo v)
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
        { // dx_sn
            if (t_Area.a.veicoli[i].x[0] > (MIN_X + 1))
            {
                attron(COLOR_PAIR(2));
                mvwaddch(game, t_Area.a.veicoli[i].y[0], t_Area.a.veicoli[i].x[1] + 1, ' ');
                mvwaddch(game, t_Area.a.veicoli[i].y[1], t_Area.a.veicoli[i].x[1] + 1, ' ');
                attroff(COLOR_PAIR(2));
            }
            else if (t_Area.a.veicoli[i].x[0] == (MIN_X + 1))
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
    int i;
    Veicolo aux = v;
    

    while (true){
        usleep(800000);
        
        v.pid = getpid();
        
        /* if(check){
            aux = readVPipe(pipeVAuxR);
            if(aux.pid == v.pid){
                v = aux;
                //v = sposta_veicolo(pipeVAuxR, pipeVW, v);
            }
        } */
        //usleep(66666);
        /* aux = readVPipe(pipeVAuxR);
        if(aux.num_veicolo == v.num_veicolo)
            v = aux; */
        //sleep(1);
        if(v.x[1] == MAX_X-1 || v.x[0] == MIN_X+1)
            check = true; 
        else
            check = false;


        if (check)
        {
            aux = readVPipe(pipeVAuxR);
            if(aux.num_veicolo == v.num_veicolo)
                v = aux;
        }       
        
        v = sposta_veicolo(v);
        //usleep(66665);         
        writeVPipe(v, pipeVW);        
    }
    _exit(EXIT_SUCCESS);
}

Veicolo sposta_veicolo(Veicolo v)
{
    Veicolo veicolo, aux = v;
    int corsia, i;
    veicolo = v;
    Autostrada strada;
    _Bool check = false;

    if (veicolo.vr == 0)
    { // verso sn-dx

        if (veicolo.x[1] < (MAX_X - 1))
        {
            veicolo.x[0]++;
            veicolo.x[1]++;
        }
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

    return veicolo;
}

Veicolo cambio_corsia(int pipeVAuxW, Veicolo v, Area a, int corsia)
{
    Veicolo veicolo;
    int cont = 0, index;
    veicolo = v;

    if (veicolo.vr == 0)
    { // verso sn-dx
        if ((veicolo.corsia == 1) || (veicolo.corsia == 3))
        {
            if (corsia == 1 || corsia == 3)
            {
                veicolo.x[0] = MIN_X + 1;
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
            }
            else
            {
                veicolo.x[1] = MAX_X - 1;
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

                veicolo.x[1] = MAX_X - 1;

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
                veicolo.x[1] = MAX_X - 1;
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
            }
            else
            {
                veicolo.x[0] = MIN_X + 1;
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
                veicolo.x[1] = MAX_X - 1;
                if (veicolo.tipo == 'a')
                    veicolo.x[0] = veicolo.x[1] - L_RANA;
                else
                    veicolo.x[0] = veicolo.x[1] - (L_RANA + 3);
            }
            else if (corsia == 1 || corsia == 3)
            {
                veicolo.x[0] = MIN_X + 1;

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

    return veicolo;
}

_Bool check_Inserimento_corsia(int corsia, Area strada, Veicolo v)
{
    int i, j, t_x;

    if (v.vr == 0) // verso sn_dx
    {
        if ((v.corsia == 2 && (corsia == 1 || corsia == 3)) || ((v.corsia == 1 || v.corsia == 3) && corsia == 2)) // Condizione di cambio corsia
        {
            t_x = MIN_X + 1;
            for (i = 0; i < N_VEICOLI; i++){
                if (strada.a.veicoli[i].corsia == corsia && strada.a.veicoli[i].num_veicolo != v.num_veicolo)
                {
                    if (strada.a.veicoli[i].x[1] > t_x)
                        t_x = strada.a.veicoli[i].x[1];
                }
            }
            if (t_x >= ((MAX_X - 1) - (v.l + 3))) // non c'è abbastanza spazio
                return true;
        }else{
            t_x = MAX_X - 1;
            for (i = 0; i < N_VEICOLI; i++){
                if (strada.a.veicoli[i].corsia == corsia && strada.a.veicoli[i].num_veicolo != v.num_veicolo)
                {
                    if (strada.a.veicoli[i].x[0] < t_x)
                        t_x = strada.a.veicoli[i].x[0];
                }
            }
            if (t_x <= ((MIN_X + 1) + (v.l + 3)))
                return true;
        }
    }
    else
    {
        if ((v.corsia == 2 && corsia != 2) || ((v.corsia == 1 || v.corsia == 3) && corsia == 2))
        {

            t_x = MAX_X - 1;
            for (i = 0; i < N_VEICOLI; i++)
            {
                if (strada.a.veicoli[i].corsia == corsia && strada.a.veicoli[i].num_veicolo != v.num_veicolo)
                {
                    if (strada.a.veicoli[i].x[0] < t_x)
                        t_x = strada.a.veicoli[i].x[0];
                }
            }
            if (t_x < ((MIN_X + 1) + (v.l + 3)))
                return true;
        }
        else
        {
            t_x = MIN_X + 1;
            for (i = 0; i < N_VEICOLI; i++)
            {
                if (strada.a.veicoli[i].corsia == corsia && strada.a.veicoli[i].num_veicolo != v.num_veicolo)
                {
                    if (strada.a.veicoli[i].x[1] > t_x)
                        t_x = strada.a.veicoli[i].x[1];
                }
            }
            if (t_x > ((MAX_X - 1) - (v.l + 3)))
                return true;
        }
    }
    return false;
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
