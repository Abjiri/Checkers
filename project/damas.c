#include "damas.h"

void fim (Estado *e) {
	int x, y, i = 0, j = 0;

	for (x = 0; x < 10; ++x) {
		for (y = 0; y < 10; ++y) {
			if (e->tab[x][y] == PRETO || e->tab[x][y] == PCOME || e->tab[x][y] == REI || e->tab[x][y] == KCOME) i++;
	}}
	for (x = 0; x < 10; ++x) {
		for (y = 0; y < 10; ++y) {
			if (e->tab[x][y] == BRANCO || e->tab[x][y] == BCOME || e->tab[x][y] == RAINHA || e->tab[x][y] == QCOME) j++;
	}}
	if (!i) e->fim = 'B';
	if (!j) e->fim = 'P';
}

Peca fimTurno (Estado e1, Estado e2) {
	int x, y;
	Peca peca = e1.turno, evol;

	for (x = 0; x < 10; ++x) {
		for (y = 0; y < 10; ++y) {
			if (e1.tab[x][y] == JOGAVEL || e1.tab[x][y] == COMER || e1.tab[x][y] == DEVORAR) return peca;
	}}

	if (peca == PRETO) evol = REI;
	else evol = RAINHA;
	
	for (x = 0; x < 10; ++x) {
		for (y = 0; y < 10; ++y) {
			if ((e1.tab[x][y] == peca && e2.tab[x][y] != peca) ||
				(e1.tab[x][y] == evol && e2.tab[x][y] != evol)) {
				if (peca == PRETO) return BRANCO;
				return PRETO;
	}}}
	return peca;
}

int pecasComer (Estado *e) {
	int x, y, r = 0;
	for (x = 0; x < 10; ++x) {
		for (y = 0; y < 10; ++y) {
			if (e->tab[x][y] == COMER && r < 1) r = 1;
			if (e->tab[x][y] == DEVORAR) r = 2;
	}}
	return r;
}

void regredirRosas (Estado *e, int x, int y) {
	if (e->tab[x][y] == BCOME) e->tab[x][y] = BRANCO;
	if (e->tab[x][y] == PCOME) e->tab[x][y] = PRETO;
	if (e->tab[x][y] == KCOME) e->tab[x][y] = REI;
	if (e->tab[x][y] == QCOME) e->tab[x][y] = RAINHA;
}

void apagarComidas (Estado *e) {
	int x, y;
	if (e->comidas && !pecasComer(e)) {
		for (x = 0; x < 10; ++x) {
			for (y = 0; y < 10; ++y) {
				if (e->tab[x][y] == COMIDA) e->tab[x][y] = ESCURO;
				regredirRosas(e,x,y);
		}}
		e->comidas = 0;
}}

void tirarAmarelos (Estado *e, int nr) {
	int x, y;
	for (x = 0; x < 10; ++x) {
		for (y = 0; y < 10; ++y) {
			if ((nr == 3 && (e->tab[x][y] == JOGAVEL || e->tab[x][y] == COMER || e->tab[x][y] == DEVORAR)) ||
				(nr == 2 && (e->tab[x][y] == JOGAVEL || e->tab[x][y] == COMER)) ||
				(nr == 1 && (e->tab[x][y] == JOGAVEL))) e->tab[x][y] = ESCURO;
}}}

Peca valorPeca (Estado *e, int x, int y, Peca oposto) {
	Peca tipo;
	if (e->tab[x][y] == oposto) tipo = COMER;
	else tipo = DEVORAR;
	return tipo; 
}

int limites (int x, int y, int xs, int ys) {
	int r = 0;
	if ((xs == 1 && ys == 1 && x < 8 && y < 8)  ||
		(xs == 1 && ys == -1 && x < 8 && y > 1) ||
		(xs == -1 && ys == 1 && x > 1 && y < 8) ||
		(xs == -1 && ys == -1 && x > 1 && y > 1)) r = 1;
	return r;
}

void comerNormal (Estado *e, int x, int y, int xs, int ys) {
	Peca peca = e->tab[x][y], oposto, evol;
	if (peca == PRETO || peca == PCOME) { oposto = BRANCO; evol = RAINHA; }
	else { oposto = PRETO; evol = REI; }

	if (limites(x,y,xs,ys) && (e->tab[x+xs][y+ys] == oposto || e->tab[x+xs][y+ys] == evol) && e->tab[x+xs*2][y+ys*2] == ESCURO)
		e->tab[x+xs*2][y+ys*2] = valorPeca(e,x+xs,y+ys,oposto);
}

