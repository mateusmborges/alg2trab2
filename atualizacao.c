/* * * * * * * * * * * * * * * * * * *
	Aluno: Mateus Morishigue Borges
	NUSP: 9850328
* * * * * * * * * * * * * * * * * * */

#include "atualizacao.h"
#include "auxlib.h"

void casoatnumeroinscricao(int rrn, char* valorCampo, FILE* arqbin){
	fseek(arqbin,16000 + (rrn*80), SEEK_SET);			//pula pro rrn
	fseek(arqbin,5,SEEK_CUR);							//pula 5 bytes
	int nilido = strtol(valorCampo,NULL,10);			//transforma a string em int
	fwrite(&nilido,sizeof(int),1,arqbin);				//escreve o novo inteiro
}

void casoatnota(int rrn, char* valorCampo, FILE* arqbin){
	fseek(arqbin,16000 + (rrn*80), SEEK_SET);			//pula pro rrn
	fseek(arqbin,9,SEEK_CUR);							//pula 9 bytes
	double notalida = strtod(valorCampo,NULL);				//transforma a string em double
	fwrite(&notalida,sizeof(double),1,arqbin);			//escreve o novo double
}

void casoatdata(int rrn, char* valorCampo, FILE* arqbin){
	char buffer[50]; //string para servir de buffer para o valorCampo
	clearstring(buffer,50);	//limpa a string buffer
	strcpy(buffer,valorCampo);	//passa o valorCampo para o buffer

	fseek(arqbin,16000 + (rrn*80), SEEK_SET);			//pula pro rrn
	fseek(arqbin,17,SEEK_CUR);							//pula 17 bytes
	//se for nulo, então coloca \0@@@@@@@@@
	if(ehnulo(valorCampo,50)){			
		fputc('\0',arqbin);
		for(int i = 0; i < 9; i++){
			fputs("@", arqbin);
		}
	}
	//se nao for nulo, então coloca a data certinho
	else{
		fwrite(&buffer,sizeof(char),10,arqbin);			//escreve a nova string
	}
}

