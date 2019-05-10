/* * * * * * * * * * * * * * * * * * *
	Aluno: Mateus Morishigue Borges
	NUSP: 9850328
* * * * * * * * * * * * * * * * * * */

#ifndef ATUALIZACAO_H
#define ATUALIZACAO_H

#include <stdio.h>	

void casoatnumeroinscricao(int rrn, char* valorCampo, FILE* arqbin);
void casoatnota(int rrn, char* valorCampo, FILE* arqbin);
void casoatdata(int rrn, char* valorCampo, FILE* arqbin);
void casoatcidade(int rrn, char* valorCampo, FILE* arqbin);
void casoatnomeescola(int rrn, char* valorCampo, FILE* arqbin);
int atualizacao(char* readFile, int n);

#endif