void fimComer (Estado *e) {
	int r = pecasComer(e);
	if (r == 2) tirarAmarelos(e,2);
	else if (r) tirarAmarelos(e,1);
}

void jogarNormal (Estado *e, int x, int y, int z) {
	tirarAmarelos(e,3);
	if (x > 0 && e->tab[x-1][y+z] == ESCURO) e->tab[x-1][y+z] = JOGAVEL;
	if (x < 9 && e->tab[x+1][y+z] == ESCURO) e->tab[x+1][y+z] = JOGAVEL;
	comerNormal(e,x,y,1,1);
	comerNormal(e,x,y,1,-1);
	comerNormal(e,x,y,-1,1);
	comerNormal(e,x,y,-1,-1);
	fimComer(e);
}

void cadeiaReis (Estado *e, int x, int y, int xs, int ys, char func) {
	Peca peca = e->tab[x][y], oposto, evol, tipo; int r;
	if (peca == REI || peca == KCOME) { oposto = BRANCO; evol = RAINHA; }
	else { oposto = PRETO; evol = REI; }

	while (x+xs >= 0 && x+xs <= 9 && y+ys >= 0 && y+ys <= 9) {
		if (e->tab[x+xs][y+ys] == ESCURO) { 
			e->tab[x+xs][y+ys] = JOGAVEL;
			x += xs; y += ys;
		}
		else if (e->tab[x+xs][y+ys] == oposto || e->tab[x+xs][y+ys] == evol) {
			tipo = valorPeca(e,x+xs,y+ys,oposto);
			x += xs;
			y += ys;
			while (x+xs >= 0 && x+xs <= 9 && y+ys >= 0 && y+ys <= 9 && e->tab[x+xs][y+ys] == ESCURO) {
				e->tab[x+xs][y+ys] = tipo;
				x += xs;
				y += ys;
			}
			break;
		}
		else break;
	}

	r = pecasComer(e);
	if (!r && func == 'A');
	else if (r == 2) tirarAmarelos(e,2);
	else tirarAmarelos(e,1);
}

void evoluir (Estado *e, int x, int y) {
	if (e->tab[x][y] == BRANCO && y == 0 && !e->comidas) e->tab[x][y] = RAINHA;
	if (e->tab[x][y] == PRETO  && y == 9 && !e->comidas) e->tab[x][y] = REI;
}

void marcarComidas (Estado *e, int x, int y, int c, int l) {
	while (x != c) {
		if (e->tab[x][y] != ESCURO && e->tab[x][y] != JOGAVEL && e->tab[x][y] != COMER && e->tab[x][y] != DEVORAR) e->tab[x][y] = COMIDA;
		if (x < c && y < l) { x++; y++; }
		if (x > c && y < l) { x--; y++; }
		if (x < c && y > l) { x++; y--; }
		if (x > c && y > l) { x--; y--; }
}}

void apagarComes (Estado *e, int x, int y) {
	int c, l;
	for (c = 0; c < 10; ++c) {
		for (l = 0; l < 10; ++l) {
			if (c != x || l != y) regredirRosas(e,c,l);
}}}

