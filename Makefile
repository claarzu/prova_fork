game: main.o area_gioco.o autostrada.o fiume.o rana.o
	gcc -g -o game *.o -lncurses

main.o: main.c
	gcc -g -c main.c

rana.o: rana.c
	gcc -g -c rana.c -lncurses

fiume.o: fiume.c
	gcc -g -c fiume.c 

autostrada.o: autostrada.c
	gcc -g -c autostrada.c

area_gioco.o: area_gioco.c
	gcc -g -c area_gioco.c

clean:
	rm *.o
