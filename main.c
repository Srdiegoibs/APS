**********************************************
* reiDaAps 4.0 implemantação de uma arvore B *
**********************************************
#include <stdio.h> // entrada e saida 
#include <stdlib.h> 


#define TAM 100

// estrutura da arvore
typedef struct 
{
int n; // numero de chaves no nodo
int chaves[TAM]; // vetor de chaves
int filhos[TAM]; // vetor de filhos
} nodo_td;

// instancia do Nodo, NUNCA vai haver mais do q 1 nodo na memoria principal
// portanto um controlador global evita que a estruta precise ser passada entre funcoes
// aumentando assim a legibilidade
nodo_td nodo;


int main()
{
	
	printf ("oi\n");
	
	return 0;
}