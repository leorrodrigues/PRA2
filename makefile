all: Trabalho_Final

Trabalho_Final: Trabalho_Final.o
	gcc -o Trabalho_Final Trabalho_Final.o

Trabalho_Final.o: Trabalho_Final.c
	gcc -c Trabalho_Final.c

clean:
	rm Trabalho_Final.o Trabalho_Final
