#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LISTSIZE 100

void print_intArr(int* arr, int len){
  for(int i=0; i<len; i++){
    printf("%d",arr[i]);
    if(i<len-1){
      printf(",");//so doesnt print a comma at the end of the list
    }
  }
  printf("\n");
}

//swaps the positions of 2 particuilar chars in a given str and returns a new string
int* swap(int pos_a, int pos_b, int* arr,int len){
  //printf("a %d, b %d\n",pos_a,pos_b);
  int* endArr = (int*)malloc(sizeof(int)*len);
  //loops through all the chars in th string
  for(int i =0; i<len; i++){
    //when reaches the correct position populates it with the other char
    if(i == pos_a){
      endArr[i] = arr[pos_b];
    }
    else if(i == pos_b){
      endArr[i] = arr[pos_a];
    }
    else{
      endArr[i] = arr[i];
    }
    //printf("endStr %s\n",endStr);
  }
  //print_intArr(endArr,len);
  return endArr;
}

int getRandom(int top, int bot){
  int random = rand();
  return random % (top +1-bot)+bot;
  //return random;
}



int* cocktailSort(int list[], int len){
  int start_ix = 1;
  int end_ix = len-1;
  //loops till the range contains no values
  while(start_ix < end_ix){
    //printf("start_ix: %d, end_ix: %d\n",start_ix,end_ix);

    int newStart_ix = end_ix;
    int newEnd_ix = start_ix;
    //printf("BEFORE:");
    //print_intArr(list,len);

    //loops left to right in the gievn range
    for(int i = start_ix; i<end_ix; i++){
      if(list[i] > list[i+1]){
        //printf("swap: %d, %d\n",list[i],list[i+1]);
        list = swap(i,i+1,list,len);
        //will be in index of the last swapped element
        newEnd_ix = i;
      }
    }
    //updating the top of the range
    end_ix = newEnd_ix;
    //printf("MIDDLE: start_ix: %d, end_ix: %d\n",start_ix,end_ix);
    //print_intArr(list,len);

    //looping from righ to left through the range
    for(int j = end_ix; j>start_ix; j--){
      if(list[j-1] > list[j]){
        //printf("swap: %d, %d\n",list[j-1],list[j]);
        list = swap(j,j-1,list,len);
        newStart_ix = j;
      }
    }
    //udating the bottom of the range
    start_ix = newStart_ix ;

    //printf("AFTER:");
    //print_intArr(list,len);
    //printf("AFTER: start_ix: %d, end_ix: %d\n",start_ix,end_ix);

  }
  return list;
}

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
  
  //ARRAY OF 100 RANDOMLY GENERATED +VE ints

  //int UnSorted[] = {1,9,2,8,3,8,4,5,6,7,8,'\0'};  
  //int UnSorted[] = {1,9,2,8,3,18,4,5,6,7,28,'\0'}; 
  int* UnSorted = (int*)malloc(sizeof(int)*LISTSIZE);
  UnSorted = populateList(500,0,LISTSIZE);
  int* sorted = (int*)malloc(sizeof(int)*LISTSIZE);
  printf("Un sorted List:");
  print_intArr(UnSorted,LISTSIZE);
  sorted = cocktailSort(UnSorted, LISTSIZE);
  printf("Sorted List:");
  print_intArr(sorted,LISTSIZE);
  return 0;
}
