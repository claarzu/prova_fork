#include "area_gioco.h"

_Bool menu_gioco();
void stampa_intro();

int main(){
    srand(time(NULL));
    WINDOW *game, *marciapiede, *autostrada, *prato, *fiume, *tane;
    int kcode = -1, zona = 1, colore_tane, cont = 0;
    int pipeA[2], pipeVAux[2], pipeV[2];
    // pipe(pipeP);// Descrittori pipe
    pid_t pidA[3], pidV[N_VEICOLI]; // Pid processi figli
    _Bool loop = true, go = true, gioca = false;
    Rana r;
    Area area;
    Tronco t_aux;
    int i, j;
    Veicolo v; 
    Proiettile aux_p;

    initscr();   // Inizializza schermo
    noecho();    // Imposta la modalità della tastiera
    curs_set(0); // nasconde il cursore
    start_color();
    keypad(stdscr, true); // Abilita la gestione dei tasti freccia
    init_color(COLOR_LBROWN, 150, 100, 50);
    init_color(COLOR_LYELLOW, 1500, 1500, 0);
    stampa_intro(); // stampa schermata iniziale
    clear();
    refresh();
    gioca = menu_gioco(); // gestione e stampa del menù iniziale
    clear();
    refresh();

    if (gioca){
        game = newwin(MAX_Y, MAX_X, 0, 0);
        box(game, ACS_VLINE, ACS_HLINE);
        init_color(COLOR_GREY, 90, 80, 70);     //
        init_color(COLOR_LGREY, 250, 250, 250); // Colore marrone
        init_color(COLOR_LRED, 600, 0, 0);
        init_color(COLOR_LCYAN, 0, 700, 700);
        init_pair(1, COLOR_LGREY, COLOR_LGREY);
        init_pair(2, COLOR_GREY, COLOR_GREY);
        init_pair(3, COLOR_GREEN, COLOR_GREEN);
        init_pair(4, COLOR_BLUE, COLOR_BLUE);

        marciapiede = subwin(game, H_RANA, MAX_X - 2, MARCIAPIEDE_Y_MIN, MIN_X + 1);
        wbkgd(marciapiede, COLOR_PAIR(1));
        box(marciapiede, ACS_VLINE, ACS_HLINE);
        autostrada = subwin(game, H_STRADA, MAX_X - 2, STRADA_Y_MIN, MIN_X + 1);
        wbkgd(autostrada, COLOR_PAIR(2));
        prato = subwin(game, H_RANA, MAX_X - 2, PRATO_Y_MIN, MIN_X + 1);
        wbkgd(prato, COLOR_PAIR(3));
        box(prato, ACS_VLINE, ACS_HLINE);
        fiume = subwin(game, H_FIUME, MAX_X - 2, FIUME_Y_MIN, MIN_X + 1);
        wbkgd(fiume, COLOR_PAIR(4));
        box(fiume, ACS_VLINE, ACS_HLINE);
        init_color(COLOR_BROWN, 255, 100, 50);
        init_pair(5, COLOR_BROWN, COLOR_BROWN);
        tane = subwin(game, H_RANA + 1, MAX_X - 2, TANA_Y_MIN, MIN_X + 1);
        wbkgd(tane, COLOR_PAIR(5));
        r = init_rana(MAX_Y);
        area = init_area(marciapiede, autostrada, prato, fiume, tane);
        stampa_tane(game, area.t);
        stampa_tronchi(game, area.f);
        stampa_veicoli(game, area.a);
        wrefresh(game);


        if (createPipe(pipeV, UNBLOCK_W) != 1){
            perror("Errore creazione pipe area");           
            _exit(-1);
        }
        if (createPipe(pipeVAux, UNBLOCK_RW) != 1){
            perror("Errore creazione pipe area");           
            _exit(-1);
        }
        for (i = 0; i < N_VEICOLI; i++){
            switch (pidV[i] = fork()){
                case -1:
                    perror("Errore fork veicolo");
                    _exit(-1);
                case 0:  
                    close(pipeV[READ]); 
                    close(pipeVAux[WRITE]); 
                    sposta_veicolo(pipeVAux[READ],  pipeV[WRITE], area.a.veicoli[i]);
                    _exit(EXIT_SUCCESS);   
            }
        }
        close(pipeV[WRITE]); 
        close(pipeVAux[READ]); 
        aggiornaArea(game, fiume, autostrada, area, pipeV[READ], pipeVAux[WRITE]);
            /* for (i = 0; i < N_CORSIE_FLUSSI; i++){
            switch (pidA[i] = fork()){
                case -1:
                    perror("Errore fork tronco");
                    _exit(-1);
                case 0:
                    //close(pipeA[READ]);
                    if(i == 0){
                       // gestione_strada(pipeA, pipeVAux, area);
                    }else if(i == 1){
                        gestione_fiume(pipeA, area);  
                    }else{
                        // Rana
                    }                 
                    _exit(0);
            }
        } */ 
        
    }

    endwin();
    return 0;
}

