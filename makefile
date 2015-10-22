all: Trabalho_Final.exe

Trabalho_Final.exe: Trabalho_Final.o
	gcc -o Trabalho_Final.exe Trabalho_Final.o

Trabalho_Final.o: Trabalho_Final.c
	gcc -c Trabalho_Final.c

clean:
	rm Trabalho_Final.o Trabalho_Final.exe
