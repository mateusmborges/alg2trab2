/* * * * * * * * * * * * * * * * * * *
	Aluno: Mateus Morishigue Borges
	NUSP: 9850328
* * * * * * * * * * * * * * * * * * */

#ifndef REMOCAO_H
#define REMOCAO_H

#include <stdio.h>

void casonumeroinscricao(int alvo, FILE* arquivo);
void casonota(double alvo, FILE* arquivo);
void casodata(char* alvo, FILE* arquivo);
void casocidade(char* alvo, FILE* arquivo);
void casonomeescola(char* alvo, FILE* arquivo);
int remocao(char* readFile, int n);

#endif