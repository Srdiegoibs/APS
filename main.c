/**********************************************************************************
 *                                  Rei da APS 4.0                                *                           
 * Implemantação de uma árvore B em disco, das operações de busca e inserção.     *
 * Cada nodo é armazando em um arquivo em disco, deve conter um identificador ID, *
 * a qtd de chaves as chaves propriamente ditas e o identificador para os filhos. * 
 **********************************************************************************/
#include <stdio.h> // entrada e saida
#include <stdlib.h> // alocacao e rand 
#include <string.h> // manipulação de cadeia de caracteres 
#define TAM 100

// estrutura nodo da arvore B
typedef struct {
int n;           // numero de chaves no nodo
int chaves[TAM]; // vetor de chaves
int filhos[TAM]; // vetor de filhos
} nodo_td;

// instancia do Nodo_td, NUNCA vai haver mais do q 1 nodo na memoria principal
// portanto um nodo global evita que a estruta precise ser passada entre funcoes
// aumentando assim a legibilidade
nodo_td nodo;

// ponteiro para arquivo 
FILE *pArquivo;

void criaNodo (char *IDstr);
void insere (int x); 
void busca(int x);
void le (int ID);
void escreve (int ID);
void limpaBuffer (char c);
void ordenaChaves (int arr[], int n, int x);

int main () {
	
	// int x;
	// 	scanf("%d", &x);

	// for (int i = 0; i < 5; i++)
	// 	nodo.chaves[i] = x;

	// for (int i = 0; i < 5; i++)
	// 	printf("%d", nodo.chaves[i]);

	// ordenaChaves (nodo.chaves, 5, x);
	
	// for (int i = 0; i < 5; i++)
	// 	 nodo.chaves[i] = x;



	int n;
	int chave;
	char *IDstr = (char *) malloc (10 * sizeof (char));
	char *comando = (char *) malloc (7 * sizeof (char));
	char c = 0;
	
	// printf ("Digite Grau Maximo: ");
	// scanf ("%d", &n);
	printf ("Digite ID raiz: ");
	scanf ("%s", IDstr);
	
	criaNodo (IDstr);	

	do {
		printf ("\n> ");
		// limpaBuffer(c);
		
		scanf (" %s", comando);
		// limpaBuffer(c);
		
		if (strcmp (comando, "insere") == 0) {
			scanf ("%d", &chave);
			pArquivo = fopen (IDstr, "w+");
			insere (chave);
		}	
		
	} while ( strcmp(comando, "fim") );

	return 0;
}

void criaNodo (char *IDstr) {
	strcat(IDstr,".dat");
	pArquivo = fopen(IDstr, "wt");
	// printf ("Escrito()\n");
	printf ("Criada raiz vazia em %s.\n", IDstr);
	fclose (pArquivo);
}

void ordenaChaves (int v[], int n, int x) {  
    int i, j;  
    for (i = 1; i < n; i++) {  
        x = v[i];  
        j = i - 1;  
        while (j >= 0 && v[j] > x) {  
            v[j + 1] = v[j];  
            j = j - 1;  
        }  
        v[j + 1] = x;  
    }  
}

// insere chave(conteudo) na arvore
void insere (int x) {
	ordenaChaves (nodo.chaves, nodo.n, x);
	nodo.n++;
	for (int i = 0; i < nodo.n; i++) {
		fprintf (pArquivo, "%d", &nodo.chaves[i]);
	}

	for(int i = 0; i < TAM; i++)
		printf("%d", nodo.chaves[i]);

	fclose (pArquivo);	
}

// busca chave(conteudo) na arvore 
void busca(int x) {	
}

void le (int ID) {
}

void escreve (int ID) {
}

void limpaBuffer(char c) {
	while((c = getchar()) != '\n' && c != EOF);
}