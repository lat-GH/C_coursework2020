#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//defining the size of the hash tabel as a constant
#define HASHTABLESIZE 100 

//----------------------------string functions-----------------------

//copies the contents of the source string into the destination string BUT uses a spesic range that it copies over (prevents copying chars that are not relevant)
void strcopy_subStr(char *dest, char *source, int length){
  char* destStart = dest;
  //copying the contents of t into s
  for(int i =0; i<length; i++){
    *dest = *source;    
    dest++;
    source++;
  }
  *dest = '\0';
}
//checks if 2 strings are equal or not
bool isEqual_Str(char *val1,  char *val2){
  //as long as the chars are equal
  while(*val1 == *val2){
    //if reach then end 
    if(*val1 == '\0' &&*val2 =='\0' ){  //0 cannot equal 0
      return true;
    }
    val1++;
    val2++;
  }
  return false; //they are not equal
}

//-----------------------------linked list methods-------------------
//used to create a linked list of strings
struct STRList{
  char* value;
  struct STRList *next;
};

//appends a string value to the end of the link list
struct STRList* append_Str(char *val, struct STRList *list){
  struct STRList *ptr = list;
  //creates new list element
  struct STRList *li = (struct STRList*)malloc(sizeof(struct STRList));
  li->value = val;
  li -> next = NULL;  
  //case for first element in the list
  if(list == NULL){
   return li;
  } 
  else{
    //finds the end of the list
    while(ptr->next != NULL){ 
      ptr = ptr->next;  
    }
    //appends it ot the end
    ptr->next = li;
    return list;
  }
}
//searches the linked list for a particular string value
bool search_Str(char *val, struct STRList *list){
  struct STRList *ptr = list;
  //while not the end of the list
  while(ptr != NULL){
    //printf("List value: %s, val: %s\n",ptr->value, val);
    if(isEqual_Str(ptr->value, val) == true){
      return true;
    }
    ptr = ptr->next;
  }
  return false;
}
//removes a string (and the struct list elements) from the linked list
struct STRList*  remove_Str(char *val, struct STRList *list){
  struct STRList *ptr = list;
  //pointer to the list item, before the one being removed
  struct STRList *PreviousPtr = NULL;
  while(ptr != NULL){ 
    //printf("List value: %s, val: %s\n",ptr->value, val);//for testing
    //if found the value
    if(isEqual_Str(ptr->value, val) == true){
      // case for removing the first and only element
      if(PreviousPtr == NULL && ptr->next == NULL ){ 
        free(ptr);
        ptr = NULL;    
        return NULL; //there are no longer any elements in the list   
      } 
      //case for removing the first but NOT only element in the
      else if(PreviousPtr == NULL && ptr->next != NULL ){ 
        struct STRList *newPtr = ptr->next; 
        free(ptr);
        ptr = NULL;    
        return newPtr; //returning the new start of the list   
      }  
      //case for removing elements in the middle of the list
      else if(ptr->next != NULL){ 
        PreviousPtr->next = ptr->next; //once the element is removed will need a new next element 
        free(ptr);
        ptr = NULL;    
        return list;//start of the list is still the same
      }
      //when the item to remove is the last item in the list
      else if(ptr->next == NULL){ 
        PreviousPtr->next = NULL;
        free(ptr);
        ptr = NULL;    
        return list; //start of the list is still the same
      }          
    }
    //move pointer forward
    PreviousPtr = ptr;
    ptr = ptr->next;
  }
   printf("value not found so couldnt remove\n");
   return list;
}
//displays the contents of the linked list
void printLinkedList_Str(struct STRList *list, int index){
  if(list!= NULL){ 
     printf("value: %s index: %d\n", list->value,index);
      index++;   
    printLinkedList_Str(list->next, index);
  }
  else{
    //when reaches the end of the list, or list is empty
    //printf("NULL\n");
  }
}