void jogada (Estado *e, int x, int y, int c, int l) {
	Peca peca = e->tab[x][y];

	if (peca == PRETO || peca == PCOME) jogarNormal(e,x,y,1);

	else if (peca == BRANCO || peca == BCOME) jogarNormal(e,x,y,-1);

	else if (peca == REI || peca == KCOME || peca == RAINHA || peca == QCOME) {
		tirarAmarelos(e,3);
		cadeiaReis(e,x,y,1,1,'A');
		cadeiaReis(e,x,y,-1,1,'A');
		cadeiaReis(e,x,y,1,-1,'A');
		cadeiaReis(e,x,y,-1,-1,'A');
	}
	else if (peca == JOGAVEL) {
		e->tab[x][y] = e->tab[c][l];
		e->tab[c][l] = ESCURO;
		tirarAmarelos(e,3);
		evoluir(e,x,y);
	}
	else if (peca == COMER || peca == DEVORAR) {
		marcarComidas(e,x,y,c,l);
		e->tab[x][y] = e->tab[c][l];
		e->tab[c][l] = ESCURO;
		e->comidas++;
		tirarAmarelos(e,3);
		apagarComes(e,x,y);
		if (e->tab[x][y] == PCOME || e->tab[x][y] == BCOME) {
			comerNormal(e,x,y,1,1);
			comerNormal(e,x,y,1,-1);
			comerNormal(e,x,y,-1,1);
			comerNormal(e,x,y,-1,-1);
			fimComer(e);
		}
		else {
			cadeiaReis(e,x,y,1,1,'C');
			cadeiaReis(e,x,y,-1,1,'C');
			cadeiaReis(e,x,y,1,-1,'C');
			cadeiaReis(e,x,y,-1,-1,'C');
		}
		apagarComidas(e);
		evoluir(e,x,y);
}}

int comem (Estado *e, int x, int y, char turno) {
	Estado est = *e; int r, ret = 0;
	jogada(&est,x,y,0,0);
	r = pecasComer(&est);
	if (r) {
		if (turno == 'P' && e->tab[x][y] == PRETO) e->tab[x][y] = PCOME;
		else if (turno == 'P' && e->tab[x][y] == REI) e->tab[x][y] = KCOME;
		else if (turno == 'B' && e->tab[x][y] == BRANCO) e->tab[x][y] = BCOME;
		else e->tab[x][y] = QCOME;
	}
	return r;
}

int verificarComer (Estado e) {
	int x, y;
	for (x = 0; x < 10; ++x) {
		for (y = 0; y < 10; ++y) {
			if (e.tab[x][y] == PCOME || e.tab[x][y] == BCOME || e.tab[x][y] == KCOME || e.tab[x][y] == QCOME) return 1;
	}}
	return 0;
}

Estado lerEstado (int *c, int *l, char *nome) {
	int x, y, comidas; char peca, turno, dir[100]; Estado e;
	sprintf(dir,"%s%s.txt",dirbase,nome);
	FILE *fp = fopen(dir,"r");
	fscanf(fp, "%d %d %d %c \n", c, l, &comidas, &turno);
	for (y = 0; y < 10; ++y) {
		for (x = 0; x < 10; ++x) {
			fscanf(fp,"%c", &peca);
			switch (peca) {
				case 'C': e.tab[x][y] = CLARO;   break;
				case 'E': e.tab[x][y] = ESCURO;  break;
				case 'P': e.tab[x][y] = PRETO;   break;
				case 'B': e.tab[x][y] = BRANCO;  break;
				case 'J': e.tab[x][y] = JOGAVEL; break;
				case 'A': e.tab[x][y] = COMER;   break;
				case 'D': e.tab[x][y] = DEVORAR; break;
				case 'K': e.tab[x][y] = REI;     break;
				case 'Q': e.tab[x][y] = RAINHA;  break;
				case 'Z': e.tab[x][y] = COMIDA;  break;
				case 'M': e.tab[x][y] = PCOME;   break;
				case 'N': e.tab[x][y] = BCOME;	 break;
				case 'O': e.tab[x][y] = KCOME;   break;
				case 'U': e.tab[x][y] = QCOME;	 break;
		}}
		fscanf(fp,"\n");
	}
	if (turno == 'P') e.turno = PRETO;
	else e.turno = BRANCO;
	e.comidas = comidas;
	e.fim = 'A';
	return e;
}

void apagarRosas (Estado *e, int r[], int p, char turno) {
	int i, x, y, max = 0;
	for (i = 0; i < p; ++i)
		if (max < r[i]) max = r[i];

	i = 0;
	if (max == 2) {
		for (x = 0; x < 10; ++x) {
			for (y = 0; y < 10; ++y) {
				if ((turno == 'P' && (e->tab[x][y] == PRETO  || e->tab[x][y] == PCOME || e->tab[x][y] == REI || e->tab[x][y] == KCOME)) || 
					(turno == 'B' && (e->tab[x][y] == BRANCO || e->tab[x][y] == BCOME || e->tab[x][y] == RAINHA || e->tab[x][y] == QCOME))) {
					if (r[i] != 2) regredirRosas(e,x,y); ++i;
}}}}}