void casoatcidade(int rrn, char* valorCampo, FILE* arqbin){
	/* 	DECLARAÇÃO DE VARIÁVEIS 
		i = variavel para laços for
		itcv1 = inteiro para armazenar o indicador de tamanho do campo variavel 1
		itcv2 = inteiro para armazenar o indicador de tamanho do campo variavel 2
		vclen = inteiro para armazenar o tamanho do valor do campo
		faltante = inteiro para armazenar quanto falta para o final do registro
		tag4 = char para armazenar a tag 4
		tag5 = char para armazenar a tag 5
		cvlido = string para armazenar o campo variavel lido
		buffer = string para servir de buffer para o valorCampo
	*/
	int i, itcv1, itcv2, vclen, faltante;
	char tag4, tag5;
	char cvlido[50];
	char buffer[50];

	clearstring(cvlido,50);	//limpa a string cvlido
	clearstring(buffer,50);	//limpa a string buffer

	strcpy(buffer,valorCampo);	//passa o valorCampo para o buffer

	//verifica se o valor do campo é nulo
	if(ehnulo(valorCampo,50)){
		vclen = 0;								//se for nulo, vclen recebe 0
	}
	else{
		vclen = strlen(valorCampo) + 2;			//se nao for nulo, vclen guarda o tamanho da string do valorCampo
	}

	fseek(arqbin,16000 + (rrn*80), SEEK_SET);	//pula pro rrn
	fseek(arqbin,31,SEEK_CUR);							//pula 31 bytes
	fread(&tag4,sizeof(char),1,arqbin);					//le o byte do campo tag4
	//se tiver um campo pra cidade
	if(tag4 == '4'){
		fseek(arqbin,-5,SEEK_CUR);						//volta 5 bytes
		fread(&itcv1,sizeof(int),1,arqbin);				//le o inteiro do indicador de tamanho do campo variavel 1 (cidade)
		fseek(arqbin,itcv1 + 4,SEEK_CUR);				//pula pra onde deveria estar o char '5'
		fread(&tag5,sizeof(char),1,arqbin);				//le o char
		//se tiver um campo nomeEscola
		if(tag5 == '5'){
			fseek(arqbin,-5,SEEK_CUR);					//volta 5 bytes
			fread(&itcv2,sizeof(int),1,arqbin);			//le o inteiro do indicador de tamanho do campo variavel 2 (nomeEscola)
			fseek(arqbin,1,SEEK_CUR);					//pula 1 byte
			fread(&cvlido,sizeof(char),itcv2-1,arqbin);	//le o campo variavel lido
			fseek(arqbin,-(itcv2 + 4 + itcv1 + 4),SEEK_CUR);//volta pra onde deveria estar o indicador de tamanho do campo variavel 1
			//se o campo for nulo
			if(vclen == 0){
				fwrite(&itcv2,sizeof(int),1,arqbin);		//escreve o indicador de tamanho do campo variavel 2
				fputc('5',arqbin);							//escreve o byte '5'
				fwrite(&cvlido,sizeof(char),itcv2-1,arqbin);//escreve o campo variavel lido
			}
			else{
				fwrite(&vclen,sizeof(int),1,arqbin);		//escreve o tamanho da string
				fputc('4',arqbin);							//escreve o byte '4'
				fwrite(&buffer,sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
				fwrite(&itcv2,sizeof(int),1,arqbin);		//escreve o indicador de tamanho do campo variavel 2
				fputc('5',arqbin);							//escreve o byte '5'
				fwrite(&cvlido,sizeof(char),itcv2-1,arqbin);//escreve o campo variavel lido		
			}
			//completa o registro com lixo
			for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
				fputs("@",arqbin);
			}
		}
		//se nao tiver um campo nomeEscola
		else{
			fseek(arqbin,-(5 + itcv1 + 4),SEEK_CUR);	//volta pra onde deveria estar o indicador de tamanho do campo variavel 1
			if(vclen != 0){
				fwrite(&vclen,sizeof(int),1,arqbin);		//escreve o tamanho da string
				fputc('4',arqbin);							//escreve o byte '4'
				fwrite(&buffer,sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada		
			}						
			//completa o registro com lixo
			for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
				fputs("@",arqbin);
			}
		}
	}
	//se nao tiver um campo pra cidade mas tiver pra nomeEscola
	else if(tag4 == '5'){
		fseek(arqbin,-5,SEEK_CUR);						//volta 5 bytes	
		fread(&itcv1,sizeof(int),1,arqbin);				//le o inteiro do indicador de tamanho do campo variavel 1 (cidade)
		fputc('5',arqbin);								//escreve o byte '5'
		fread(&cvlido,sizeof(char),itcv1-1,arqbin);		//le o campo variavel lido
		fseek(arqbin,-(itcv1 + 4),SEEK_CUR);			//volta pra onde deveria estar o indicador de tamanho do campo variavel 1
		fwrite(&vclen,sizeof(int),1,arqbin);			//escreve o tamanho da string
		fputc('4',arqbin);								//escreve o byte '4'
		fwrite(&buffer,sizeof(char),vclen-1,arqbin);	//escreve a string lida da entrada
		fwrite(&itcv1,sizeof(int),1,arqbin);			//escreve o indicador de tamanho do campo variavel 1
		fputc('5',arqbin);								//escreve o byte '5'
		fwrite(&cvlido,sizeof(char),itcv1-1,arqbin);	//escreve o campo variavel lido
		//completa o registro com lixo
		for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
			fputs("@",arqbin);
		}
	}
	//se nao tiver nenhum campo variavel
	else{
		fseek(arqbin,-5,SEEK_CUR);						//volta 5 bytes	
		fwrite(&vclen,sizeof(int),1,arqbin);			//escreve o tamanho da string
		fputc('4',arqbin);								//escreve o byte '4'
		fwrite(&buffer,sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
		//completa o registro com lixo
		for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
			fputs("@",arqbin);
		}
	}
	
	//Completa ate o final do registro com lixo
	faltante = ftell(arqbin) % 80;
	if(faltante != 0){
		faltante = 80 - faltante;
	}
	for(i = 0; i < faltante; i++){
		fputs("@",arqbin);
	}
}

