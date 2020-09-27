/*
  Author : Gopal Panigrahi
  Date : 5 September 2020
  Description : A large martix and a vector are multiplied parallely.
*/
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

void main()
{
  long size = 10000;

  /*
    Create and initializes large matrix and vector for multiplication
  */
  int **matrix = (int**)malloc(size*sizeof(int*));
  for(long i=0;i<size;i++){
    matrix[i] = (int*)malloc(size*sizeof(int));
  }

  int *vector = (int*)malloc(size*sizeof(int));
  int *result = (int*)calloc(size,sizeof(int));

  for(long i=0;i<size;i++){
    for(long j=0;j<size;j++){
      matrix[i][j] = i+j;
    }
    vector[i] = i;
  }

  // SERIAL REGION
  double start = omp_get_wtime();
  for(long i=0;i<size;i++){
    for(long j=0;j<size;j++){
      result[i] += matrix[i][j]*vector[j];
    }
  }
  double end = omp_get_wtime();
  printf("Time Taken In Serial %lf \n",end-start);

  // PARALLEL REGION
  start = omp_get_wtime();
  #pragma omp parallel for default(none) collapse(2) shared(matrix,vector,result,size) schedule(auto)
  for(long i=0;i<size;i++){
    for(long j=0;j<size;j++){
      result[i] += matrix[i][j]*vector[j];
    }
  }
  end = omp_get_wtime();
  printf("Time Taken In Parallel %lf \n",end-start);
}
