/**********************************************************************************
 *                                  Rei da APS 4.0                                *                           
 * Implemantação de uma árvore B em disco, as operações de busca e inserção.      *
 * Cada nodo é armazando em um arquivo em disco, deve conter um identificador ID, *
 * a qtd de chaves as chaves propriamente ditas e o identificador para os filhos. * 
 **********************************************************************************/
#include <stdio.h> // entrada e saida
#include <stdlib.h> // alocacao e rand 
#include <string.h> // manipulação de cadeia de caracteres, verificar entradas
#define TAM 100

// estrutura que condiz a cada nodo da estrutura
typedef struct {
int n;           // numero de chaves no nodo
int chaves[TAM]; // vetor de chaves
int filhos[TAM]; // vetor de filhos
} nodo_td;

// instancia do Nodo_td, NUNCA vai haver mais do q 1 nodo na memoria principal
// portanto um nodo global evita que a estruta precise ser passada entre funcoes
// aumentando assim a legibilidade
nodo_td nodo;

// ponteiro para arquivo global, um pouco perigoso porem se usado com  
// responsabilidade muito eficiente, sempre lembrar de dar fclose
FILE *pArquivo;

// Funcoes

/*
  Possiveis retornos:    
  	0 = Se arquivo vazio ou nao existir
	1 = Caso contrario  
*/

// Retorna tamanho do arquivo
int verificaTamanho (const char *nome);
// Verifica se ja existe arquivo com o mesmo nome
int existeArquivo (const char *nome);

// Transforma de int para string, retorna a string propriamente dita 
char *intParaStr (int ID);

/* SOBRE O ARQUIVO GERADO */
/*
    A primeira linhas contem 1 valor:
    n = numeros de elementos

    O arquivo é seguido de 2 linhas e cada linha possui os
	as chaves n e os filhos n+1 

    EX:
    2
    13 17
    999 123
*/

// Pega da memoria secundaria o conteudo de um nodo da arvore (arquivo)
// e salva o conteudo na struct nodo_td na memoria primaria 
void le (int ID);

// Salva na memoria secundaria (disco) nodo da arvore
// contido na struct nodo_td  
void escreve (int ID);

void busca(int ID, int x);

void insere (int x); 
void limpaBuffer (char c);
void ordenaChaves (int v[], int n, int x);

int main () {
	int v[6] = {4, 6, 1, 2, 9};

	ordenaChaves (v,6,5);

	for (int i  = 0; i < 5; i++)
		printf ("%d ", v[i]);
	// // printf ("%d\n", nodo.n);
	// // for (int i = 0; i < 10; i++){
	// // 	printf ("%d ", nodo.filhos[i]);
	// // }

	// int n, chave, ID;
	// char *comando = (char *) malloc (7 * sizeof (char));
	// char c = 0;

	// // printf ("Digite Grau Maximo: ");
	// // scanf ("%d", &n);
	// printf ("Digite ID raiz: ");
	// scanf ("%d", &ID);
	
	// char *nome = intParaStr (ID);
	// int existe = existeArquivo (nome);

	// if (existe) {
	// 	printf ("Raiz em %s encontrada.\n", nome);
	// 	le (ID);
	// 	// escreve(ID);
	// }
	// else {
	// 	// printf ("NAO EXISTE");
	// 	escreve (ID);
	// 	printf ("Criada raiz vazia em %s.\n", nome);
	// }	

	// do {
	// 	printf ("\n> ");
	// 	// limpaBuffer(c);
		
	// 	scanf (" %s", comando);
	// 	// limpaBuffer(c);
		
	// 	if (strcmp (comando, "insere") == 0) {
	// 		scanf ("%d", &chave);
	// 		pArquivo = fopen (IDstr, "w+");
	// 		insere (chave);
	// 	}	
		
	// } while ( strcmp(comando, "fim") );
	fclose (pArquivo);
	return 0;
}

