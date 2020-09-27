/*
  Author : Gopal Panigrahi
  Date : 19 September 2020
  Description : In this program, Bubble sort is implemented using Odd - even Transposition
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void swap(int *a, int *b){
  int temp = *a;
  *a = *b;
  *b = temp;
}

void main(){
  srand(time(0));
  int size = 500000;
  double end,start;
  int *array = (int*)calloc(size,sizeof(int));

  // Initializes array with random values
  for(int i=0;i<size;i++){
    int rn = rand()%(i+1);
    array[i]=i;
    swap(&array[rn],&array[i]);
  }

  // SERIAL REGION
  start = omp_get_wtime();
  for(int i=0;i<size;i++){
      int first = i&1;
      for(int j=first;j<size-1;j+=2){
        if(array[j]>array[j+1])
          swap(&array[j],&array[j+1]);
      }
  }
  end = omp_get_wtime();
  printf("Serial Time %lf\n",end-start);


  // Initializes array with random values
  for(int i=0;i<size;i++){
    int rn = rand()%(i+1);
    array[i]=i;
    swap(&array[rn],&array[i]);
  }

  // PARALLEL REGION
  start = omp_get_wtime();
  for(int i=0;i<size;i++){
    int first = i&1;
    #pragma omp parallel for default(none) shared(size,array,first)
      for(int j=first;j<size-1;j+=2){
        if(array[j]>array[j+1])
          swap(&array[j],&array[j+1]);

    }
  }
  end = omp_get_wtime();
  printf("Parallel Time %lf\n",end-start);
}
