/**********************************************************************************
 *                                  Rei da APS 4.0                                *                           
 * Implemantação de uma árvore B em disco, as operações de busca e inserção.      *
 * Cada nodo é armazando em um arquivo em disco, deve conter um identificador ID, *
 * a qtd de chaves as chaves propriamente ditas e o identificador para os filhos. * 
 **********************************************************************************/
#include <stdio.h> // entrada e saida
#include <stdlib.h> // alocacao e rand 
#include <string.h> // manipulação de cadeia de caracteres, verificar entradas
#include <time.h> // time
#define TAM 100

#define EXISTE 1
#define NAOEXISTE 0
#define TEMCONTEUDO 1
#define VAZIO 0
#define ENCONTRADA 1
#define NAOENCONTRADA 0

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
int R;
int D;
int precisaInserir;
int raizEsquerda, raizDireita;
int contLeituras, contEscritas;

// Ponteiro para arquivo global, um pouco perigoso porem se usado com  
// responsabilidade muito eficiente, sempre lembrar de dar fclose
FILE *pArquivo;
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

// Funcoes:

/*
  Possiveis retornos:    
  	0 = Se arquivo vazio ou nao existir
	1 = Caso contrario  
*/

// Retorna tamanho do arquivo
int verificaTamanho (const char *);
// Verifica se ja existe arquivo com o mesmo nome
int existeArquivo (const char *);

// Transforma de int para string, retorna a string propriamente dita 
char *intParaStr (int);
// Pega da memoria secundaria o conteudo de um nodo da arvore (arquivo)
// e salva o conteudo na struct nodo_td na memoria primaria 
void le (int);

// Salva na memoria secundaria (disco) nodo da arvore
// contido na struct nodo_td  
void escreve (int);

void split (int ID);

void busca(int, int);

int insere (int, int); 
void limpaBuffer (char);
void insereOrdenado (int);
static int buscaChave (int);
void ordenaNaoUsar ();
// void inverteTalvezNaoUsa (int ini, int fim);
void inverte ();
void ordenaNaoUsar ();

int main () {
	int chave, ID;
	char *comando = (char *) malloc (7 * sizeof (char));
	char c = 0;
	srand (time (NULL));

	printf ("Digite Grau Maximo: ");
	scanf ("%d", &D);
	printf ("Digite ID raiz: ");
	scanf ("%d", &ID);
	R = ID;

	char *nome = intParaStr (ID);
	int existe = existeArquivo (nome);

	if (existe) {
		printf ("Raiz em %s encontrada.\n", nome);
	}
	else {
		// printf ("NAO EXISTE");
		escreve (ID);
		printf ("Criada raiz vazia em %s.\n", nome);
	}
	
	printf ("\n");
	do {
		printf ("> ");
		
		limpaBuffer(c);	
		scanf ("%s", comando);
		
		if (strcmp (comando, "insere") == 0) {
			scanf ("%d", &chave);
			insere (R, chave);
			printf ("%d leitura(s), %d escrita(s)\n", contLeituras, contLeituras);
			contEscritas = 0;
			contLeituras = 0;
		}	
		if (strcmp (comando, "busca") == 0){
			scanf ("%d", &chave);
			busca (R, chave);
		}
	} while ( strcmp(comando, "fim") );

	fclose (pArquivo);
	return 0;
}

void ordenaNaoUsar () {
	int i, j, aux;
	for (i = 0; i < (D/2); i++) {
		for (j = 0; j < ((D/2) - i - 1); i++){
			if (nodo.chaves[i]>nodo.chaves[j+1]) {
				aux = nodo.chaves[j];
				nodo.chaves[j] = nodo.chaves[j+1];
				nodo.chaves[j+1] = aux;
			}
		}
	}
}

// void inverteTalvezNaoUsa (int ini, int fim) {
// 	if ((fim - ini) <= 0)
// 		return;
	
// 	int aux;
// 	aux = nodo.chaves[fim];
// 	nodo.chaves[fim] = nodo.chaves[ini];
// 	nodo.chaves[ini] = aux;
// 	return inverteTalvezNaoUsa ((ini+1), (fim-1));
// }

void inverte () {
	int i, j;
    i = 0;
    j = (D/2)+1;

    for(i = 0; i<(D/2); i++) {
        int aux;
        aux = nodo.chaves[j];
        nodo.chaves[j] = nodo.chaves[i];
        nodo.chaves[i] = aux;
    }
}

