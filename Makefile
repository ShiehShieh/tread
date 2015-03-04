FLAGS=-lncurses

tread : src/tread.o src/bookcase.o src/tr_ui.o src/util.o
	(cd src; gcc -o ../tread tread.o bookcase.o tr_ui.o util.o $(FLAGS))

tread.o : src/tread.c src/bookcase.h src/tr_ui.c
	(cd src; gcc -c tread.c $(FLAGS))

bookcase.o : src/bookcase.c src/bookcase.h src/util.h
	(cd src; gcc -c bookcase.c $(FLAGS))

tr_ui.o : src/tr_ui.c src/bookcase.h src/util.h
	(cd src; gcc -c tr_ui.c $(FLAGS))

util.o : src/util.c
	(cd src; gcc -c util.c $(FLAGS))

clean :
	rm tread
	(cd src;rm tread.o bookcase.o tr_ui.o util.o $(FLAGS))