//----------------------------hash table methods----------------
//takes a string and calculates its position in the hash table
int hash(char *n){
  //sum of all the ascii char values of the string
  int sum = 0;
  int val;
  int position;
  for(int i = 0; *n != 0; n++){
    int val = *n;
    sum = sum + val;
    i++;
  }
  position = sum % HASHTABLESIZE;
  //printf("hash: %d\n",position); //for testing
  return position;
}
//searches the hash tabel for a string value
bool search(char *n, struct STRList *ht[]){ //pass aray by value
  int po = hash(n);
  //searches for the strign at teh position in the hash table
  if(search_Str(n,ht[po]) == true){
    return true;
  }
  else{
    return false;
  }
}
//adds a string element to the hash table
void add(struct STRList *n, struct STRList *ht[] ){ //pass by refrence, want to alter the values of the hash table
  int po = hash(n->value); 
  //handles clashes by appending the val to the end of the list at that position 
  ht[po] = append_Str(n->value, ht[po]);  
}
//remobves the value from the hash table
void remov(char *n, struct STRList *ht[] ){//pass by refrence, want to alter the values of the hash table
  int po = hash(n);
  ht[po] = remove_Str(n, ht[po]); 
  
}

//reads names in from a text file and populates the hash table with them
//text file must have format: "NAME1","NAME2","NAME3"
int populateNames(char* fileName, struct STRList *ht[]){  
  //max length of the name
  int maxnameSize = 15; 
  //where the name is stored whilst its being read in from teh file, one char ata time
  char tempName[maxnameSize];
  tempName[0] = ' '; // giving it an inital value, so that the first tempName can assign over something
  
  int offset = 1; //starts as 1 to take into account the first " read in
  int numOfNames = 0;
  int nameSize = 0;
  bool hitFisrtQuote = false;
  //max file size (in chars)
  int maxChar = 5000000;
  char *nameStr = (char*) malloc(maxChar*sizeof(char));

  FILE *file;
  file = fopen(fileName, "r");
  if(file == NULL){
    printf("Could not open file %s",fileName);
    return -1;
  }
  //reads in the file char by char
  while(fgets(nameStr,maxChar,file)!= NULL){
    //printf("%s", nameStr); //for testing
  }
  fclose(file);

  //looping through all the cahrs in nameStr
  for(int c = 0; *nameStr !='\0'; nameStr ++){
    //if hits a second quote, for a pair around the name, end of the name string
      if (*nameStr == '"' && hitFisrtQuote == true ){ 
        hitFisrtQuote = false;
        // need to take into account all of the extra chars inbetween: ","
        offset = c+3; 

        //creates a new list element
        struct STRList nameElement;     
        //need create memory for the name char*, that will be the same size as the name want to copy into
        nameElement.value = (char*)malloc((nameSize+1)*sizeof(char)); 
        //only copying the number of chars read for the name instead of the whole 20 that was allocated 
        strcopy_subStr(nameElement.value, tempName,nameSize); 
        //adding the list to the hass
        add(&nameElement,ht);
        numOfNames ++;
        //emptying the tempName
        for(int i = 0; i<nameSize; i++){
          tempName[i] = ' ';
        }  
        nameSize = 0;    
      }
      //when hit the first quote around the name
      else if(*nameStr == '"' && hitFisrtQuote == false ){
        hitFisrtQuote = true;
      }
      //appending the letter char to the name string
      else if (*nameStr != ',' && hitFisrtQuote == true){        
        //printf("%c \n", *nameStr); //for testing
        tempName[c-offset] = *nameStr; 
        //printf("tempName %s\n", tempName);//for testing
        nameSize ++;
      }
    c++;
  }
  return numOfNames;
}

int main(void) {
  int index =0;
  //array of pointers, NOT a pointer to an array
  struct STRList *hash_table[HASHTABLESIZE]; 
  //initalizses the hash table to all be null
  for(int i = 0; i<HASHTABLESIZE; i++){
    hash_table[i] = NULL;
  }
  //file name reading names from 
  //must follow format: "NAME1","NAME2","NAME3"
  char* filename = "names.txt";

  int num_of_names = populateNames(filename, hash_table); 
  printf("num_of_names: %d \n",num_of_names);

  printf("\n HASH TABLE:\n");
  for(int i = 0; i<HASHTABLESIZE; i++){
    //displays the index of the position in the hash
    printf("%d: \n",i);
    //if the position in the hash is empty
    if(hash_table[i] == NULL){
      printf("Null\n");
    }
    printLinkedList_Str(hash_table[i],index);
  }
    return 0;
}
