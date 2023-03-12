#include "rana.h"

Rana init_rana(int maxy){
    Rana r;
    int i, j;
    r.num_vite = V_RANA;
    // todo da modificare x e y rana come array
    r.x = 0;
    r.y = maxy;

    r.l = H_RANA;
    r.h = H_RANA;
    for (i = 0; i < H_RANA; i++){
        for ( j = 0; j < L_RANA; j++){
            if (i == 0){
                if (j == 0 || j == 1 || j == 3 || j == 4)
                    r.sprite[i][j] =' ';
                else if (j == 2)
                    r.sprite[i][j] ='o';
            } else {
                if (j == 0 || j == 4)
                    r.sprite[i][j] ='_';
                else if (j == 1 || j == 3)
                    r.sprite[i][j] ='\'';
                else if (j == 2)
                    r.sprite[i][j] ='0';
            }
        }
    }
    return r;
}
Rana *copia_rana(Rana *r){
    Rana *tmp_rana;
    int i;
    tmp_rana = (Rana*)malloc(sizeof(Rana)*1);
    tmp_rana->h = r->h;
    tmp_rana->l = r->l;
    tmp_rana->x = r->x;
    tmp_rana->y = r->y;
    free(r);
    return tmp_rana;
}
void gestione_rana(Rana rana, int maxx, int maxy, int pipeout, _Bool *go){
    int c;
    keypad(stdscr, true); // Abilita la gestione dei tasti freccia

    while(*go == true){ // Gestione spostamento guardia
        c = getch();
        switch(c) {
            case KEY_UP:
                if(rana.y > 0)
                    rana.y -= 1;
                break;
            case KEY_DOWN:
                if(rana.y < maxy - 1)
                    rana.y += 1;
                break;
            case KEY_LEFT:
                if(rana.x > 0)
                    rana.x -= 1;
                break;
            case KEY_RIGHT:
                if(rana.x < maxx - 1)
                    rana.x += 1;
                break;
            case ESCI:
                *go = false;
                break;
            default:
                break;
        }
        write(pipeout,&rana,sizeof(rana));
    }
}
void stampa_sprite(WINDOW *w,int row, int column,char s[row][column],int y, int x, int color_pair){
    int i, j;
    int y_row = y, x_column;
    for (i = 0; i < row; ++i) {
        x_column = x;
        for (j = 0; j < column; ++j) {
            wattron(w, COLOR_PAIR(color_pair));
            mvwprintw(w,y_row,x_column,"%c", s[i][j]);
            wattroff(w, COLOR_PAIR(color_pair));
            x_column++;
        }
        y_row++;
    }
}
void writePPipe(Proiettile p, int p_out){
    write(p_out, &p, sizeof(p));
}
Proiettile readPPipe(int p_in){
    Proiettile p;
    read(p_in, &p, sizeof(p));
    return p;
}

int createPipe(int fd[2], int mode){

    if (pipe(fd)==-1) {
        perror("Errore nella creazione della pipe.");
        exit(1);
    }
    if (mode==UNBLOCK_R){
        if ( fcntl( fd[0], F_SETFL, O_NDELAY) < 0 ) {
            perror("Error unblocking the pipe. ");
            return 0;
        }
    }
    if (mode==UNBLOCK_W){
        if ( fcntl( fd[1], F_SETFL, O_NDELAY) < 0 ) {
            perror("Error unblocking the pipe. ");
            return 0;
        }
    }
    if (mode==UNBLOCK_RW){
        if ( fcntl( fd[0], F_SETFL, O_NDELAY) < 0 ) {
            perror("Error unblocking the pipe. ");
            return 0;
        }
        if ( fcntl( fd[1], F_SETFL, O_NDELAY) < 0 ) {
            perror("Error unblocking the pipe. ");
            return 0;
        }
    }
    return 1;
}

_Bool countdown(unsigned int sec){
    unsigned int current =  time(0) + sec;
    while (time(0) < current);
    return true;
}