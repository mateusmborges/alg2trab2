/* * * * * * * * * * * * * * * * * * *
	Aluno: Mateus Morishigue Borges
	NUSP: 9850328
* * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

void binarioNaTela1(FILE *ponteiroArquivoBinario) {
	/* Escolha essa função se você ainda tem o ponteiro de arquivo 'FILE *' aberto.
	*  Lembrando que você tem que ter aberto ele no fopen para leitura também pra funcionar (exemplo: rb, rb+, wb+, ...) */

	size_t fl;
	char *mb;
	fseek(ponteiroArquivoBinario, 0, SEEK_END);
	fl = ftell(ponteiroArquivoBinario);
	fseek(ponteiroArquivoBinario, 0, SEEK_SET);
	mb = (char *) malloc(fl);
	fread(mb, 1, fl, ponteiroArquivoBinario);
	fwrite(mb, 1, fl, stdout);
	free(mb);
}

//funcao q verifica se uma string contem apenas digitos
int ehNumero(char* string){
	if(string[0] == '\0'){
		return 0;
	}
	int i = 0;
	while(string[i] != '\0'){
		if(isdigit(string[i]) || string[i] == '.'){
			i++;
		}
		else{
			return 0;
		}
	}
	return 1;
}

int main(){

	//para printar os acentos certos
	setlocale(LC_CTYPE,"Portuguese");

	//pega a entrada
	char entrada[100];
	fgets(entrada,100,stdin);
	//decodificação da entrada
	char func = entrada[0];
	char readFile[50], nc[5];
	int i, j, n, atual, topoPilha;
	const int menosum = -1;
	//limpar as strings acima
	for(i = 0; i < 30; i++){
		if(i < 5){
			nc[i] = '\0';
		}
		readFile[i] = '\0';
	}

	//Funcionalidade 5 - remoção
	if(func == '5'){
		//funcao para gerar uma string readFile e uma string nc
		char *token = strtok(entrada, " ");
		for(i = 0; i < 3; i++){
			if(i == 1 && token){
				strcpy(readFile,token);
			}
			else if(i == 2 && token){
				strcpy(nc,token);
			}
			token = strtok(NULL," ");
		}
		//verifica se nao tem nenhum \n nas strings
		for(i = 0; i < 30; i++){
			if(i < 5){
				if(nc[i] == '\n'){
					nc[i] = '\0';
				}
			}
			if(readFile[i] == '\n'){
				readFile[i] = '\0';
			}
		}
		//verifica se o arquivo readFile existe
		FILE *arqbin = fopen(readFile,"r+b");	//abre o arquivo readFile
		if(!arqbin){							//se nao conseguir abrir, retorna o erro
			printf("Falha no processamento do arquivo.");	
			return 0;
		}
		//verifica se o arquivo está consistente
		char status;
		fread(&status,sizeof(char),1,arqbin);
		if(status != '1'){
			printf("Falha no processamento do arquivo");
			return 0;
		}
		//le o topo da pilha no cabeçalho
		fread(&topoPilha,sizeof(int),1,arqbin);
		//verifica se nc é um numero valido
		if(ehNumero(nc)){			//se for um numero valido
			n = strtol(nc,NULL,10);	//transforma a string nc em um int n
		}
		else{						//se nao for um numero valido, dá erro
			printf("Falha no processamento do arquivo.");
			return 0;
		}
		//cria as variaveis para leitura dos nomes dos campos e valores
		char nomeCampo[n][50], valorCampo[n][50];
		int vci; //valor do campo inteiro
		double vcf; //valor do campo double
		int encont; //cria uma variavel pra ver se algum registro foi encontrado
		//limpa todas as matrizes
		for(i = 0; i < n; i++){
			for(j = 0; j < 50; j++){
				nomeCampo[i][j] = '\0';
				valorCampo[i][j] = '\0';
			}
		}	
		//laço para passar lendo a entrada n vezes
		for(atual = 0; atual < n; atual++){
			//limpa a entrada
			for(i = 0; i < 100; i++){
				entrada[i] = '\0';
			}
			//le a proxima entrada
			fgets(entrada,100,stdin);
			//tira o \n da entrada	
			for(i = 0; i < 100; i++){
				if(entrada[i] == '\n'){
					entrada[i] = '\0';
				}
			}
			//funcao para gerar uma string nomeCampo[atual] e uma string valorCampo[atual]
			char *token = strtok(entrada, " ");
			if(!token){
				return 0;
			}
			for(i = 0; i < 6; i++){
				if(i == 0 && token){
					strcpy(nomeCampo[atual],token);
				}
				else if(i == 1 && token){
					strcpy(valorCampo[atual],token);
				}						
				else if(i > 1 && token){
					strcat(valorCampo[atual]," ");
					strcat(valorCampo[atual],token);
				}
				token = strtok(NULL," ");
			}	
			//verifica se nao tem nenhum \n nas strings
			for(i = 0; i < 50; i++){
				if(nomeCampo[atual][i] == '\n'){
					nomeCampo[atual][i] = '\0';
				}
				if(valorCampo[atual][i] == '\n'){
					valorCampo[atual][i] = '\0';
				}
			}
			//o if abaixo serve para tirar as aspas se a entrada em valorCampo for uma string
			if(valorCampo[atual][0] == '\"'){
				i = 1;
				while(valorCampo[atual][i] != '\"'){
					valorCampo[atual][i-1] = valorCampo[atual][i];
					i++;
				}
				valorCampo[atual][i-1] = '\0';
			}
			//verifica se o campo da entrada é nroInscricao
			if(strcmp(nomeCampo[atual],"nroInscricao") == 0){
				//verifica se o valor do campo é um numero
				if(ehNumero(valorCampo[atual])){
					vci = strtol(valorCampo[atual],NULL,10);
				}
				//se o valor nao for um numero, dá erro
				else{
					printf("Falha no processamento do arquivo.");
					return 0;
				}
				//vai pro começo dos registros
				fseek(arqbin,16000,SEEK_SET);
				//cria uma variavel int para ler o nroInscricao do registro atual
				int nira;
				//cria uma variavel char para ler se o registro está removido ou nao
				char regrem;
				//zera a qtd de registros encontrados
				encont = 0;
				//passa o arquivo inteiro até acabar
				while(arqbin){
					fseek(arqbin,5,SEEK_CUR); //pula 5 bytes para ler o nroInscricao do registro
					fread(&nira,sizeof(int),1,arqbin);		
					//verifica se o numero da entrada é igual ao numero de inscricao do registro atual
					if(vci == nira){
						fseek(arqbin,-9,SEEK_CUR); 				//volta pro começo do registro
						fread(&regrem,sizeof(char),1,arqbin); 	//le o byte do campo removido
						//se o byte for '-' é pq o registro nao foi removido ainda
						if(regrem == '-'){
							encont++;
							fseek(arqbin,-1,SEEK_CUR);
							fputc('*',arqbin); //atualiza o campo para removido
							fwrite(&topoPilha,sizeof(int),1,arqbin); //atualiza o topo da pilha
							//transforma todos os outros campos do registro em lixo
							for(i = 0; i < 75; i++){
								fputc('@',arqbin);
							}
							topoPilha = (ftell(arqbin) - 16000 - 80)/80;	//atualiza o topoPilha com o RRN do registro atual
							//atualiza o topoPilha no cabeçalho
							fseek(arqbin,1,SEEK_SET);
							fwrite(&topoPilha,sizeof(int),1,arqbin);
							break;
						}
						//se o registro já estiver removido
						else{
							fseek(arqbin,79,SEEK_CUR);
						}
					}
					//se o numero de entrada nao for igual ao numero de insc do registro atual
					else{
						fseek(arqbin,71,SEEK_CUR);
					}
					//char para verificar se nao acabou o arquivo
					char endoffile;
					fread(&endoffile,sizeof(char),1,arqbin);
					//verifica se nao acabou o arquivo
					if(feof(arqbin)){
						if(encont == 0){	//se nao encontrou nenhum registro, retorna inexistente
							printf("Registro inexistente.");
						}
						break;
					}
					//se nao acabou, volta 1 byte
					else{
						fseek(arqbin,-1,SEEK_CUR);
					}
				} //fim do while arqbin
			}
			//verifica se o campo da entrada é nota
			else if(strcmp(nomeCampo[atual],"nota") == 0){
				//verifica se o valor do campo é um numero
				if(ehNumero(valorCampo[atual])){
					vcf = strtod(valorCampo[atual],NULL);
				}
				//se o valor nao for um numero, dá erro
				else{
					printf("Falha no processamento do arquivo.");
					return 0;
				}
				//vai pro começo dos registros
				fseek(arqbin,16000,SEEK_SET);
				//cria uma variavel double para ler a nota do registro atual
				double nora;
				//cria uma variavel char para ler se o registro está removido ou nao
				char regrem;
				//zera a qtd de registros encontrados
				encont = 0;
				//passa o arquivo inteiro até acabar
				while(arqbin){
					fseek(arqbin,9,SEEK_CUR); //pula 9 bytes para ler a nota do registro atual
					fread(&nora,sizeof(double),1,arqbin);
					//verifica se o numero da entrada é igual a nota do registro atual
					if(vcf == nora){
						fseek(arqbin,-17,SEEK_CUR); 			//volta pro começo do registro
						fread(&regrem,sizeof(char),1,arqbin); 	//le o byte do campo removido
						//se o byte for '-' é pq o registro nao foi removido ainda
						if(regrem == '-'){
							encont++;
							fseek(arqbin,-1,SEEK_CUR);
							fputc('*',arqbin); //atualiza o campo para removido
							fwrite(&topoPilha,sizeof(int),1,arqbin); //atualiza o topo da pilha
							//transforma todos os outros campos do registro em lixo
							for(i = 0; i < 75; i++){
								fputc('@',arqbin);
							}
							topoPilha = (ftell(arqbin) - 16000 - 80)/80;	//atualiza o topoPilha com o RRN do registro atual
						}
						//se o registro já estiver removido
						else{
							fseek(arqbin,79,SEEK_CUR);
						}
					}
					//se o numero de entrada nao for igual a nota do registro atual
					else{
						fseek(arqbin,63,SEEK_CUR);
					}
					//char para verificar se nao acabou o arquivo
					char endoffile;
					fread(&endoffile,sizeof(char),1,arqbin);
					//verifica se nao acabou o arquivo
					if(feof(arqbin)){
						if(encont == 0){	//se nao encontrou nenhum registro, retorna inexistente
							printf("Registro inexistente.");
						}
						break;
					}
					//se nao acabou, volta 1 byte
					else{
						fseek(arqbin,-1,SEEK_CUR);
					}
				} //fim do while arqbin
				//atualiza o topoPilha no cabeçalho
				fseek(arqbin,1,SEEK_SET);
				fwrite(&topoPilha,sizeof(int),1,arqbin);			
			}
			//verifica se o campo da entrada é data
			else if(strcmp(nomeCampo[atual],"data") == 0){
				//vai pro começo dos registros
				fseek(arqbin,16000,SEEK_SET);
				//cria uma variavel string para ler a data do registro atual
				char dra[11];
				dra[10] = '\0'; //coloca o ultimo byte como '\0'
				//cria uma variavel char para ler se o registro está removido ou nao
				char regrem;
				//zera a qtd de registros encontrados
				encont = 0;
				//passa o arquivo inteiro até acabar
				while(arqbin){
					fseek(arqbin,17,SEEK_CUR); //pula 17 bytes para ler a data do registro atual
					fread(&dra,sizeof(char),10,arqbin);
					//verifica se a string da entrada é igual a data do registro atual
					if(strcmp(valorCampo[atual],dra) == 0){
						fseek(arqbin,-27,SEEK_CUR); 			//volta pro começo do registro
						fread(&regrem,sizeof(char),1,arqbin); 	//le o byte do campo removido
						//se o byte for '-' é pq o registro nao foi removido ainda
						if(regrem == '-'){
							encont++;
							fseek(arqbin,-1,SEEK_CUR);
							fputc('*',arqbin); //atualiza o campo para removido
							fwrite(&topoPilha,sizeof(int),1,arqbin); //atualiza o topo da pilha
							//transforma todos os outros campos do registro em lixo
							for(i = 0; i < 75; i++){
								fputc('@',arqbin);
							}
							topoPilha = (ftell(arqbin) - 16000 - 80)/80;	//atualiza o topoPilha com o RRN do registro atual
						}
						//se o registro já estiver removido
						else{
							fseek(arqbin,79,SEEK_CUR);
						}
					}
					//se a string de entraad nao for igual a data do registro atual
					else{
						fseek(arqbin,53,SEEK_CUR);
					}
					//char para verificar se nao acabou o arquivo
					char endoffile;
					fread(&endoffile,sizeof(char),1,arqbin);
					//verifica se nao acabou o arquivo
					if(feof(arqbin)){
						if(encont == 0){	//se nao encontrou nenhum registro, retorna inexistente
							printf("Registro inexistente.");
						}
						break;
					}
					//se nao acabou, volta 1 byte
					else{
						fseek(arqbin,-1,SEEK_CUR);
					}
				} //fim do while arqbin
				//atualiza o topoPilha no cabeçalho
				fseek(arqbin,1,SEEK_SET);
				fwrite(&topoPilha,sizeof(int),1,arqbin);
			}
			//verifica se o campo da entrada é cidade
			else if(strcmp(nomeCampo[atual],"cidade") == 0){
				//vai pro começo dos registros
				fseek(arqbin,16000,SEEK_SET);
				//cria uma variavel string para ler a cidade do registro atual
				char cra[30];
				cra[29] = '\0'; //coloca o ultimo byte como '\0'
				//cria uma variavel char para ler se o registro está removido ou nao
				char regrem;
				//cria uma variavel para comparar com a tag4
				char tag4;
				//cria uma variavel para armazenar o indicador de tamanho do campo variavel
				int itcv;
				//zera a qtd de registros encontrados
				encont = 0;
				//passa o arquivo inteiro até acabar
				while(arqbin){
					fseek(arqbin,31,SEEK_CUR); //pula 31 bytes para ler a tag do primeiro campo variavel do registro atual
					fread(&tag4,sizeof(char),1,arqbin); //le o que era pra ser a tag da cidade
					//se o char lido corresponder a tag da cidade ('4')
					if(tag4 == '4'){
						fseek(arqbin,-5,SEEK_CUR); 			//volta 5 bytes para ler o indicador de tamanho do campo variavel
						fread(&itcv,sizeof(int),1,arqbin);	//le os 4 bytes seguintes e armazena o indicador de tamanho do campo variavel
						fread(&tag4,sizeof(char),1,arqbin);	//le o proximo byte e armazena em tag4
						fread(&cra,sizeof(char),itcv-1,arqbin);//le uma string do campo variavel
						//verifica se a string da entrada é igual a cidade do registro atual
						if(strcmp(valorCampo[atual],cra) == 0){
							fseek(arqbin,-(31 + itcv),SEEK_CUR); 	//volta pro começo do registro
							fread(&regrem,sizeof(char),1,arqbin); 	//le o byte do campo removido
							//se o byte for '-' é pq o registro nao foi removido ainda
							if(regrem == '-'){
								encont++;
								fseek(arqbin,-1,SEEK_CUR);
								fputc('*',arqbin); //atualiza o campo para removido
								fwrite(&topoPilha,sizeof(int),1,arqbin); //atualiza o topo da pilha
								//transforma todos os outros campos do registro em lixo
								for(i = 0; i < 75; i++){
									fputc('@',arqbin);
								}
								topoPilha = (ftell(arqbin) - 16000 - 80)/80;	//atualiza o topoPilha com o RRN do registro atual
							}
							//se o registro já estiver removido
							else{
								fseek(arqbin,79,SEEK_CUR);
							}
						}
						//se a string de entrada nao for igual a cidade do registro atual
						else{
							fseek(arqbin,80-(31+itcv),SEEK_CUR);
						}
					}
					//se o char lido nao for '4'
					else{
						fseek(arqbin,48,SEEK_CUR);
					}				
					//char para verificar se nao acabou o arquivo
					char endoffile;
					fread(&endoffile,sizeof(char),1,arqbin);
					//verifica se nao acabou o arquivo
					if(feof(arqbin)){
						if(encont == 0){	//se nao encontrou nenhum registro, retorna inexistente
							printf("Registro inexistente.");
						}
						break;
					}
					//se nao acabou, volta 1 byte
					else{
						fseek(arqbin,-1,SEEK_CUR);
					}
				} //fim do while arqbin
				//atualiza o topoPilha no cabeçalho
				fseek(arqbin,1,SEEK_SET);
				fwrite(&topoPilha,sizeof(int),1,arqbin);
			}
			//verifica se o campo da entrada é nomeEscola
			else if(strcmp(nomeCampo[atual],"nomeEscola") == 0){
				//vai pro começo dos registros
				fseek(arqbin,16000,SEEK_SET);
				//cria uma variavel string para ler o nome da escola do registro atual
				char nera[30];
				nera[29] = '\0'; //coloca o ultimo byte como '\0'
				//cria uma variavel string para ler a cidade do registro atual (se tiver que ler a cidade)
				char cra[30];
				cra[29] = '\0'; //coloca o ultimo byte como '\0'
				//cria uma variavel char para ler se o registro está removido ou nao
				char regrem;
				//cria duas variaveis para comparar com a tag4 e com a tag5
				char tag4, tag5;
				//cria duas variaveis para armazenar os indicadores de tamanho dos campos variaveis
				int itcv1, itcv2;
				//zera a qtd de registros encontrados
				encont = 0;
				//passa o arquivo inteiro até acabar
				while(arqbin){
					fseek(arqbin,31,SEEK_CUR); //pula 31 bytes para ler a tag do primeiro campo variavel do registro atual
					fread(&tag5,sizeof(char),1,arqbin); //le o que era pra ser a tag da cidade
					//se o char lido corresponder a tag do nomeEscola ('5')
					if(tag5 == '5'){
						fseek(arqbin,-5,SEEK_CUR); 			//volta 5 bytes para ler o indicador de tamanho do campo variavel
						fread(&itcv1,sizeof(int),1,arqbin);	//le os 4 bytes seguintes e armazena o indicador de tamanho do campo variavel
						fread(&tag5,sizeof(char),1,arqbin);	//le o proximo byte e armazena em tag5
						fread(&nera,sizeof(char),itcv1-1,arqbin);//le uma string do campo variavel					
						//verifica se a string da entrada é igual ao nome de escola do registro atual
						if(strcmp(valorCampo[atual],nera) == 0){
							fseek(arqbin,-(31 + itcv1),SEEK_CUR); 	//volta pro começo do registro
							fread(&regrem,sizeof(char),1,arqbin); 	//le o byte do campo removido
							//se o byte for '-' é pq o registro nao foi removido ainda
							if(regrem == '-'){
								encont++;
								fseek(arqbin,-1,SEEK_CUR);
								fputc('*',arqbin); //atualiza o campo para removido
								fwrite(&topoPilha,sizeof(int),1,arqbin); //atualiza o topo da pilha
								//transforma todos os outros campos do registro em lixo
								for(i = 0; i < 75; i++){
									fputc('@',arqbin);
								}
								topoPilha = (ftell(arqbin) - 16000 - 80)/80;	//atualiza o topoPilha com o RRN do registro atual
							}
							//se o registro já estiver removido
							else{
								fseek(arqbin,79,SEEK_CUR);
							}
						}
						//se a string de entrada nao for igual ao nome da escola do registro atual
						else{
							fseek(arqbin,80-(itcv1 + 31),SEEK_CUR);
						}
					}
					//se o char lido corresponder a tag da cidade ('4')
					else if(tag5 == '4'){
						fseek(arqbin,-5,SEEK_CUR); 			//volta 5 bytes para ler o indicador de tamanho do campo variavel
						fread(&itcv1,sizeof(int),1,arqbin);	//le os 4 bytes seguintes e armazena o indicador de tamanho do campo variavel
						fread(&tag4,sizeof(char),1,arqbin);	//le o proximo byte e armazena em tag4
						fread(&cra,sizeof(char),itcv1-1,arqbin);//le uma string do campo variavel
						fseek(arqbin,4,SEEK_CUR);			//pula 5 bytes
						fread(&tag5,sizeof(char),1,arqbin);	//le o byte e coloca em tag5
						//verifica se o byte lido é a tag do campo nomeEscola
						if(tag5 == '5'){
							fseek(arqbin,-5,SEEK_CUR);			//volta 5 bytes para ler o indicador de tamanho do campo variavel
							fread(&itcv2,sizeof(int),1,arqbin);	//le os 4 bytes seguintes e armazena o indicador de tamanho do campo variavel
							fread(&tag5,sizeof(char),1,arqbin);	//le o proximo byte e armazena em tag5
							fread(&nera,sizeof(char),itcv2-1,arqbin);//le uma string do campo variavel
							//verifica se a string da entrada é igual ao nome de escola do registro atual
							if(strcmp(valorCampo[atual],nera) == 0){
								fseek(arqbin,-(itcv2 + 4 + itcv1 + 31),SEEK_CUR);	//volta pro começo do registro 
								fread(&regrem,sizeof(char),1,arqbin);				//le o byte do campo removido
								//se o byte for '-' é pq o registro nao foi removido ainda
								if(regrem == '-'){
									encont++;
									fseek(arqbin,-1,SEEK_CUR);
									fputc('*',arqbin); //atualiza o campo para removido
									fwrite(&topoPilha,sizeof(int),1,arqbin); //atualiza o topo da pilha
									//transforma todos os outros campos do registro em lixo
									for(i = 0; i < 75; i++){
										fputc('@',arqbin);
									}
									topoPilha = (ftell(arqbin) - 16000 - 80)/80;	//atualiza o topoPilha com o RRN do registro atual
								}
								//se o registro já estiver removido
								else{
									fseek(arqbin,79,SEEK_CUR);
								}	
							}
							//se a string de entrada nao for igual ao nome da escola do registro atual
							else{
								fseek(arqbin,80-(itcv2 + 4 + itcv1 + 31),SEEK_CUR);
							}
						}
						//se nao for a tag do campo nomeEscola
						else{
							fseek(arqbin,80-(itcv1 + 5 + 31),SEEK_CUR);
						}
					}
					//se o char lido nao for nem '4' nem '5'	
					else{
						fseek(arqbin,48,SEEK_CUR);
					}			
					//char para verificar se nao acabou o arquivo
					char endoffile;
					fread(&endoffile,sizeof(char),1,arqbin);
					//verifica se nao acabou o arquivo
					if(feof(arqbin)){
						if(encont == 0){	//se nao encontrou nenhum registro, retorna inexistente
							printf("Registro inexistente.");
						}
						break;
					}
					//se nao acabou, volta 1 byte
					else{
						fseek(arqbin,-1,SEEK_CUR);
					}
				} //fim do while arqbin
				//atualiza o topoPilha no cabeçalho
				fseek(arqbin,1,SEEK_SET);
				fwrite(&topoPilha,sizeof(int),1,arqbin);
			}
			//caso nao for nenhum, dá erro
			else{
				printf("Falha no processamento do arquivo.");
				return 0;
			}

		}	//acaba for de atual a n

		//printa o arquivo no stdout com a função feita pelo monitor
		//binarioNaTela1(arqbin);
		//fecha o arquivo de leitura/escrita
		fclose(arqbin);

	} //acaba a funcionalidade 5
	
	//Funcionalidade 6 - inserção
	else if(func == '6'){
		//funcao para gerar uma string readFile e uma string nc
		char *token = strtok(entrada, " ");
		for(i = 0; i < 3; i++){
			if(i == 1 && token){
				strcpy(readFile,token);
			}
			else if(i == 2 && token){
				strcpy(nc,token);
			}
			token = strtok(NULL," ");
		}
		//verifica se nao tem nenhum \n nas strings
		for(i = 0; i < 30; i++){
			if(i < 5){
				if(nc[i] == '\n'){
					nc[i] = '\0';
				}
			}
			if(readFile[i] == '\n'){
				readFile[i] = '\0';
			}
		}
		//verifica se o arquivo readFile existe
		FILE *arqbin = fopen(readFile,"r+b");	//abre o arquivo readFile
		if(!arqbin){							//se nao conseguir abrir, retorna o erro
			printf("Falha no processamento do arquivo.");	
			return 0;
		}
		//verifica se o arquivo está consistente
		char status;
		fread(&status,sizeof(char),1,arqbin);
		if(status != '1'){
			printf("Falha no processamento do arquivo");
			return 0;
		}
		//le o topo da pilha no cabeçalho
		fread(&topoPilha,sizeof(int),1,arqbin);
		//verifica se nc é um numero valido
		if(ehNumero(nc)){			//se for um numero valido
			n = strtol(nc,NULL,10);	//transforma a string nc em um int n
		}
		else{						//se nao for um numero valido, dá erro
			printf("Falha no processamento do arquivo.");
			return 0;
		}
		//cria as variaveis para leitura dos valores dos campos
		int vni[n];
		double vno[n];
		char vda[n][11];
		char vcid[n][50], vne[n][50];
		//inteiro para armazenar o indicador de tamanho do campo variavel
		int itcv = 0;
		//cria um char para ver se o registro é removido ou nao
		char regrem;
		//cria uma variavel pra ver se algum registro foi encontrado
		int encont; 
		//limpa todas as matrizes
		for(i = 0; i < n; i++){
			for(j = 0; j < 50; j++){
				if(j < 11){
					vda[i][j] = '\0';
				}
				vcid[i][j] = '\0';
				vne[i][j] = '\0';
			}
		}
		//laço para passar lendo a entrada n vezes
		for(atual = 0; atual < n; atual++){
			//limpa a entrada
			for(i = 0; i < 100; i++){
				entrada[i] = '\0';
			}
			//le a proxima entrada
			fgets(entrada,100,stdin);
			//tira o \n da entrada	
			for(i = 0; i < 100; i++){
				if(entrada[i] == '\n'){
					entrada[i] = '\0';
				}
			}
			//variavel auxiliar de apoio na geração das strings variaveis
			int entrou1 = 0, entrou2 = 0;
			//funcao para gerar as strings dos valores dos campos
			char *token = strtok(entrada, " ");
			if(!token){
				return 0;
			}
			//funcao para ler os valores dos campos
			for(i = 0; i < 5; i++){						//sao 5 campos
				//se tiver no campo 0
				if(i == 0 && token){					
					vni[atual] = strtol(token,NULL,10);	//transforma a string em int
				}
				//se tiver no campo 1
				else if(i == 1 && token){	
					//se a nota nao for nula			
					if(strcmp(token,"NULO") != 0){		
						vno[atual] = strtod(token,NULL);//transforma a string em double
					}
				}
				//se tiver no campo 2						
				else if(i == 2 && token){
					strcpy(vda[atual],token);	//copia a string em token para vda
				}
				//se tiver no campo 3
				else if(i == 3 && token){
					strcat(vcid[atual],token);	//copia a string em token para vcid
					//verifica se começa com aspas
					if(vcid[atual][0] == '\"'){
						entrou1++;				//se é string, entra no proximo if
					}
					if(entrou1 > 0){
						//se nao for o final da string, continua concatenando a string
						if(vcid[atual][strlen(vcid[atual])-1] != '\"'){	
							i--;
							strcat(vcid[atual]," ");
						}
					}
				}
				//se tiver no campo 4
				else if(i == 4 && token){
					strcat(vne[atual],token);	//copia a string em token para vne
					//verifica se começa com aspas
					if(vne[atual][0] == '\"'){
						entrou2++;				//se é string, entra no proximo if
					}
					if(entrou2 > 0){
						//se nao for o final da string, continua concatenando a string
						if(vne[atual][strlen(vcid[atual])-1] != '\"'){
							i--;
							strcat(vne[atual]," ");
						}
					}
				}
				//atualiza o token
				token = strtok(NULL," ");
			}	
			//verifica se nao tem nenhum \n nas strings
			for(i = 0; i < 50; i++){
				if(vcid[atual][i] == '\n'){
					vcid[atual][i] = '\0';
				}
				if(vne[atual][i] == '\n'){
					vne[atual][i] = '\0';
				}
				if(i < 11){
					if(vda[atual][i] == '\n'){
						vda[atual][i] = '\0';
					}
				}
			}
			//os ifs abaixo serve para tirar as aspas se tiver, nas entradas dos campos variaveis
			if(vcid[atual][0] == '\"'){
				i = 1;
				while(vcid[atual][i] != '\"'){
					vcid[atual][i-1] = vcid[atual][i];
					i++;
				}
				vcid[atual][i-1] = '\0';
			}
			if(vne[atual][0] == '\"'){
				i = 1;
				while(vne[atual][i] != '\"'){
					vne[atual][i-1] = vne[atual][i];
					i++;
				}
				vne[atual][i-1] = '\0';
			}
			//seta para ler o topo da pilha
			fseek(arqbin,1,SEEK_SET);
			//le o topoPilha
			fread(&topoPilha,sizeof(int),1,arqbin);
			//se nao for -1
			if(topoPilha != -1){
				fseek(arqbin,16000,SEEK_SET);				//pula pro começo dos registros
				fseek(arqbin,80 * topoPilha,SEEK_CUR);		//pula pro rrn do topoPilha
				fread(&regrem,sizeof(char),1,arqbin);		//le o char removido
				//se o registro estiver removido
				if(regrem == '*'){	
					fread(&topoPilha,sizeof(int),1,arqbin);	//atualiza o topoPilha
					fseek(arqbin,-5,SEEK_CUR);				//volta pro inicio do registro
					fputc('-',arqbin);						//marca '-' no byte
					fwrite(&menosum,sizeof(int),1,arqbin);	//escreve -1
					fwrite(&vni[atual],sizeof(int),1,arqbin);//escreve o valor do numero de inscricao
					fwrite(&vno[atual],sizeof(double),1,arqbin);//escreve o valor da nota
					fwrite(&vda[atual],sizeof(char),10,arqbin);//escreve o valor da data
					//verifica se o valor da cidade existe
					if(strcmp(vcid[atual],"NULO") != 0){
						itcv = strlen(vcid[atual]) + 2;		//indicador de tamanho recebe o tamanho da string + 2
						fwrite(&itcv,sizeof(int),1,arqbin);	//escreve o indicador de tamanho
						fputc('4',arqbin);					//coloca o byte '4'
						fwrite(&vcid[atual],sizeof(char),strlen(vcid[atual])+1,arqbin);//escreve o campo variavel
					}
					//verifica se o valor do nome da escola existe
					if(strcmp(vne[atual],"NULO") != 0){
						itcv = strlen(vne[atual]) + 2;		//indicador de tamanho recebe o tamanho da string + 2
						fwrite(&itcv,sizeof(int),1,arqbin);	//escreve o indicador de tamanho
						fputc('5',arqbin);					//coloca o byte '5'
						fwrite(&vne[atual],sizeof(char),strlen(vne[atual])+1,arqbin);//escreve o campo variavel
					}
				}
				//se o registro nao tiver removido, erro
				else{
					printf("Falha no processamento do arquivo.");
					return 0;
				}
				//atualiza o topo da pilha no cabeçalho
				fseek(arqbin,1,SEEK_SET);
				fwrite(&topoPilha,sizeof(int),1,arqbin);
			}
			//se for -1, coloca no final do arquivo
			else{
				fseek(arqbin,0,SEEK_END);					//pula pro final do arquivo
				fputc('-',arqbin);							//coloca o byte '-'
				fwrite(&menosum,sizeof(int),1,arqbin);		//escreve -1
				fwrite(&vni[atual],sizeof(int),1,arqbin);	//escreve o valor do numero de inscricao
				fwrite(&vno[atual],sizeof(double),1,arqbin);//escreve o valor da nota
				fwrite(&vda[atual],sizeof(char),10,arqbin);	//escreve o valor da data
				//verifica se o valor da cidade existe
				if(strcmp(vcid[atual],"NULO") != 0){
					itcv = strlen(vcid[atual]) + 2;			//indicador de tamanho recebe o tamanho da string + 2
					fwrite(&itcv,sizeof(int),1,arqbin);		//escreve o indicador de tamanho
					fputc('4',arqbin);						//coloca o byte '4'
					fwrite(&vcid[atual],sizeof(char),strlen(vcid[atual]+1),arqbin);//escreve o campo variavel
				}
				//verifica se o valor do nome da escola existe
				if(strcmp(vne[atual],"NULO") != 0){
					itcv = strlen(vne[atual]) + 2;			//indicador de tamanho recebe o tamanho da string + 2
					fwrite(&itcv,sizeof(int),1,arqbin);		//escreve o indicador de tamanho
					fputc('5',arqbin);						//coloca o byte '5'
					fwrite(&vne[atual],sizeof(char),strlen(vne[atual]+1),arqbin);//escreve o campo variavel
				}
				//completa o registro com lixo
				for(i = 0; i < (80 - (ftell(arqbin) % 80)); i++){
					fputc('@',arqbin);
				}
			}
		} //acaba o for de atual a n
		//printa o arquivo no stdout com a função feita pelo monitor
		//binarioNaTela1(arqbin);
		//fecha o arquivo
		fclose(arqbin);
	} //acaba a funcionalidade 6

	//Funcionalidade 7 - atualização
	else if(func == '7'){
		//funcao para gerar uma string readFile e uma string nc
		char *token = strtok(entrada, " ");
		for(i = 0; i < 3; i++){
			if(i == 1 && token){
				strcpy(readFile,token);
			}
			else if(i == 2 && token){
				strcpy(nc,token);
			}
			token = strtok(NULL," ");
		}
		//verifica se nao tem nenhum \n nas strings
		for(i = 0; i < 30; i++){
			if(i < 5){
				if(nc[i] == '\n'){
					nc[i] = '\0';
				}
			}
			if(readFile[i] == '\n'){
				readFile[i] = '\0';
			}
		}
		//verifica se o arquivo readFile existe
		FILE *arqbin = fopen(readFile,"r+b");	//abre o arquivo readFile
		if(!arqbin){							//se nao conseguir abrir, retorna o erro
			printf("Falha no processamento do arquivo.");	
			return 0;
		}
		//verifica se o arquivo está consistente
		char status;
		fread(&status,sizeof(char),1,arqbin);
		if(status != '1'){
			printf("Falha no processamento do arquivo");
			return 0;
		}
		//le o topo da pilha no cabeçalho
		fread(&topoPilha,sizeof(int),1,arqbin);
		//verifica se nc é um numero valido
		if(ehNumero(nc)){			//se for um numero valido
			n = strtol(nc,NULL,10);	//transforma a string nc em um int n
		}
		else{						//se nao for um numero valido, dá erro
			printf("Falha no processamento do arquivo.");
			return 0;
		}
		//cria as variaveis para leitura dos nomes dos campos e valores
		char nomeCampo[n][50], valorCampo[n][50];
		//inteiro para ler o RRN da entrada
		int rrn[n];
		//cria as variaveis para escrita
		int nilido;			//numero inscricao lido
		double notalida;	//nota lida
		char cvlido[50];	//campo variavel lido
		//inteiros para armazenar os indicadores de tamanho dos campos variaveis
		int itcv1, itcv2;
		//cria os char tag4 e tag5
		char tag4 = '4';
		char tag5 = '5';
		//cria uma variavel char para ler se o registro está removido ou nao
		char regrem;
		//limpa todas as matrizes
		for(i = 0; i < n; i++){
			for(j = 0; j < 50; j++){
				nomeCampo[i][j] = '\0';
				valorCampo[i][j] = '\0';
				cvlido[j] = '\0';
			}
		}
		//laço para passar lendo a entrada n vezes	
		for(atual = 0; atual < n; atual++){
			//limpa a entrada
			for(i = 0; i < 100; i++){
				entrada[i] = '\0';
			}
			//le a proxima entrada
			fgets(entrada,100,stdin);
			//tira o \n da entrada	
			for(i = 0; i < 100; i++){
				if(entrada[i] == '\n'){
					entrada[i] = '\0';
				}
			}
			//funcao para gerar uma string nomeCampo[atual] e uma string valorCampo[atual]
			char *token = strtok(entrada, " ");
			if(!token){
				return 0;
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
			//verifica se a entrada é igual a "nroInscricao"
			if(strcmp(nomeCampo[atual],"nroInscricao") == 0){
				fseek(arqbin,16000 + (rrn[atual]*80), SEEK_SET);	//pula pro rrn
				fseek(arqbin,5,SEEK_CUR);							//pula 5 bytes
				nilido = strtol(valorCampo[atual],NULL,10);			//transforma a string em int
				fwrite(&nilido,sizeof(int),1,arqbin);				//escreve o novo inteiro
			}
			//verifica se a entrada é igual a "nota"
			else if(strcmp(nomeCampo[atual],"nota") == 0){		
				fseek(arqbin,16000 + (rrn[atual]*80), SEEK_SET);	//pula pro rrn
				fseek(arqbin,9,SEEK_CUR);							//pula 9 bytes
				notalida = strtod(valorCampo[atual],NULL);			//transforma a string em double
				fwrite(&notalida,sizeof(double),1,arqbin);			//escreve o novo double
			}
			//verifica se a entrada é igual a "data"
			else if(strcmp(nomeCampo[atual],"data") == 0){
				fseek(arqbin,16000 + (rrn[atual]*80), SEEK_SET);	//pula pro rrn
				fseek(arqbin,17,SEEK_CUR);							//pula 17 bytes
				fwrite(&valorCampo[atual],sizeof(char),10,arqbin);	//escreve a nova string
			}
			//verifica se a entrada é igual a "cidade"
			else if(strcmp(nomeCampo[atual],"cidade") == 0){
				int vclen = strlen(valorCampo[atual]) + 2;			//inteiro pra guardar o tamanho da string
				fseek(arqbin,16000 + (rrn[atual]*80), SEEK_SET);	//pula pro rrn
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
						fwrite(&vclen,sizeof(int),1,arqbin);		//escreve o tamanho da string
						fputc('4',arqbin);							//escreve o byte '4'
						fwrite(&valorCampo[atual],sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
						fwrite(&itcv2,sizeof(int),1,arqbin);		//escreve o indicador de tamanho do campo variavel 2
						fputc('5',arqbin);							//escreve o byte '5'
						fwrite(&cvlido,sizeof(char),itcv2-1,arqbin);//escreve o campo variavel lido
						//completa o registro com lixo
						for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
							fputc('@',arqbin);
						}
					}
					//se nao tiver um campo nomeEscola
					else{
						fseek(arqbin,-(5 + itcv1 + 4),SEEK_CUR);	//volta pra onde deveria estar o indicador de tamanho do campo variavel 1
						fwrite(&vclen,sizeof(int),1,arqbin);		//escreve o tamanho da string
						fputc('4',arqbin);							//escreve o byte '4'
						fwrite(&valorCampo[atual],sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
						//completa o registro com lixo
						for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
							fputc('@',arqbin);
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
					fwrite(&valorCampo[atual],sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
					fwrite(&itcv1,sizeof(int),1,arqbin);			//escreve o indicador de tamanho do campo variavel 1
					fputc('5',arqbin);								//escreve o byte '5'
					fwrite(&cvlido,sizeof(char),itcv1-1,arqbin);	//escreve o campo variavel lido
					//completa o registro com lixo
					for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
						fputc('@',arqbin);
					}
				}
				//se nao tiver nenhum campo variavel
				else{
					fseek(arqbin,-5,SEEK_CUR);						//volta 5 bytes	
					fwrite(&vclen,sizeof(int),1,arqbin);			//escreve o tamanho da string
					fputc('4',arqbin);								//escreve o byte '4'
					fwrite(&valorCampo[atual],sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
					//completa o registro com lixo
					for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
						fputc('@',arqbin);
					}
				}
			}
			//verifica se a entrada é igual a "nomeEscola"
			else if(strcmp(nomeCampo[atual],"nomeEscola") == 0){
				int vclen = strlen(valorCampo[atual]) + 2;			//inteiro pra guardar o tamanho da string
				fseek(arqbin,16000 + (rrn[atual]*80), SEEK_SET);	//pula pro rrn
				fseek(arqbin,31,SEEK_CUR);							//pula 31 bytes
				fread(&tag5,sizeof(char),1,arqbin);					//le o char
				//se tiver um campo nomeEscola
				if(tag5 == '5'){
					fseek(arqbin,-5,SEEK_CUR);						//volta 5 bytes	
					fwrite(&vclen,sizeof(int),1,arqbin);			//escreve o tamanho da string
					fputc('5',arqbin);								//escreve o byte '5'
					fwrite(&valorCampo[atual],sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
					//completa o registro com lixo
					for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
						fputc('@',arqbin);
					}	
				}
				//se tiver um campo cidade
				else if(tag5 == '4'){
					fseek(arqbin,-5,SEEK_CUR);						//volta 5 bytes	
					fread(&itcv1,sizeof(int),1,arqbin);				//le o inteiro do indicador de tamanho do campo variavel 1 (cidade)
					fseek(arqbin,itcv1,SEEK_CUR);					//pula os bytes do campo variavel
					fwrite(&vclen,sizeof(int),1,arqbin);			//escreve o tamanho da string
					fputc('5',arqbin);								//escreve o byte '5'
					fwrite(&valorCampo[atual],sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
					//completa o registro com lixo
					for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
						fputc('@',arqbin);
					}
				}
				//se nao tiver nenhum campo variavel
				else{
					fseek(arqbin,-5,SEEK_CUR);						//volta 5 bytes	
					fwrite(&vclen,sizeof(int),1,arqbin);			//escreve o tamanho da string
					fputc('5',arqbin);								//escreve o byte '5'
					fwrite(&valorCampo[atual],sizeof(char),vclen-1,arqbin);//escreve a string lida da entrada
					//completa o registro com lixo
					for(i = 0; i < 80 - (ftell(arqbin) % 80); i++){
						fputc('@',arqbin);
					}
				}
			}
			//caso a entrada nao pertencer a algum campo
			else{
				printf("Falha no processamento do arquivo.");
				return 0;
			}

		} //acaba o for de atual a n
		//printa o arquivo no stdout com a função feita pelo monitor
		//binarioNaTela1(arqbin);
		//fecha o arquivo
		fclose(arqbin);
	} //acaba a funcionalidade 7

	//Caso nao tiver a funcionalidade implementada ainda
	else{
		printf("Funcionalidade %c nao implementada.\n",func);
		return 0;
	}


	return 0;
}