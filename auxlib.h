/* * * * * * * * * * * * * * * * * * *
	Aluno: Mateus Morishigue Borges
	NUSP: 9850328
* * * * * * * * * * * * * * * * * * */

#ifndef AUXLIB_H
#define AUXLIB_H

#define ERR_NOTFUNC 0
#define ERR_NOTFILE 0
#define ERR_NOTCONSIST 0
#define ERR_NOTVALCMP 0
#define ERR_NOTNUMBER 0
#define ERR_REMOC 0
#define ERR_NOTTOKEN 0
#define ERR_REGNOTREM 0
#define ERR_ARENTREG 0


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

void binarioNaTela(char *nomeArquivoBinario);
void binarioNaTela1(FILE *ponteiroArquivoBinario);
void trim(char *str);
int ehNumero(char* str);
int clearstring(char* str, int n);
int vctoi(char* nomeCampo);
int ehnulo(char* str, int strlen);

#endif