void casoatnomeescola(int rrn, char* valorCampo, FILE* arqbin){
	/* 	DECLARAÇÃO DE VARIÁVEIS 
		i = variavel para laços for
		itcv1 = inteiro para armazenar o indicador de tamanho do campo variavel 1
		vclen = inteiro para armazenar o tamanho do valor do campo
		faltante = inteiro para armazenar quanto falta para o final do registro
		tag5 = char para armazenar a tag 5
		buffer = string para servir de buffer para o valorCampo
	*/
	int i, itcv1, vclen, faltante;
	char tag5;
	char buffer[50];
	
	clearstring(buffer,50);	//limpa a string buffer
	strcpy(buffer,valorCampo);	//passa o valorCampo para o buffer

	//verifica se o valor do campo é nulo
	if(ehnulo(valorCampo,50)){
		vclen = 0;		//se for nulo, vclen recebe 0
	}
	else{
		vclen = strlen(valorCampo) + 2;			//se nao for nulo, vclen guarda o tamanho da string do valorCampo
	}
	fseek(arqbin,16000 + (rrn*80), SEEK_SET);	//pula pro rrn
	fseek(arqbin,31,SEEK_CUR);							//pula 31 bytes
	fread(&tag5,sizeof(char),1,arqbin);					//le o char
	//se tiver um campo nomeEscola
	if(tag5 == '5'){
		fseek(arqbin,-5,SEEK_CUR);							//volta 5 bytes	
		if(vclen != 0){
			fwrite(&vclen,sizeof(int),1,arqbin);			//escreve o tamanho da string
			fputc('5',arqbin);								//escreve o byte '5'
			fwrite(&buffer,sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
		}
		//completa o registro com lixo
		for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
			fputs("@",arqbin);
		}	
	}
	//se tiver um campo cidade
	else if(tag5 == '4'){
		fseek(arqbin,-5,SEEK_CUR);						//volta 5 bytes	
		fread(&itcv1,sizeof(int),1,arqbin);				//le o inteiro do indicador de tamanho do campo variavel 1 (cidade)
		fseek(arqbin,itcv1,SEEK_CUR);					//pula os bytes do campo variavel
		if(vclen != 0){
			fwrite(&vclen,sizeof(int),1,arqbin);			//escreve o tamanho da string
			fputc('5',arqbin);								//escreve o byte '5'
			fwrite(&buffer,sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada	
		}
		//completa o registro com lixo
		for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
			fputs("@",arqbin);
		}
	}
	//se nao tiver nenhum campo variavel
	else{
		fseek(arqbin,-5,SEEK_CUR);							//volta 5 bytes	
		if(vclen != 0){
			fwrite(&vclen,sizeof(int),1,arqbin);			//escreve o tamanho da string
			fputc('5',arqbin);								//escreve o byte '5'
			fwrite(&buffer,sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
		}
		//completa o registro com lixo
		for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
			fputs("@",arqbin);
		}
	}
	
	//Completa ate o final do registro com lixo
	faltante = ftell(arqbin) % 80;
	if(faltante != 0){
		faltante = 80 - faltante;
	}
	for(i = 0; i < faltante; i++){
		fputs("@",arqbin);
	}
}

