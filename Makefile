
all:
	gcc compress.c -g -Wall -pedantic -ansi -o compress

clean:
	rm *.o compress
