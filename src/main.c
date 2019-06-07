/* Contador de palavras
	*
	* Este programa recebera umvetor de inteiros em sua
	* entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o vetor 
	* ordenado, apos, encerrar.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


pthread_t workers[2];


typedef struct {
	long int *vet_par;//recebe o endereço do vetor que deve ser avaliado (primo ou não)
	int inicio;//inice de inicio;
	int final; //indice de final;
} thread_args;


void merge(long int vetor[], int comeco, int meio, int fim) {
    int com1 = comeco, com2 = meio+1, comAux = 0, tam = fim-comeco+1;
    int *vetAux;
    vetAux = (int*)malloc(tam * sizeof(int));
	
    while(com1 <= meio && com2 <= fim){
	if(vetor[com1] < vetor[com2]) {
	vetAux[comAux] = vetor[com1];
	com1++;
	} else {
	vetAux[comAux] = vetor[com2];
	com2++;
	}
	comAux++;
	}
	
	//Caso ainda haja elementos na primeira metade
    while(com1 <= meio){  
	vetAux[comAux] = vetor[com1];
	comAux++;
	com1++;
	}
	
	 //Caso ainda haja elementos na segunda metade
    while(com2 <= fim) {  
	vetAux[comAux] = vetor[com2];
	comAux++;
	com2++;
	}
	
	//Move os elementos de volta para o vetor original
    for(comAux = comeco; comAux <= fim; comAux++){    
	vetor[comAux] = vetAux[comAux-comeco];
	}
    
	//desaloca o vetor auxiliar;
    free(vetAux);
} 

 void mergeSort(long int *vetor, int comeco, int fim){
	int meio = (fim+comeco)/2;	
	if (comeco < fim) {
		mergeSort(vetor, comeco, meio);
		mergeSort(vetor, meio+1, fim);
		merge(vetor,comeco,meio,fim);
	}
}


void* worker(void *arg) {
	thread_args *info = (thread_args *)arg;//cria a esturutura que ira armazenas os parametros do megeSort
	mergeSort(info->vet_par,info->inicio,info->final);//chama a função mergesort;
	free(info);//desaloca a struct com os argumentos;
	return NULL;
} 



void mergeSort_iter ( long int *v, int n) {
	//cria uma struct que armazenará os parametros da metade direita do vetor
	//cria uma struct que armazenará os parametros da metade direita do vetor
	thread_args *para_dir;
	thread_args *para_esq;
	
	para_dir=(thread_args*)malloc(sizeof(thread_args));
	para_esq=(thread_args*)malloc(sizeof(thread_args));
	
	//atibui valores de inicio e fim da metade esquerda do vetor;
	para_esq->vet_par=v;
	para_esq->inicio=0;
	para_esq->final=n/2;
	
	//atibui valores de inicio e fim da metade direita do vetor;
	para_dir->vet_par=v;
	para_dir->inicio=(n/2)+1;
	para_dir->final=n-1;
	
	//cria 2 threads que analizaram em pararelo cada metade do vetor;		
	pthread_create(&(workers[0]), NULL, worker, para_dir);
	pthread_create(&(workers[1]), NULL, worker, para_esq);
	
	//agrarda as threads terminarem
	for (int i = 0; i <2; i++) {
		pthread_join(workers[i], NULL);
	}
	//Apos completar as thread termos cada metade do vetor alinhada, restando agora fazer um merge entre elas:
	merge(v,0,n/2,n-1);
	
}



int main() {

	long int x[110000];//armazena entrada
	char caracter='\0';
	int i;
	int entrada;

	/*inicializar o vetor de entrada em -1, será critério de parada do loop que percerrerá a entrada*/
	for (i=0;i<110000;i++){
		x[i]=-1;
	}

	/*leitura dos valores*/
	i=0;
	while(caracter!='\n'){
		/*aqui realiza a leitura da entrada*/
		scanf("%li%c", &x[i],&caracter);
		entrada ++;
		i++;
	}


	//chama função auxiliar que dividira o vetor e realizara o processamento em paralelo de cada metade do vetor
	mergeSort_iter(x,entrada);

	//mprimi o vetor ordenado;
	for (i=0;i<entrada;i++){
		if(i!=entrada-1){
			printf("%ld ",x[i]);
		}else{
			printf("%ld\n",x[i]);
		}
	}



}