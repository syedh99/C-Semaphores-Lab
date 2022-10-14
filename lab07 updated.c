//HOMEWORK03 by RUI CHEN fb4138

#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <semaphore.h> 
//declare globe var, such as the three matrices and mutex lock
int matrix1[256][256], matrix2[256][256], result[256][256];

//to declare semaphore
sem_t mutex;

//this func is to write a matrix into a file called "output.txt"
int matrixOutput(int matrix[256][256]){
	FILE *fp=fopen("output.txt", "w+");
	if(fp==NULL){
		printf("could not open file!\n");
		exit(-1);
	}
	
	for(int i=0; i<256; i++){
		for(int j=0; j<256; j++){
			fprintf(fp,"%d ", matrix[i][j]);	
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}


void* matrixAdd(void *i){		
	//first need to convert i datatype
	int num=(int)i;

	//put a semaphore so only one thread can modify the result matrix at once
	sem_wait(&mutex);

	//this thread will do addition for rows from num*32 to num*32+31
	for(int i=num*32;i<=(num*32)+31;i++)
	{
		for(int j=0;j<256;j++)
		{
			result[i][j] = matrix1[i][j] + matrix2[i][j];
		}
	}
	
	printf("Thread %d Done!\n", num);
	
	//to release the semaphore
	sem_post(&mutex);

	pthread_exit(NULL);
}


int main(){
	//to initialize semaphore
	sem_init(&mutex,0,1);

	//firstly initialize the two matrices
	//both matrix 1 and 2 will be like:
	// 1,2,...,256
	// 1,2,...,256
	// ...
	// 1,2,...,256
	for(int i=0; i<256; i++)
	{
		for(int j=0; j<256; j++)
		{
		
			matrix1[i][j]=j+1;
			matrix2[i][j]=j+1;
			result[i][j]=0;
		}
	}

	//secondly to create the threads and assign jobs for them
	pthread_t thread[8];	
	for(int i=7; i>=0; i--) 
	{ 
		pthread_create(&thread[i],NULL,&matrixAdd,i);
	}

	//thirdly use pthread join to make sure all threads are done before the main thread
	for(int i=7; i>=0; i--) 
	{ 
		pthread_join(thread[i],NULL);
	}
	
	//lastly to write the result into output.txt
	matrixOutput(result);
	
	return 0;
}
