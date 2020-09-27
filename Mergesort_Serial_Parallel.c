/*
  Author : Gopal Panigrahi
  Date : 19 September 2020
  Description : In this program, Mergesort is implemented by using task generation
  and parallel merge. Two different parallel implementations are written.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

/*
  Used to swap the values of two variables
*/
void swap(int *a, int *b){
  int temp = *a;
  *a = *b;
  *b = temp;
}

/*
  Insertion sort if used to cutoff the mergesort of small array size.
  Using Insertion sort improves the performance
*/
void insertion_sort(int array[],int low,int high){
  for(int i=low,j;i<=high;i++){
    int temp=array[i];
    for(j=i-1;j>=low && array[j]>temp;j--){
      array[j+1]=array[j];
    }
    array[j+1]=temp;
  }
}

/*
  Lower Bound function returns the number of elements before the key in the
  array.
*/
int lower_bound(int array[],int low,int high,int key){
  if(key<=array[low]){return 0;}
  if(key>array[high-1]){return (high-low);}
  int l = low;
  while(low<high){
        int mid = (low + high)/2;
        if (key <= array[mid]) {
            high = mid;
        } else {
            low = mid + 1;
        }
  }
  return low-l;
}

/*
  Serial Merge using auxillary array
  Two pointers are initialized to start of orginal and auxillary array and
  One pointer to the Right half of the original array at mid+1
*/
void merge_ser(int array[],int low,int mid,int high,int aux[]){
  for(int i=low,j=low,k=mid+1;i<=high;i++){
    if(j>mid){
      aux[i]=array[k++];
    }else if(k>high){
      aux[i]=array[j++];
    }else if(array[j]<=array[k]){
      aux[i]=array[j++];
    }else{
      aux[i]=array[k++];
    }
  }

  for(int i=low;i<=high;i++){
    array[i] = aux[i];
  }
}

/*
Parallel Merge Technique
To merge the Left Half and Right Half of orginal array into Auxillary Array
Let three array be L,R,A
  rank(v,M) = position of value v in array M
  rank(x,A) = rank(x,L) + rank(x,R)
Here for every element in L, binary search is applied on R and vice versa.
This process can be done in parallel. Hence for n elements on n processors this
merge takes only lg(n) time.
*/
void merge_par(int array[],int low,int mid,int high,int aux[]){
  int base=low, end=mid-low;
  #pragma omp parallel for num_threads(4)
  for(int i=0;i<=end;i++){
    int index = i+lower_bound(array,mid+1,high+1,array[i+base]);
    aux[index+low] = array[i+base];
  }
  base=mid+1;
  end=high-mid-1;

  #pragma omp parallel for num_threads(4)
  for(int i=0;i<=end;i++){
    int index = i+lower_bound(array,low,mid+1,array[i+base]);
    aux[index+low] = array[i+base];
  }
  for(int i=low;i<=high;i++){
    array[i] = aux[i];
  }
}

/*
  Task construct is used to create mergesort tasks and are merge parallely.
  After a cutoff the smaller tasks will execute serially.
*/

void mergesort_parallel_merge(int array[],int low,int high,int aux[]) {
  if((high-low)<=20){
    insertion_sort(array,low,high);
    return;
  }
  int mid=(low+high)/2;
  #pragma omp task shared(array,mid,high,aux) final((high-low)<=8192)
    mergesort_parallel_merge(array,low,mid,aux);
  #pragma omp task shared(array,mid,high,aux) final((high-low)<=8192)
    mergesort_parallel_merge(array,mid+1,high,aux);
  #pragma omp taskwait
  if((high-low)<=8192)
    merge_ser(array,low,mid,high,aux);
  else
    merge_par(array,low,mid,high,aux);
}

/*
  Here, function calls are divided in tasks. After a cutoff the tasks
  generation stops.Here, only serial merging is utilized
*/
void mergesort_par(int array[],int low,int high,int aux[]) {
    if((high-low)<=20){
      insertion_sort(array,low,high);
      return;
    }
    int mid=(low+high)/2;
    #pragma omp task shared(array,mid,high,aux) final((high-low)<=8192)
      mergesort_par(array,low,mid,aux);
    #pragma omp task shared(array,mid,high,aux) final((high-low)<=8192)
      mergesort_par(array,mid+1,high,aux);
    #pragma omp taskwait
    merge_ser(array,low,mid,high,aux);
}

/*
  Serial Mergesort
*/

void mergesort_ser(int array[],int low,int high,int aux[]) {
  if((high-low)<=20){
    insertion_sort(array,low,high);
    return;
  }
  int mid=(low+high)/2;
  mergesort_ser(array,low,mid,aux);
  mergesort_ser(array,mid+1,high,aux);
  merge_ser(array,low,mid,high,aux);
}

void main(){
  srand(time(0));
  int size = 100000000;
  int *array = (int*)malloc(size*sizeof(int));
  int *aux = (int*)malloc(size*sizeof(int));
  double end,start;

  // Initializes array with random values
  for(int i=0;i<size;i++)
  {
    int num=rand()%(i+1);
    array[i]=i;
    swap(&array[num],&array[i]);
  }

  // SERIAL REGION
  start = omp_get_wtime();
  mergesort_ser(array,0,size-1,aux);
  end = omp_get_wtime();
  printf("Time taken for Serial Execution %lf\n",end-start);

  // Initializes array with random values
  for(int i=0;i<size;i++)
  {
    int num=rand()%(i+1);
    array[i]=i;
    swap(&array[num],&array[i]);
  }

  // PARALLEL REGION
  start = omp_get_wtime();
  #pragma omp parallel
  {
    #pragma omp single
    {
      mergesort_par(array,0,size-1,aux);
    }
  }
  end = omp_get_wtime();
  printf("Time taken for Parallel Execution %lf\n",end-start);

  // Initializes array with random values
  for(int i=0;i<size;i++)
  {
    int num=rand()%(i+1);
    array[i]=i;
    swap(&array[num],&array[i]);
  }

  // PARALLEL REGION
  start = omp_get_wtime();
  #pragma omp parallel
  {
    #pragma omp single
    {
      mergesort_parallel_merge(array,0,size-1,aux);
    }
  }
  end = omp_get_wtime();
  printf("Time taken for Parallel Execution with Parallel merge %lf\n",end-start);

}