void split (int ID) {
	char *nome = intParaStr(ID);
	int novaID;

	if(ID == R) {
		if (D%2 != 0) {
			nodo.n = D/2;
			
			precisaInserir = nodo.chaves[nodo.n];
			escreve (ID);
			
			inverte ();
			ordenaNaoUsar ();

			raizEsquerda = ID;
			inverte ();
			
			novaID = rand()%10000;
			nome = intParaStr (novaID);
			pArquivo = fopen (nome, "w");
			escreve (novaID);
			fclose(pArquivo);
			raizDireita = novaID;
			
			novaID = rand()%10000;
			nome = intParaStr (novaID);
			pArquivo = fopen (nome, "w");
			nodo.n = 1;
			nodo.chaves[0] = precisaInserir;
			nodo.filhos[0] = raizEsquerda;
			nodo.filhos[1] = raizDireita;
			escreve (novaID);
			fclose (pArquivo);
			R = novaID;
			
			printf("-- Nova raiz com ID %d --\n", R);
			precisaInserir = -1;

		}
		else {
			nodo.n = ((D/2) - 1);
			precisaInserir = nodo.chaves[nodo.n];
            
            escreve(ID);

            nodo.n = D/2;
            inverte ();
            ordenaNaoUsar ();
            raizEsquerda = ID;

            novaID = rand()%10000;
            nome = intParaStr (novaID);
            pArquivo = fopen(nome, "w");
            escreve (novaID);
            fclose (pArquivo);
            raizDireita = novaID;

            novaID = rand()%10000;
            nome = intParaStr (ID);
            pArquivo = fopen(nome, "w");
            nodo.n = 1;
            nodo.chaves[0] = precisaInserir;
            nodo.filhos[0] = raizEsquerda;
            nodo.filhos[1] = raizDireita;
            escreve (novaID);
            fclose (pArquivo);
            R = novaID;
            printf("-- Nova raiz com ID %d --\n", novaID);
            precisaInserir = -1;
		}
	}
	else {

        if (D%2 != 0) {
            nodo.n = (D/2);
            
			precisaInserir = nodo.chaves[nodo.n];
            escreve(ID);

            inverte ();
            ordenaNaoUsar ();

            novaID = rand()%10000;
            nome = intParaStr (novaID);
            pArquivo = fopen(nome, "w");
            escreve(novaID);
            fclose(pArquivo);
            raizDireita = novaID;
        }
        else {
            precisaInserir = nodo.chaves[D/2];

            nodo.n = D/2;
            escreve (ID);
            raizDireita = ID;

            inverte ();
        	ordenaNaoUsar ();

            novaID = rand()%10000;
            nome = intParaStr (novaID);
            pArquivo = fopen(nome, "w");
            escreve (novaID);
            fclose (pArquivo);
            raizDireita = novaID;
        }
    }
}


int insere (int ID, int x) {
	// Base da recursão NODO ser folha
	le (ID);
	int i, j, aux;
	if (nodo.filhos[0] == -1) {	
		insereOrdenado (x);
		nodo.n++;
		nodo.filhos[nodo.n] = -1;
		if (nodo.n == D) {
			split (ID);
		}
		else {
			escreve (ID);	
			precisaInserir = -1;
		}	
	}

	if (precisaInserir != -1) {
		le (ID);
        insereOrdenado (precisaInserir);
        nodo.n++;
        nodo.filhos[nodo.n] = raizDireita;

        for(i = 0; i < nodo.n; i++) {
            if(nodo.chaves[i] == x) 
                break;
        }
        i++;

        for(j = nodo.n+1; j > i; j--) {
            if(i > 0) {
                aux = nodo.filhos[i-1];
                nodo.filhos[i-1] = nodo.filhos[i];
                nodo.filhos[i] = aux;
            }
        }

        if(nodo.n == D) {
            split(ID);
        }
        else {
            escreve(ID);
            precisaInserir = -1;
        }

	}
	
	if (x < nodo.chaves[0]) 
		return insere (nodo.filhos[0], x);

	else if (x > nodo.chaves[nodo.n-1])
		return insere (nodo.filhos[nodo.n], x); 
	
	else
		for (int i = 1; i < nodo.n; i++) {
			if ((x < nodo.chaves[i]) && (x > nodo.chaves[i-1]))   
				return insere (nodo.filhos[i], x); 
			break;
		}
}