int atualizacao(char* readFile, int n){

	/* 	DECLARAÇÃO DE VARIÁVEIS 
		i = variavel para laços for
		atual = variavel do laço para ler todas as linhas de entrada
		rrn = vetor com os rrns lidos	
		topoPilha = inteiro para guardar o topo da pilha
		decodvalor = inteiro para decodificar o valor do campo para o switch
		status = char para guardar o status do arquivo
		regrem = char para ver se o registro é removido ou nao
		nomeCampo = strings para armazenarem os nomes dos campos
		valorCampo = strings para armazenarem os valores dos campos
	*/
	int i, atual, rrn[n], topoPilha, decodvalor;
	char status, regrem;
	char nomeCampo[n][50], valorCampo[n][50];	

	//limpar as strings abaixo
	for(i = 0; i < n; i++){
		clearstring(nomeCampo[i],50);
		clearstring(valorCampo[i],50);
	}

	trim(readFile);	//tira os \n e \r da string, se tiver

	//verifica se o arquivo readFile existe
	FILE *arqbin = fopen(readFile,"r+b");	//abre o arquivo readFile
	if(!arqbin){							//se nao conseguir abrir, retorna o erro
		printf("Falha no processamento do arquivo.");	
		return ERR_NOTFILE;
	}

	//verifica se o arquivo está consistente
	fread(&status,sizeof(char),1,arqbin);
	if(status != '1'){						//se o arquivo nao está consistente
		printf("Falha no processamento do arquivo.2");
		return ERR_NOTCONSIST;
	}

	//volta um byte para reescrever o status
	fseek(arqbin,-1,SEEK_CUR);
	//atualiza o status
	status = '0';
	fwrite(&status,sizeof(char),1,arqbin);

	//le o topo da pilha no cabeçalho
	fread(&topoPilha,sizeof(int),1,arqbin);
		
	/* Laço para passar lendo a entrada n vezes */
	for(atual = 0; atual < n; atual++){
		//ler a proxima entrada
		char entrada[100];
		clearstring(entrada,100);
		fgets(entrada,100,stdin);
		
		trim(entrada); 		//tira o \n e o \r da entrada

		//funcao para gerar uma string nomeCampo[atual] e uma string valorCampo[atual]
		char *token = strtok(entrada, " ");
		if(!token){
			return ERR_NOTTOKEN;
		}
		for(i = 0; i < 6; i++){
			if(i == 0 && token){
				rrn[atual] = strtol(token,NULL,10);
			}
			else if(i == 1 && token){
				strcpy(nomeCampo[atual],token);
			}	
			else if(i == 2 && token){
				strcpy(valorCampo[atual],token);
			}						
			else if(i > 2 && token){
				strcat(valorCampo[atual]," ");
				strcat(valorCampo[atual],token);
			}
			token = strtok(NULL," ");
		}

		//tira as aspas do valor do campo, se tiver
		if(valorCampo[atual][0] == '\"'){
			i = 1;
			while(valorCampo[atual][i] != '\"'){
				valorCampo[atual][i-1] = valorCampo[atual][i];
				i++;
			}
			valorCampo[atual][i-1] = '\0';
		}

		//checa se existe algum registro no rrn
		fseek(arqbin,16000 + (rrn[atual]*80), SEEK_SET);		//pula pro rrn
		fread(&regrem,sizeof(char),1,arqbin);					//le o byte do campo removido
		//se nao existir registro, seja por estar no final ou por estar removido
		if(feof(arqbin) || regrem == '*'){
			continue;
		}

		decodvalor = vctoi(nomeCampo[atual]);

		switch(decodvalor){
			//se o nomeCampo for nroInscricao
			case 1:
				casoatnumeroinscricao(rrn[atual], valorCampo[atual], arqbin);
			break;
			//se o nomeCampo for nota
			case 2:
				casoatnota(rrn[atual], valorCampo[atual], arqbin);
			break;
			//se o nomeCampo for data
			case 3:
				casoatdata(rrn[atual], valorCampo[atual], arqbin);
			break;
			//se o nomeCampo for cidade
			case 4:
				casoatcidade(rrn[atual], valorCampo[atual], arqbin);
			break;
			//se o nomeCampo for nomeEscola
			case 5:
				casoatnomeescola(rrn[atual], valorCampo[atual], arqbin);
			break;
			//se o nomeCampo nao for nenhum dos acima
			default:
				printf("Falha no processamento do arquivo.3\n");
				return ERR_NOTVALCMP;
		} //acaba o switch
	}	//acaba o for de atual a n
	
	//volta pro primeiro byte para atualizar o status
	fseek(arqbin,0,SEEK_SET);
	//atualiza o status para consistente
	status = '1';
	fwrite(&status,sizeof(char),1,arqbin);

	//printa o arquivo no stdout com a função feita pelo monitor
	binarioNaTela1(arqbin);		

	//fecha o arquivo de leitura/escrita
	fclose(arqbin);

	return 0;
}		
		