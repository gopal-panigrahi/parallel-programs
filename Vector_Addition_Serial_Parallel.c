/*
  Author : Gopal Panigrahi
  Date : 5 September 2020
  Description : Two Large vectors are added parallely
*/
#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

void main()
{
  long size = 100000000;
  /*
    Create and initializes large vectors for addition
  */

  int *First = (int*)malloc(size*sizeof(int));
  int *Second = (int*)malloc(size*sizeof(int));
  int *Third = (int*)malloc(size*sizeof(int));

  for(long i=0;i<size;i++){
    First[i]=Second[i]=10;
  }

  // SERIAL REGION
  double start = omp_get_wtime();
  for(long i=0;i<size;i++){
      Third[i]=First[i]+Second[i];
  }
  double end = omp_get_wtime();
  printf("Time Taken In Serial %lf \n",end-start);

  // PARALLEL REGION
  start = omp_get_wtime();
  #pragma omp parallel for default(none) shared(First,Second,Third,size) schedule(auto)
    for(long i=0;i<size;i++){
      Third[i]=First[i]+Second[i];
    }
  end = omp_get_wtime();
  printf("Time Taken In Parallel %lf \n",end-start);
}