void escreve (int ID) {
	contEscritas++;
	// Altera nome de int para string
	char *nome = intParaStr (ID);
	// Contador 
	int i;

	// Se tamanho nao existir
	if (!verificaTamanho (nome)) {
		// Abre arquivo para escrita 
		pArquivo = fopen (nome, "w");
		// Seta a qtd de chaves como ZERO
		nodo.n = 0;
		// E o valor do primeiro elemento do vetor filhos 
		// como -1, sabendo que tal vetor precisa ser n+1 
		fprintf (pArquivo, "%d\n", nodo.n);
		fprintf (pArquivo, "-1 ", nodo.filhos[0]);

		printf ("Escrito (");
		for (i = 0; i < nodo.n; i++)
			printf (" %d ", nodo.chaves[i]);
		printf (")\n");

		// fecha o arquivo
		fclose (pArquivo);
	}

	// Senao 
	else {
		// Abre o arquivo como leitura e escrita
		pArquivo = fopen (nome, "w");

		// Guarda no arquivo o valor da qtd
		fprintf (pArquivo, "%d\n", nodo.n);

		// Preenche o vetor de chaves 	
		for (i = 0; i < nodo.n; i++)
			fprintf (pArquivo, "%d ", nodo.chaves[i]);

		// \n para deixar o arquivo no formato desejado
		fprintf (pArquivo, "%c", '\n');

		// Preenche vetor de filhos, no caso com -1, pois não possi filhos
		for (i = 0; i < nodo.n+1; i++)
			fprintf (pArquivo, "%d ", nodo.filhos[i]);

		// Printa o que foi escrito em disco 
		printf ("Escrito (");
		for (i = 0; i < nodo.n; i++)
			printf (" %d ", nodo.chaves[i]);
		printf (")\n");	

		// Fecha arquivo
		fclose (pArquivo);	
	}
}

void le (int ID) {
	contLeituras++;
	char *nome = intParaStr (ID);

	// Abri arquivo em modo leitura
	pArquivo = fopen (nome, "r");

	// Le para memoria principal a quantidade
	fscanf (pArquivo, "%d", &nodo.n);

	int i;
	for (i = 0; i < nodo.n; i++)
		fscanf (pArquivo, "%d", &nodo.chaves[i]); // Le o vetor de chaves di arquivo na struct  

	for (i = 0; i < nodo.n + 1; i++)
		fscanf (pArquivo, "%d", &nodo.filhos[i]); // Le o vetor de filhos para a memoria principal 
	
	// Printa o vetor de chaves ja presentes na memoria principal  
	printf ("Lido (");
	for (i = 0; i < nodo.n; i++)
		printf (" %d ", nodo.chaves[i]);
	printf (")\n");

	// Desaloca o ponteiro de arquivo
	fclose (pArquivo);	
}

void insereOrdenado (int x) {
    int i = nodo.n;
	int aux;
	if (nodo.n == 0)
		nodo.chaves[0] = x;  
	
	else {
		nodo.chaves[nodo.n] = x;
		while (i > 0 && nodo.chaves[i] < nodo.chaves[i - 1]) {
			aux = nodo.chaves[i - 1];  
			nodo.chaves[i - 1] = nodo.chaves[i];
			nodo.chaves[i] = aux;  
			i--;  
		}  
	}
} 

static int buscaChave (int x) {
  int ini = 0;
  int fim = nodo.n;

  while (fim >= ini) {
    int meio = (fim + ini) / 2;
    if(nodo.chaves[meio] == x) 
		return ENCONTRADA;

    else if(nodo.chaves[meio] < x) 
		ini = meio + 1;

    else fim = meio - 1;
  }
  
  return NAOENCONTRADA;
}

// Busca chave na arvore 
void busca(int ID, int x) {
	// Le o nodo, passa a informação do disco para a estrutura na memoria primaria  
    le(ID);
	// Base da referenciase a chave esta na raiz ENCONTADA
	if (buscaChave(x)) 
		ENCONTRADA;		
	
	// Se nao esta...
    if (!buscaChave (x)){
		// Se chave buscada menor que 1º chave  
        if ((x < nodo.chaves[0]) || (nodo.filhos[0] == -1))
           return busca (nodo.filhos[0], x); // Retorna de forma recursiva a busca da chave no 1ª filho
		
		// Se Chave buscada maior que a ultima 
        else if ((x > nodo.chaves[nodo.n-1]) || (nodo.filhos[nodo.n] == -1))
            return busca (nodo.filhos[nodo.n], x); // Retorna de forma recursiva a busca da chave no ultimo filho
		
		// Senao chave buscada estara entre o inicio e o fim
        else {
			// Varre o vetor de chaves da RAIZ
            for (int i = 1; i < nodo.n; i++) {
                if ((x < nodo.chaves[i]) && (x > nodo.chaves[i-1])) // Caso esteja entre o ini e fim  
                    return busca (nodo.filhos[i], x); // Retorna de forma recursiva 
					break; // para o laço 
            }
		}
    }
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

		return TEMCONTEUDO;
	}
	return VAZIO;
}

int existeArquivo (const char *nome) {
	pArquivo = fopen (nome, "r");

	if (pArquivo) {
		fclose (pArquivo);
		return EXISTE;
	}
		return NAOEXISTE;
}