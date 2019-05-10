all: 
	gcc programaTrab2.c auxlib.h auxlib.c remocao.h remocao.c insercao.h insercao.c atualizacao.h atualizacao.c recuperacao.h recuperacao.c -o programa
run:
	./programa