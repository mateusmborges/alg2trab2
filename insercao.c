/* * * * * * * * * * * * * * * * * * *
	Aluno: Mateus Morishigue Borges
	NUSP: 9850328
* * * * * * * * * * * * * * * * * * */

#include "insercao.h"
#include "auxlib.h"

int insercao(char* readFile, int n){

	/* 	DECLARAÇÃO DE VARIÁVEIS 
		i = variavel para laços for
		j = variavel para laços for
		atual = variavel do laço para ler todas as linhas de entrada
		entrou1 = inteiro para apoio na geração das strings variáveis
		entrou2 = inteiro para apoio na geração das strings variáveis
		vni = valor do numero de inscrição
		itcv1 = inteiro para armazenar o indicador de tamanho do campo variavel 1
		itcv2 = inteiro para armazenar o indicador de tamanho do campo variavel 2
		topoPilha = inteiro para guardar o topo da pilha
		vno = valor da nota
		status = char para guardar o status do arquivo
		vda = valor da data
		vcid = valor da cidade
		vne = valor do nomeEscola
		regrem = char para ver se o registro é removido ou nao
	*/
	int i, j, atual, entrou1 = 0, entrou2 = 0, vni[n], itcv1 = 0, itcv2 = 0, topoPilha;
	const int menosum = -1;
	double vno[n];
	char status, vda[n][13], vcid[n][50], vne[n][50], regrem;

	for(i = 0; i < n; i++){
		clearstring(vda[i],13);
		clearstring(vcid[i],50);
		clearstring(vne[i],50);
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
	//fwrite(&status,sizeof(char),1,arqbin);

	//le o topo da pilha no cabeçalho
	fread(&topoPilha,sizeof(int),1,arqbin);

	/* Laço para passar lendo a entrada n vezes */
	for(atual = 0; atual < n; atual++){
		//ler a proxima entrada
		char entrada[100];
		clearstring(entrada,100);
		fgets(entrada,100,stdin);
		
		trim(entrada); 		//tira o \n e o \r da entrada

		//funcao para gerar as strings dos valores dos campos
		char *token = strtok(entrada, " ");
		if(!token){
			return ERR_NOTTOKEN;
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
				else{
					vno[atual] = -1.0;
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
					if(vne[atual][strlen(vne[atual])-1] != '\"'){
						i--;
						strcat(vne[atual]," ");
					}
				}
			}
			//atualiza o token
			token = strtok(NULL," ");
		}
		trim(vcid[atual]);
		trim(vne[atual]);
		trim(vda[atual]);
		
		//os ifs abaixo serve para tirar as aspas se tiver, nas entradas dos campos variaveis
		if(vda[atual][0] == '\"'){
			i = 1;
			while(vda[atual][i] != '\"'){
				vda[atual][i-1] = vda[atual][i];
				i++;
			}
			vda[atual][i-1] = '\0';
		}
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
			fseek(arqbin,80 * topoPilha,SEEK_CUR);	//pula pro rrn do topoPilha
			fread(&regrem,sizeof(char),1,arqbin);		//le o char removido
			//se o registro estiver removido
			if(regrem == '*'){	
				fread(&topoPilha,sizeof(int),1,arqbin);	//atualiza o topoPilha
				fseek(arqbin,-5,SEEK_CUR);				//volta pro inicio do registro
				fputc('-',arqbin);						//marca '-' no byte
				fwrite(&menosum,sizeof(int),1,arqbin);	//escreve -1
				fwrite(&vni[atual],sizeof(int),1,arqbin);//escreve o valor do numero de inscricao
				fwrite(&vno[atual],sizeof(double),1,arqbin);//escreve o valor da nota
				//verifica se o valor da data existe
				if(ehnulo(vda[atual],13)){		//se nao existir, coloca "\0@@@@@@@@@"
					for(j = 0; j < 10; j++){
						if(j == 0){
							fputc('\0',arqbin);
						}
						else{
							fputs("@",arqbin);
						}
					}
				}
				//se existir
				else{
					fwrite(&vda[atual],sizeof(char),10,arqbin);//escreve o valor da data
				}
				//verifica se o valor da cidade existe
				if(!ehnulo(vcid[atual],50)){
					if(strlen(vcid[atual])){
						itcv1 = strlen(vcid[atual]) + 2;		//indicador de tamanho recebe o tamanho da string + 2
						fwrite(&itcv1,sizeof(int),1,arqbin);	//escreve o indicador de tamanho
						fputc('4',arqbin);						//coloca o byte '4'
						fwrite(&vcid[atual],sizeof(char),itcv1-1,arqbin);//escreve o campo variavel
					}
				}
				//verifica se o valor do nome da escola existe
				if(!ehnulo(vne[atual],50)){
					if(strlen(vne[atual])){
						itcv2 = strlen(vne[atual]) + 2;			//indicador de tamanho recebe o tamanho da string + 2
						fwrite(&itcv2,sizeof(int),1,arqbin);	//escreve o indicador de tamanho
						fputc('5',arqbin);						//coloca o byte '5'
						fwrite(&vne[atual],sizeof(char),itcv2-1,arqbin);//escreve o campo variavel
					}
				}
			}
			//se o registro nao tiver removido, erro
			else{
				printf("Falha no processamento do arquivo.");
				return ERR_REGNOTREM;
			}
			//atualiza o topo da pilha no cabeçalho
			fseek(arqbin,1,SEEK_SET);
			fwrite(&topoPilha,sizeof(int),1,arqbin);
		}
		//se for -1, coloca no final do arquivo
		else{
			fseek(arqbin,0,SEEK_END);					//pula pro final do arquivo
			int faltante = ftell(arqbin) % 80;
			if(faltante != 0){
				faltante = 80 - faltante;
			}
			for(j = 0; j < faltante; j++){
				fputs("@",arqbin);
			}
			fputc('-',arqbin);							//coloca o byte '-'
			fwrite(&menosum,sizeof(int),1,arqbin);		//escreve -1
			fwrite(&vni[atual],sizeof(int),1,arqbin);	//escreve o valor do numero de inscricao
			fwrite(&vno[atual],sizeof(double),1,arqbin);//escreve o valor da nota
			if(ehnulo(vda[atual],13)){		//se nao existir, coloca "\0@@@@@@@@@"
				for(j = 0; j < 10; j++){
					if(j == 0){
						fputc('\0',arqbin);
					}
					else{
						fputs("@",arqbin);
					}
				}
			}
			//se existir
			else{
				fwrite(&vda[atual],sizeof(char),10,arqbin);//escreve o valor da data
			}
			//verifica se o valor da cidade existe
			if(!ehnulo(vcid[atual],50)){
				if(strlen(vcid[atual])){
					itcv1 = strlen(vcid[atual]) + 2;			//indicador de tamanho recebe o tamanho da string + 2
					fwrite(&itcv1,sizeof(int),1,arqbin);		//escreve o indicador de tamanho
					fputc('4',arqbin);							//coloca o byte '4'
					fwrite(&vcid[atual],sizeof(char),itcv1-1,arqbin);//escreve o campo variavel
				}
			}
			//verifica se o valor do nome da escola existe
			if(!ehnulo(vne[atual],50)){
				if(strlen(vne[atual])){
					itcv2 = strlen(vne[atual]) + 2;				//indicador de tamanho recebe o tamanho da string + 2
					fwrite(&itcv2,sizeof(int),1,arqbin);		//escreve o indicador de tamanho
					fputc('5',arqbin);							//coloca o byte '5'
					fwrite(&vne[atual],sizeof(char),itcv2-1,arqbin);//escreve o campo variavel
				}
			}
			//completa o registro com lixo
			for(i = 0; i < (80 - (ftell(arqbin) % 80)); i++){
				fputs("@",arqbin);
			}
			//Completa ate o final do registro com lixo
			fseek(arqbin,0,SEEK_END);					//pula pro final do arquivo
			faltante = ftell(arqbin) % 80;
			if(faltante != 0){
				faltante = 80 - faltante;
			}
			for(j = 0; j < faltante; j++){
				fputs("@",arqbin);
			}
		}
	} //acaba o for de atual a n

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