/* * * * * * * * * * * * * * * * * * *
	Aluno: Mateus Morishigue Borges
	NUSP: 9850328
* * * * * * * * * * * * * * * * * * */

#include "recuperacao.h"
#include "auxlib.h"

#define NAO 0
#define SIM 1


int recuperacao(char* readFile){
	/* 	DECLARAÇÃO DE VARIÁVEIS 
		i = variavel para laços for
		acessos = contador de páginas de disco acessadas
		numReg = contador de numero de registros lidos
		encadeamento = inteiro para guardar o encadeamento
		nroInscricao = inteiro para guardar o nroInscricao
		itcv = inteiro para guardar o indicador do tamanho do campo variavel
		nota = double para guardar a nota
		status = char para guardar o status do arquivo
		regrem = char para ver se o registro é removido ou nao
		arroba = char para passar pelos @
		tag4 = char para guardar a tag 4		
		tag5 = char para guardar a tag 5		
		first = booleano para apoio ao print dos espaços
		campoVar = string para guardar o campo variavel 1
		campoVar2 = string para guardar o campo variavel 2
		data = string para guardar a data
	*/
	int i, acessos = 0, numReg = 0, encadeamento, nroInscricao, itcv;
	double nota;
	char status, regrem, arroba, tag4, tag5;
	int first = NAO;
	char campoVar[25], campoVar2[25], data[11];

	trim(readFile); //tira os \n e \r da string

	//abre o arquivo de leitura
	FILE *arqbin = fopen(readFile,"r");	//abre o arquivo readFile
	if(!arqbin){						//se nao conseguir abrir, retorna o erro
		printf("Falha no processamento do arquivo.");	
		return ERR_NOTFILE;
	}

	//verifica se o arquivo está consistente
	fread(&status,sizeof(char),1,arqbin);
	if(status != '1'){						//se o arquivo nao está consistente
		printf("Falha no processamento do arquivo.2");
		return ERR_NOTCONSIST;
	}

	//seta o ponteiro pro primeiro registro (segunda pagina)
	fseek(arqbin,16000,SEEK_SET);

	//verifica se existem registros na segunda pagina
	if(feof(arqbin)){
		printf("Registro inexistente.");
		return ERR_ARENTREG;
	}
	acessos++;	//incrementa os acessos a disco
	numReg++; 	//incrementa a qtd de registros lidos

	//seta o ponteiro pro começo do buffer
	fseek(arqbin,16000,SEEK_SET);
	acessos++; //incrementa os acessos a disco

	//limpa as strings abaixo
	clearstring(campoVar, 25);
	clearstring(campoVar2, 25);
	clearstring(data, 11);

	/* Loop para percorrer o arquivo */
	while(arqbin){
		//incrementa os acessos a disco a cada 200 registros lidos
		if(numReg >= 200){
			numReg = 0;
			acessos++;
		}

		fread(&regrem,sizeof(char),1,arqbin);	//le o char do status do registro
		//se o registro nao estiver removido
		if(regrem == '-'){
			fread(&encadeamento,sizeof(int),1,arqbin);	//le os 4 bytes seguintes e coloca em encadeamento
			fread(&nroInscricao,sizeof(int),1,arqbin);	//le os 4 bytes seguintes e coloca em nroInscricao
			//se o nroInscricao nao for nulo, printa
			if(nroInscricao != 1077952576){
				printf("%d",nroInscricao);
				first = SIM;
			}
			fread(&nota,sizeof(double),1,arqbin);		//le os 8 bytes seguintes e coloca em nota
			//se a nota nao for nula, printa
			if(nota != -1.0){
				//se é o primeiro a aparecer
				if(first){
					printf(" ");
				}
				printf("%.1f", nota);
				first = SIM;
			}
			fread(&data,sizeof(char),10,arqbin);		//le os 10 bytes seguintes e coloca em data
			//se a data nao for nula
			if(data[0] != '\0'){
				//se é o primeiro a aparecer
				if(first){
					printf(" ");
				}
				printf("%s", data);
				first = SIM;
			}
			fread(&itcv,sizeof(int),1,arqbin);		//le os 4 bytes seguintes e coloca em itcv
			//checa se itcv existe
			if(itcv != 1077952576){
				if(first){
					printf(" ");
				}
				first = SIM;
				printf("%d",itcv-2);							//printa o tamanho do campo variavel
				fread(&tag4,sizeof(char),1,arqbin);				//le o char da tag4
				fread(&campoVar,sizeof(char),itcv-1,arqbin);	//le o campo variavel
				printf(" %s",campoVar);							//printa o campo variavel
				fread(&itcv,sizeof(int),1,arqbin);		//le os 4 bytes seguintes e coloca em itcv
				//checa se itcv existe
				if(itcv != 1077952576){					
					if(first){
						printf(" ");
					}
					first = SIM;
					printf("%d",itcv-2);					//printa o tamanho do campo variavel
					fread(&tag5,sizeof(char),1,arqbin);		//le o char da tag5
					fread(&campoVar2,sizeof(char),itcv-1,arqbin);//le o campo variavel 2
					printf(" %s",campoVar2);				//printa o campo variavel 2
				}
				//se itcv nao existe
				else{
					fseek(arqbin,80-(ftell(arqbin) % 80),SEEK_CUR);	//passa pro proximo registro
				}
			}
			//se itcv nao existe
			else{
				fseek(arqbin,80-(ftell(arqbin) % 80),SEEK_CUR);		//passa pro proximo registro
			}
			printf("\n");
		}
		//se o registro estiver removido
		else{
			fseek(arqbin,79,SEEK_CUR);
		}

		int faltante = ftell(arqbin) % 80;
		if(faltante != 0){
			faltante = 80 - faltante;
		}
		fseek(arqbin,faltante,SEEK_CUR);

		//incrementa a quantidade de registros
		numReg++;

		char endoffile;
		fread(&endoffile,sizeof(char),1,arqbin);
		//verifica se o arquivo nao acabou
		if(!feof(arqbin)){
			fseek(arqbin,-1,SEEK_CUR);
		}
		else{
			break;
		}
	} //sai do loop qnd acabar o arquivo

	//printa a qtd de acessos a paginas de disco
	printf("Número de páginas de disco acessadas: %d",acessos);

	//fecha o arquivo
	fclose(arqbin);
}

		

