/*
  Author : Gopal Panigrahi
  Date : 27 September 2020
  Description : In this program, multiple keys are searched parallely
    in same search space.
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*
  Serial Binary Search
*/

int binary_search(int array[],int key,int size){
  int low=0,high=size-1;
  if(key<array[low] || key>array[high]){
    return 0;
  }
  while(low<=high){
    int mid = (low+high)/2;
    if(array[mid]==key){
      return 1;
    }else if(key<array[mid]){
      high = mid-1;
    }else{
      low = mid+1;
    }
  }
  return 0;
}

void main(){
  int size=100000000;

  int *array = (int*)malloc(size*sizeof(int));

  // Initializes the array
  for(int i=0;i<size;i++){
    array[i] = i;
  }

  // exists array stores if the ith key is in the Original array or not
  int *exists = (int*)calloc(size,sizeof(int));


  /*
    Searches #keys = size in the array
  */
  // SERIAL REGION
  double end,start = omp_get_wtime();
  for(int key=0;key<(size);key++){
      exists[key] = binary_search(array,key,size);
  }
  end = omp_get_wtime();

  printf("\nSerial execution time : %lf\n",end-start);


  // Reset the exists array to 0 values
  for(int i=0;i<size;i++){
    exists[i]=0;
  }

  // PARALLEL REGION
  start = omp_get_wtime();
  #pragma omp parallel for default(none) shared(array,size,exists)
    for(int key=0;key<size;key++){
        exists[key]=binary_search(array,key,size);
    }

  end = omp_get_wtime();

  printf("\nParallel execution time : %lf\n",end-start);
}
