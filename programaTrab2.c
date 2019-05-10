/* * * * * * * * * * * * * * * * * * *
	Aluno: Mateus Morishigue Borges
	NUSP: 9850328
* * * * * * * * * * * * * * * * * * */

#include "remocao.h"
#include "insercao.h"
#include "atualizacao.h"
#include "recuperacao.h" 
#include "auxlib.h"

int main(){
	
	setlocale(LC_CTYPE,"Portuguese"); //para printar os acentos certos

	int func;			//funcionalidade
	char readFile[30];	//string com o arquivo a ser lido
	int n;				//numero de vezes que irá se repetir

	clearstring(readFile,30); //limpa a string readFile

	//pega a entrada
	char entrada[100];
	fgets(entrada,100,stdin);

	//funcao para gerar uma string readFile e um numero n, se tiver
	char *token = strtok(entrada, " ");
	for(int i = 0; i < 3; i++){
		if(i == 0 && token){
			func = strtol(token,NULL,10);
		}
		if(i == 1 && token){
			strcpy(readFile,token);
		}
		if(i == 2 && token){
			n = strtol(token,NULL,10);
		}
		token = strtok(NULL," ");
	}

	//verificar qual é a funcionalidade escolhida
	switch(func){
		//se for a funcionalidade 2
		case 2:
			recuperacao(readFile);
		break;
		//se for a funcionalidade 5
		case 5:
			remocao(readFile, n);
		break;
		//se for a funcionalidade 6
		case 6:
			insercao(readFile, n);
		break;
		//se for a funcionalidade 7
		case 7:
			atualizacao(readFile, n);
		break;
		//caso nao exista a funcionalidade ainda
		default:
			printf("Funcionalidade %d não implementada.\n",func);
			return ERR_NOTFUNC;
	}

	return 0;
}
