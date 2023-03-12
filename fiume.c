#include "fiume.h"

void stampa_nemico(WINDOW *game, Nemico n)
{
    Proiettile aux = n.p;
    init_pair(18, COLOR_WHITE, COLOR_LBROWN);
    stampa_sprite(game, H_RANA, L_NEMICO, n.sprite, n.y[0], n.x[0], 18);
    stampa_Pro(game, aux, 0);
}
Nemico init_nemico(int *x, int *y, _Bool attivo)
{
    Nemico n;
    int i, j;
    char sprite[H_RANA][L_NEMICO] = {
        " (0 0) ",
        "(_,v,_)"};
    n.h = H_RANA;
    n.l = L_NEMICO;
    n.x[0] = x[0] + (L_RANA - 1);
    n.x[1] = n.x[0] + 6;
    n.y[0] = y[0];
    n.y[1] = y[1];
    for (i = 0; i < H_RANA; ++i)
    {
        for (j = 0; j < L_NEMICO; ++j)
            n.sprite[i][j] = sprite[i][j];
    }
    n.attivo = attivo;
    if (n.attivo)
    {
        n.p.num_proiettile = -1;
    }
    return n;
}
Tronco init_Tronco(int *y, Verso v, int id)
{
    Tronco aux;
    int i, j, xi = 0, xf = 0, nemico;

    xi = (rand() % MAXX_TRONCHI) + 1;
    xf = xi + L_TRONCO;
    // nemico = rand() % 2;
    aux.h = H_RANA;
    aux.l = L_TRONCO;
    for (i = 0; i < H_RANA; ++i)
    {
        for (j = 0; j < L_TRONCO; ++j)
        {
            if ((i == 0 || i == 1) && (j == 0 || j == 14))
                aux.sprite[i][j] = '|';
            else if (i == 1)
                aux.sprite[i][j] = '[';
            else if (i == 0)
                aux.sprite[i][j] = ']';
        }
    }
    aux.x[0] = xi;
    aux.x[1] = xf;
    aux.y[0] = y[0];
    aux.y[1] = y[1];
    aux.pieno = false;
    aux.n = init_nemico(aux.x, aux.y, false);
    aux.v = v;
    aux.id = id;
    return aux;
}
Fiume init_fiume(WINDOW *fiume)
{
    Fiume f;
    int i, verso_flusso = -1, id = 0;

    f.x[0] = MIN_X;
    f.x[1] = MAX_X;
    f.y[0] = FIUME_Y_MIN;
    f.y[1] = FIUME_Y_MAX;
    f.h = H_FIUME;
    f.l = L_FIUME;
    f.fiume = fiume;
    for (i = 0; i < N_CORSIE_FLUSSI; ++i)
    {
        if (i == 0)
        {
            if (verso_flusso == -1)
                verso_flusso = rand() % 2;
        }
        else
        {
            if (verso_flusso == 0)
                verso_flusso = 1;
            else
                verso_flusso = 0;
        }
        f.f[i].l = MAX_X;
        f.f[i].h = H_RANA;
        f.f[i].num_flusso = i + 1;
        f.f[i].x[0] = 1;
        f.f[i].x[1] = MAX_X - 1;
        if (i == 0)
        {
            f.f[i].y[0] = 12;
            f.f[i].y[1] = 13;
        }
        else if (i == 1)
        {
            f.f[i].y[0] = 10;
            f.f[i].y[1] = 11;
        }
        else
        {
            f.f[i].y[0] = 8;
            f.f[i].y[1] = 9;
        }
        f.f[i].vr = verso_flusso;
        id = i + 1;
        f.f[i].t = init_Tronco(f.f[i].y, f.f[i].vr, id);
    }
    return f;
}
void stampa_tronco(WINDOW *game, Tronco t)
{
    init_pair(9, COLOR_LGREY, COLOR_LBROWN);
    stampa_sprite(game, H_RANA, L_TRONCO, t.sprite, t.y[0], t.x[0], 9);
    if (t.n.attivo)
        stampa_nemico(game, t.n);
}
void stampa_tronchi(WINDOW *game, Fiume f)
{
    int i;
    for (i = 0; i < N_CORSIE_FLUSSI; ++i)
    {
        stampa_tronco(game, f.f[i].t);
    }
}
void sposta_tronco(int pipeT[2], Tronco t){
    int attivo1 = -1, attivo2 = -1;

    while (true){
        if (t.v == 0){
            if (t.x[1] < (MAX_X - 1)){
                t.x[0]++;
                t.x[1]++;
                t.n.x[0]++;
                t.n.x[1]++;
            }
            if (t.x[1] == (MAX_X - 1))
                t.v = 1;
        }
        else{
            if (t.x[0] > MIN_X + 1){
                t.x[0]--;
                t.x[1]--;
                t.n.x[0]--;
                t.n.x[1]--;
            }
            if (t.x[0] == (MIN_X + 1))
                t.v = 0;
        }
        if (t.n.attivo == false && t.pieno == false){
            attivo1 = 1 + rand() % 50;
            if (t.id == attivo1){
                t.n.attivo = true;
                t.pieno = true;
            }
            attivo1 = -1;
        }
        usleep(100000);
        writeTPipe(t, pipeT[WRITE]);
    }
    //_exit(EXIT_SUCCESS);
}
void manage_pn(int pipePW, int pipeTR, Tronco t)
{
    pid_t pidP[5];
    int i;

    for (i = 0; i < 5; i++)
    {
        sleep(1);
        switch (pidP[i] = fork())
        {
        case -1:
            perror("Error ForkP");
            _exit(EXIT_FAILURE);
        case 0:

            if (i > 0)
            {
                sleep(2);
                t = readTPipe(pipeTR);
                t.n.p.sprite = '1';
                if (i == 2)
                {
                    sleep(2);
                    t = readTPipe(pipeTR);
                    t.n.p.sprite = '2';
                }
                else if (i == 3)
                {
                    sleep(2);
                    t = readTPipe(pipeTR);
                    t.n.p.sprite = '3';
                }
                else if (i == 4)
                {
                    sleep(2);
                    t = readTPipe(pipeTR);
                    t.n.p.sprite = '4';
                }
            }
            else
                t.n.p.sprite = '0';
            t.n.p.y = t.n.y[1] - 1;
            t.n.p.x = t.n.x[0] + 3;
            sposta_p(pipePW, t.n.p, pidP[i]);
        }
    }

    /*
        while (i<10){
            i = 0;
            if (i > 0)
            {
                t = readTPipe(pipeTR);
                t.n.p.sprite = '1';
            }
            switch (pidP = fork())
            {
                case -1:
                    perror("Error ForkP");
                    _exit(EXIT_FAILURE);
                case 0:
                    i++;
                    t.n.p.y = t.n.y[1] - 1;
                    t.n.p.x = t.n.x[0] + 3;
                    t.n.p.sprite = '0';
                    sposta_p(pipePW, t.n.p, pidP);
            }
        } */
}

void sposta_p(int pipeP, Proiettile p, int pid)
{
    p.num_proiettile = (int)getpid();
    while (p.y < 23)
    {
        p.y++;
        usleep(1000000);
        writePPipe(p, pipeP);
        if (p.y == 23)
        {
            _exit(EXIT_SUCCESS);
        }
    }
    // kill(p.num_proiettile, 9);
    //_exit(EXIT_SUCCESS);
}

void stampa_Pro(WINDOW *game, Proiettile p, int color_Pair)
{
    attron(COLOR_PAIR(color_Pair));
    mvwaddch(game, p.y, p.x, p.sprite);
    attroff(COLOR_PAIR(color_Pair));
}

Tronco readTPipe(int p_in)
{
    Tronco t;
    read(p_in, &t, sizeof(t));
    return t;
}

void writeTPipe(Tronco t, int p_out)
{
    write(p_out, &t, sizeof(t));
}
