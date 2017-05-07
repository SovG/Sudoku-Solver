CC = gcc
CFLAGS = -pedantic -ansi -Wall -g -lrt -pthread -D_XOPEN_SOURCE=500
OBJ = msvv.o fileIO.o validate.o
EXEC = msvv
EXEC : $(OBJ)
		$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)

msvv.o : msvv.c fileIO.h msvv.h validate.h
	$(CC) msvv.c -c $(CFLAGS)

fileIO.o : fileIO.c fileIO.h msvv.h
	$(CC) fileIO.c -c $(CFLAGS)

validate.o : validate.c msvv.h
	$(CC) validate.c -c $(CFLAGS)

clean :
		rm -f $(OBJ) $(EXEC) logfile.txt