void guardarEstado (Estado *e, int c, int l) {
	int x, y, r[20], p = 0; char turno, dir[100];
	Estado est = lerEstado(&x,&y,"estado");
	if (c != 10) e->turno = fimTurno(*e,est);
	if (e->turno == PRETO) turno = 'P';
	else turno = 'B';

	if (!verificarComer(*e)) {
		for (x = 0; x < 10; ++x) {
			for (y = 0; y < 10; ++y) {
				if ((turno == 'P' && (e->tab[x][y] == PRETO  || e->tab[x][y] == REI)) || 
					(turno == 'B' && (e->tab[x][y] == BRANCO || e->tab[x][y] == RAINHA))) { r[p] = comem(e,x,y,turno); ++p;	}
	}}}
	apagarRosas(e,r,p,turno);

	sprintf (dir,"%sestado.txt",dirbase);
	FILE *fp = fopen(dir,"w");
	fprintf(fp, "%d %d %d %c\n", c, l, e->comidas, turno);
	for (y = 0; y < 10; ++y) {
		for (x = 0; x < 10; ++x) {
			switch (e->tab[x][y]) {
				case CLARO:   fprintf(fp, "C"); break;
				case ESCURO:  fprintf(fp, "E"); break;
				case PRETO:   fprintf(fp, "P"); break;
				case BRANCO:  fprintf(fp, "B"); break;
				case JOGAVEL: fprintf(fp, "J"); break;
				case COMER:   fprintf(fp, "A"); break;
				case DEVORAR: fprintf(fp, "D"); break;
				case REI:	  fprintf(fp, "K"); break;
				case RAINHA:  fprintf(fp, "Q"); break;
				case COMIDA:  fprintf(fp, "Z"); break;
				case PCOME:   fprintf(fp, "M"); break;
				case BCOME:   fprintf(fp, "N"); break;
				case KCOME:	  fprintf(fp, "O"); break;
				case QCOME:	  fprintf(fp, "U"); break;
		}}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void imprimirImagem (Estado e, int x, int y, Peca peca) {
	char link[100];
	sprintf(link,"%s?%d,%d",linkbase,x,y);
	ABRIR_LINK(link);
	IMAGEM(x,y,80,ficheiro[peca]);
	FECHAR_LINK;
}

void desenharPeca (Estado e, int x, int y) {
	Peca peca = e.tab[x][y], evol;

	if (e.turno == PRETO) evol = REI;
	else evol = RAINHA; 

	if ((verificarComer(e) && (peca == PCOME || peca == BCOME || peca == KCOME || peca == QCOME || peca == COMER || peca == DEVORAR)) ||
		(!verificarComer(e)) && (peca == e.turno || peca == evol || peca == JOGAVEL)) imprimirImagem(e,x,y,peca);
	else IMAGEM(x,y,80,ficheiro[peca]);
}

int main() { //o txt tem a posição da jogada anterior, a query_string tem a posição da jogada mais recente
	int x, y, l, c;
	char imagem[10], turno[15];
	Estado e = lerEstado(&c, &l,"estado"); //posição da jogada anterior
	if (strlen(getenv("QUERY_STRING"))) {
		sscanf(getenv("QUERY_STRING"), "%d,%d", &x, &y); //posição da jogada mais recente
		if (x == 10) e = lerEstado(&c,&l,"estadoInit");
		else {
			jogada(&e,x,y,c,l);
			fim(&e);
		}
		guardarEstado(&e,x,y);
	}
	
	COMECAR_HTML;
	ABRIR_SVG(1500,1000);

	if (e.fim == 'A') {
		for (x = 0; x < 10; x++) {
			for (y = 0; y < 10; y++)
				desenharPeca(e,x,y);
	}}
	else {
		sprintf(imagem, "win%c.png", e.fim);
		IMAGEM(0,0,800,imagem);
	}

	ABRIR_LINK("http://localhost/cgi-bin/Damas?10,10");
	IMAGEM(0,2,340,"novo.png");
	FECHAR_LINK;

	if (e.turno == PRETO) sprintf(turno, "turnop.png");
	else sprintf(turno, "turnob.png");
	IMAGEM(5,2,163,turno);
	
	FECHAR_SVG;
	FECHAR_HTML;
	return 0;
}