void stampa_intro()
{
    int i, j, row = 23, column = 129;
    char sprite_intro[23][129] = {
        "BBBBBBBBBBBBBBBBB                                                              GGGGGGGGGGGGG                                     ",
        "B::::::::::::::::B                                                          GGG::::::::::::G                                     ",
        "B::::::BBBBBB:::::B                                                       GG:::::::::::::::G                                     ",
        "BB:::::B     B:::::B                                                     G:::::GGGGGGGG::::G                                     ",
        "  B::::B     B:::::B  aaaaaaaaaaaaa  uuuuuu    uuuuuunnnn  nnnnnnnn     G:::::G       GGGGGG   ooooooooooo   rrrrr   rrrrrrrrr   ",
        "  B::::B     B:::::B  a::::::::::::a u::::u    u::::un:::nn::::::::nn  G:::::G               oo:::::::::::oo r::::rrr:::::::::r  ",
        "  B::::BBBBBB:::::B   aaaaaaaaa:::::au::::u    u::::un::::::::::::::nn G:::::G              o:::::::::::::::or:::::::::::::::::r ",
        "  B:::::::::::::BB             a::::au::::u    u::::unn:::::::::::::::nG:::::G    GGGGGGGGGGo:::::ooooo:::::orr::::::rrrrr::::::r",
        "  B::::BBBBBB:::::B     aaaaaaa:::::au::::u    u::::u  n:::::nnnn:::::nG:::::G    G::::::::Go::::o     o::::o r:::::r     r:::::r",
        "  B::::B     B:::::B  aa::::::::::::au::::u    u::::u  n::::n    n::::nG:::::G    GGGGG::::Go::::o     o::::o r:::::r     rrrrrrr",
        "  B::::B     B:::::B a::::aaaa::::::au::::u    u::::u  n::::n    n::::nG:::::G        G::::Go::::o     o::::o r:::::r            ",
        "  B::::B     B:::::Ba::::a    a:::::au:::::uuuu:::::u  n::::n    n::::n G:::::G       G::::Go::::o     o::::o r:::::r            ",
        "BB:::::BBBBBB::::::Ba::::a    a:::::au:::::::::::::::uun::::n    n::::n  G:::::GGGGGGGG::::Go:::::ooooo:::::o r:::::r            ",
        "B:::::::::::::::::B a:::::aaaa::::::a u:::::::::::::::un::::n    n::::n   GG:::::::::::::::Go:::::::::::::::o r:::::r            ",
        "B::::::::::::::::B   a::::::::::aa:::a uu::::::::uu:::un::::n    n::::n     GGG::::::GGG:::G oo:::::::::::oo  r:::::r            ",
        "BBBBBBBBBBBBBBBBB     aaaaaaaaaa  aaaa   uuuuuuuu  uuuunnnnnn    nnnnnn        GGGGGG   GGGG   ooooooooooo    rrrrrrr            ",
        "                                                                                                                                 ",
        "                         __      __           _                    _____                             _                           ",
        "                         \\ \\    / /          (_)                  |  __ \\                           (_)                       ",
        "                          \\ \\  / /__ _ __ ___ _  ___  _ __   ___  | |__) | __ ___   ___ ___  ___ ___ _                         ",
        "                           \\ \\/ / _ \\ '__/ __| |/ _ \\| '_ \\ / _ \\ |  ___/ '__/ _ \\ / __/ _ \\/ __/ __| |                  ",
        "                            \\  /  __/ |  \\__ \\ | (_) | | | |  __/ | |   | | | (_) | (_|  __/\\__ \\__ \\ |                    ",
        "                             \\/ \\___|_|  |___/_|\\___/|_| |_|\\___| |_|   |_|  \\___/ \\___\\___||___/___/_|                   "};

    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_LBROWN);
    WINDOW *splash_screen;
    splash_screen = newwin(25, 135, 0, 0);
    box(splash_screen, ACS_VLINE, ACS_HLINE);
    wbkgd(splash_screen, COLOR_PAIR(1));

    stampa_sprite(splash_screen, row, column, sprite_intro, 1, 3, 0);
    wrefresh(splash_screen);
    sleep(2);
    delwin(splash_screen);
}
_Bool menu_gioco(){
    int c, columng = 21, columne = 17, rowg = 4, rowe = 4;
    _Bool selezione = true, gioca = true;
    char sprite_gioca[4][21] = {
        "  _   _   _   _   _  ",
        " / \\ / \\ / \\ / \\ / \\ ",
        "( G | I | O | C | A )",
        " \\_/ \\_/ \\_/ \\_/ \\_/ ",
    };
    char sprite_esci[4][17] = {
        "  _   _   _   _  ",
        " / \\ / \\ / \\ / \\ ",
        "( E | S | C | I )",
        " \\_/ \\_/ \\_/ \\_/ ",
    };
    
    WINDOW *sfondo_screen, *gioca_screen, *esci_screen;
    sfondo_screen = newwin(27, 102, 0, 0);
    gioca_screen = subwin(sfondo_screen, 7, 23, 3, 42);
    esci_screen = subwin(sfondo_screen, 7, 23, 11, 42);
    init_pair(2, COLOR_YELLOW, COLOR_LBROWN);
    init_pair(1, COLOR_LYELLOW, COLOR_LYELLOW);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);

    wattron(sfondo_screen, COLOR_PAIR(2));
    box(sfondo_screen, ACS_VLINE, ACS_HLINE);
    wattroff(sfondo_screen, COLOR_PAIR(2));
    wbkgd(sfondo_screen, COLOR_PAIR(2));
    wattron(gioca_screen, COLOR_PAIR(1));
    box(gioca_screen, ACS_VLINE, ACS_HLINE);
    wattroff(gioca_screen, COLOR_PAIR(1));
    wbkgd(gioca_screen, COLOR_PAIR(2));
    wattron(esci_screen, COLOR_PAIR(2));
    box(esci_screen, ACS_VLINE, ACS_HLINE);
    wattroff(esci_screen, COLOR_PAIR(2));
    wbkgd(esci_screen, COLOR_PAIR(2));

    stampa_sprite(gioca_screen, rowg, columng, sprite_gioca, 1, 1, 2);
    stampa_sprite(esci_screen, rowe, columne, sprite_esci, 1, 3, 2);
    mvwprintw(sfondo_screen, 23, 22, "Select using the arrow up or down, then press space to start.");
    wrefresh(gioca_screen);
    wrefresh(sfondo_screen);
    wrefresh(esci_screen);

    while (selezione) { // Gestione spostamento guardia
        c = getch();
        switch (c){
        case KEY_UP:
            wattron(gioca_screen, COLOR_PAIR(1));
            box(gioca_screen, ACS_VLINE, ACS_HLINE);
            wattroff(gioca_screen, COLOR_PAIR(1));
            wattron(esci_screen, COLOR_PAIR(2));
            box(esci_screen, ACS_VLINE, ACS_HLINE);
            wattroff(esci_screen, COLOR_PAIR(2));
            wrefresh(gioca_screen);
            wrefresh(esci_screen);
            gioca = true;
            break;
        case KEY_DOWN:
            wattron(gioca_screen, COLOR_PAIR(2));
            box(gioca_screen, ACS_VLINE, ACS_HLINE);
            wattroff(gioca_screen, COLOR_PAIR(2));
            wattron(esci_screen, COLOR_PAIR(1));
            box(esci_screen, ACS_VLINE, ACS_HLINE);
            wattroff(esci_screen, COLOR_PAIR(1));
            wrefresh(gioca_screen);
            wrefresh(esci_screen);
            gioca = false;
            break;
        case 10: // tasto barra spaziatrice
            selezione = false;

            if (gioca) {
                delwin(sfondo_screen);
                delwin(gioca_screen);
                delwin(esci_screen);
                return gioca;
            }else{
                delwin(sfondo_screen);
                delwin(gioca_screen);
                delwin(esci_screen);
                return gioca;
            }
            break;
        default:
            break;
        }
    }
    delwin(sfondo_screen);
    delwin(gioca_screen);
    delwin(esci_screen);
}
