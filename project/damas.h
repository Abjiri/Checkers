#ifndef ___DAMAS_H___
#define ___DAMAS_H___

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cgi.h"

typedef enum {CLARO, ESCURO, PRETO, PCOME, REI, KCOME, BRANCO, BCOME, RAINHA, QCOME, JOGAVEL, COMER, DEVORAR, COMIDA} Peca;

typedef struct estado {
	Peca tab[10][10]; //coluna linha
	int comidas;
	Peca turno;
	char fim;
} Estado;

#define linkbase "http://localhost/cgi-bin/Damas"
#define dirbase "/usr/local/games/Damas/"

char *ficheiro[] = {"claro.png", "escuro.png", "preto.png", "pcome.png", "rei.png", "kcome.png", "branco.png", 
					"bcome.png", "rainha.png", "qcome.png", "jogavel.png", "jogavel.png", "jogavel.png", "comida.png"};

void fim (Estado *e);
Peca fimTurno (Estado e1, Estado e2);
int pecasComer (Estado *e);
void regredirRosas (Estado *e, int x, int y);
void apagarComidas (Estado *e);
void tirarAmarelos (Estado *e, int nr);
Peca valorPeca (Estado *e, int x, int y, Peca oposto);
int limites (int x, int y, int xs, int ys);
void comerNormal (Estado *e, int x, int y, int xs, int ys);
void fimComer (Estado *e);
void jogarNormal (Estado *e, int x, int y, int z);
void cadeiaReis (Estado *e, int x, int y, int xs, int ys, char func);
void evoluir (Estado *e, int x, int y);
void marcarComidas (Estado *e, int x, int y, int c, int l);
void apagarComes (Estado *e, int x, int y);
void jogada (Estado *e, int x, int y, int c, int l);
int comem (Estado *e, int x, int y, char turno);
int verificarComer (Estado e);
Estado lerEstado (int *c, int *l, char *nome);
void apagarRosas (Estado *e, int r[], int p, char turno);
void guardarEstado (Estado *e, int c, int l);
void imprimirImagem (Estado e, int x, int y, Peca peca);
void desenharPeca (Estado e, int x, int y);
int main();

#endif