/* Apartir do algoritmo visto em aula temos que */
/*
	Busca(nodo R, chave x)
		se R contem x
			retorne "sim" // por busca binaria!
		Se x < primeira chave em R
			i = primeiro filho de R
		Se x > ultima chave em R
			i = ultimo filho de R
		Senao
			i = tal que chave[i] < x < chave[i-1]
		Retorne busca(r->filho[i],x)
*/
void busca(int ID, int x) {
	char *nome = intParaStr (ID);
	
}

void escreve (int ID) {
	// Altera nome de int para string
	char *nome = intParaStr (ID);
	// Contador 
	int i;	

	// Se tamanho nao existir
	int tam = verificaTamanho (nome);
	if (!tam) {
		// Abre arquivo para escrita 
		pArquivo = fopen (nome, "w");

		// Seta a qtd de chaves como ZERO
		nodo.n = 0;

		// E o valor do primeiro elemento do vetor filhos 
		// como -1, sabendo que tal vetor precisa ser n+1 
		fprintf (pArquivo, "-1 ", nodo.filhos[0]);

		// fecha o arquivo
		fclose (pArquivo);
	}

	// Senao 
	else {
		// Abre o arquivo como leitura e escrita
		pArquivo = fopen (nome, "r+");

		// Incrementa a qtd
		nodo.n++;

		// Guarda no arquivo o valor da qtd
		fprintf (pArquivo, "%d\n", nodo.n);

		// Preenche o vetor de chaves 
		for (i = 0; i < nodo.n; i++)
			fprintf (pArquivo, "%d ", nodo.chaves[i]);

		// \n para deixar o arquivo no formato desejado
		fprintf (pArquivo, "%c", '\n');

		// Preenche vetor de filhos, no caso com -1, pois não possi filhos
		for (i = 0; i < nodo.n+1; i++)
			fprintf (pArquivo, "-1 ", nodo.filhos[i]);

		// Printa o que foi escrito em disco 
		printf ("Escrito (");
		for (i = 0; i < nodo.n; i++)
			printf (" %d ", nodo.chaves[i]);
		printf (")\n");		

		// Fecha arquivo
		fclose (pArquivo);	
	}
}

// Le o arquivo e escreve o conteudo na estrutura, 
// tras o nodo para memoria principal
void le (int ID) {
	// printf ("to no le\n");
	// Converte o nome para string
	char *nome = intParaStr (ID);
	int i;

	// Abri arquivo em modo leitura
	pArquivo = fopen (nome, "r");

	// Escreve 
	fscanf (pArquivo, "%d", &nodo.n);

	for (i = 0; i < nodo.n; i++)
		fscanf (pArquivo, "%d", &nodo.chaves[i]);

	for (i = 0; i < nodo.n + 1; i++)
		fscanf (pArquivo, "%d", &nodo.filhos[i]);	

	escreve (ID);	

	printf ("Lido (");
	for (i = 0; i < nodo.n; i++)
		printf (" %d ", nodo.chaves[i]);
	printf (")\n");
			
	// printf ("%d\n", nodo.n);
	// for (int i = 0; i < 10; i++){
	// 	printf ("%d ", nodo.chaves[i]);
	// }
	// printf ("\n");
	// for (int i = 0; i < 10; i++){
	// 	printf ("%d ", nodo.filhos[i]);
	// }	

	fclose (pArquivo);	
}

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

void limpaBuffer(char c) {
	while((c = getchar()) != '\n' && c != EOF);
}

char *intParaStr (int ID) {
	char *IDstr = (char *) malloc (10 * sizeof (char));

	itoa (ID, IDstr, 10);
	strcat (IDstr,".dat");

	return IDstr;
}

int verificaTamanho (const char *nome) {
	pArquivo = fopen (nome, "r");

	if (pArquivo) {
		fseek (pArquivo, 0, SEEK_END);
		int tamanho = ftell (pArquivo);
		fclose (pArquivo);

		return 1;
	}

	return 0;
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

int existeArquivo (const char *nome) {
	pArquivo = fopen (nome, "r");

	if (pArquivo) {
		fclose (pArquivo);
		return 1;
	}
		return 0;
}