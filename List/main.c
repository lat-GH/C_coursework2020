#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//definign a constant to define the size of the list of elements
#define LISTSIZE 100 //determined by the spec

//displays all of the items in a integer array
void print_intArr(int* arr, int len){
  for(int i=0; i<len; i++){
    printf("%d",arr[i]);
    //so doesnt print a comma at the end of the list
    if(i<len-1){
      printf(",");
    }
  }
  printf("\n");
  printf("\n");
}

//swaps the positions of 2 particuilar ints in a given arr and returns a new array
int* swap(int pos_a, int pos_b, int* arr,int len){
  
  int* endArr = (int*)malloc(sizeof(int)*len);
  //loops through all the ints in th array
  for(int i =0; i<len; i++){
    //when reaches the correct position populates it with the other int
    if(i == pos_a){
      endArr[i] = arr[pos_b];
    }
    else if(i == pos_b){
      endArr[i] = arr[pos_a];
    }
    else{
      endArr[i] = arr[i];
    }
  }
  //print_intArr(endArr,len); //for testing
  return endArr;
}

//returns a random number within a giev range
int getRandom(int top, int bot){
  int random = rand();
  return random % (top +1-bot)+bot;
}


//sorts a list of integers into order of size starting lowest to largest, using the cocktail sort algorithm
int* cocktailSort(int list[], int len){
  //index of the start of the range for that pass
  int start_ix = 1;
  //index of the end of the range for that pass
  int end_ix = len-1;

  //loops till the range contains no values
  while(start_ix < end_ix){

    //keeps track of the new range that the next pass will go through
    //elemnst after this point are alraedy sorted into the correct order
    int newStart_ix = end_ix;
    int newEnd_ix = start_ix;

    //loops left to right in the gievn range
    for(int i = start_ix; i<end_ix; i++){
      if(list[i] > list[i+1]){
        list = swap(i,i+1,list,len);
        //will be in index of the last swapped element
        newEnd_ix = i;
      }
    }
    //updating the top of the range
    end_ix = newEnd_ix;

    //looping from righ to left through the range
    for(int j = end_ix; j>start_ix; j--){
      if(list[j-1] > list[j]){
        list = swap(j,j-1,list,len);
        newStart_ix = j;
      }
    }
    //updating the bottom of the range
    start_ix = newStart_ix ;
  }
  return list;
}

//populates an array of pseudeo random number
//top,bottom are the range size for the random numbers
int* populateList(int top, int bottom, int len){
  int* list = (int*)malloc(sizeof(int)*len);
  int* ptr = list;
  for(int i=0; i<len; i++){
    ptr[i] = getRandom(top,bottom);
  }
  *ptr = '\0';
  return list;
}


int main(void) {
  
  //creates a list if unsorted numbers
  int* UnSorted = (int*)malloc(sizeof(int)*LISTSIZE);
  UnSorted = populateList(500,0,LISTSIZE);

  int* sorted = (int*)malloc(sizeof(int)*LISTSIZE);
  printf("Un sorted List:");
  print_intArr(UnSorted,LISTSIZE);  
  //sorts the unsorted list
  sorted = cocktailSort(UnSorted, LISTSIZE);
  printf("Sorted List:");
  print_intArr(sorted,LISTSIZE);
  return 0;
}
