#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//-----------------strings--------------------

//returns the length of a string
int getLen_Str(char* val){
  int len = 0;
  while(*val != '\0'){
    len ++;
    val ++;
  }
  return len;
}

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


//swaps the positions of 2 particuilar chars in a given str and returns a new string
char* swap(int pos_a, int pos_b, char* str,int len){
  
  char* endStr = (char*)malloc(sizeof(char)*len);
  //loops through all the chars in th string
  for(int i =0; i<len; i++){
    //when reaches the correct position populates it with the other char
    if(i == pos_a){
      endStr[i] = str[pos_b];
    }
    else if(i == pos_b){
      endStr[i] = str[pos_a];
    }
    else{
      endStr[i] = str[i];
    }
    //printf("endStr %s\n",endStr);
  }
  //str[pos_a] = b;//for testing
  //str[pos_b] = a;
  return endStr;
}

//-----------------lists------------------
//used to create a list of strings
struct STRList{
  char* value;
  struct STRList *next;
};

//appends a string to a list
struct STRList* append_Str(char *val, struct STRList *list){
  //printf("Val appending: %s\n", val); //for testing
  struct STRList *ptr = list;
  //creating a list element
  struct STRList *li = (struct STRList*)malloc(sizeof(struct STRList));
  li->value = val;
  li -> next = NULL;  
  //if there is no  starting list element
  if(list == NULL){
   return li;
  } 
  //appending onto a list that already exsists
  else{
    while(ptr->next != NULL){ 
      ptr = ptr->next;  
    }
    ptr->next = li;
    return list;
  }
}

//displays a the list of strings
void printLinkedList_Str(struct STRList *list, int index){
  struct STRList *ptr = list;
  while(ptr != NULL){
    char* local = ptr->value;
    printf("value: %s index: %d\n",local,index);
    index++;
    ptr = ptr->next;
  }
  //printf("Null\n");
}

//-------------------------main cypher----------------------
//reads in the message file and converts it into one long string
char* readfile(char* fileName){
  
  FILE *file;
  file = fopen(fileName, "r");
  bool fileEnd = false;

  int chrCounter = 0; //so can malloc the correct size of memory for the file  

  //looping through all the chars in the file
  while(fileEnd == false){
    char Chr = fgetc(file);
    //checking for the end of the file
    if(feof(file)){
      fileEnd = true;
    }else{
      chrCounter ++; //counting all the chars in the file
    }
  }
  fclose(file);

  //printf("counted all the chars: %d\n",chrCounter);

  //creating a string of the correct size
  char* message = (char*)malloc(sizeof(char)*chrCounter+1);
  char* start = message;
  //reopening the file
  file = fopen(fileName, "r");
  fileEnd = false;
  //looping through all the chars in the file again
  while(fileEnd == false){
    char chr = fgetc(file);
    if(feof(file)){
      fileEnd = true;
    }else{
      //appending all of the chars to the message, excpet new lines
      if(chr !='\n' && chr != '\r' ){
         *message = chr; 
      }
       message ++;     
    }
  }
  *message = '\0';
  fclose(file);
  //printf("start %s\n",message);
  //returning the whole file as a string
  return start;
}

//populates all of the rows of the cypher grid
struct STRList* populate(char* k, int kl, char* message, struct STRList *before){
  
  //printf("kl: %d\n", kl);
  //temp row of the grid, get rewritten for each row
  char *temp_row = (char*)malloc(sizeof(char)*kl+1);
  //the key that the rows are based on
  char key[kl];
  strcopy_subStr(key,k,kl);
  //printf("key[] %s\n", key); 

  //adds it as the first element of the list 
  before = append_Str(key, before);
  
  //initalizing temprow[]
  for(int i =0; i< kl+1; i++){
    //need to end the string properly with '\0', not auctomatic
    temp_row[i] = '\0'; 
  } 

  //looping through all of the chars in the message
  bool message_end = false;
  bool end = false;
  while(message_end == false){
    //populating the rows of the same size as the key
    for(int j = 0; j<kl; j++){
      //printf("j: %d ",j);
      if(*message != '\0'){
        temp_row[j] = *message;
        message++;
      }
      //if the last row has no chars from the message then doesnt add it to the grid
      else if(j == 0){
        message_end = true;
        end = true;
        //temp_row[j] = '\0';
      }
      //if the message doesnt fit the grid perfectly then populates the remianing chars with xs
      else{
        message_end = true;
        temp_row[j] = 'x';
      }
    }

    if(end == false){
      //need to create a new pointer for each row
      char *row = (char*)malloc(sizeof(char)*kl);
      strcopy_subStr(row,temp_row,kl);
      //printf("row[] %s\n", row);
      append_Str(row,before);
    }
  }
  return before;
}

//applies the a list conating all of the rows
struct STRList* ct_cypher(int kl, struct STRList *before,struct STRList *after){
  char key[kl];
  //populates the key read off as the first row in the list
  strcopy_subStr(key, before->value, kl);

  char* tempKey = (char*)malloc(sizeof(char)*kl);
  strcopy_subStr(tempKey, key, kl);

  char* order = (char*)malloc(sizeof(char)*kl);
  //used to reffernce the orginal indcies of each of the letters in the string, the used to re order the other rows in the same order
  char *ptr = order;
  //popultes a string with all the ascii chars of the the numbers 1-length of key
  //number 0 char in ascii is 48
  for(char i=48; i<(48+kl); i++){
    *ptr = i; 
    ptr ++;
  }
  //printf("order: %s\n",order);
  
  char sortedKey[kl];

  bool swapped = true;
  //applied a buuble sort to sort the letters of the key alphabeticaly
  for(int x = 0; x<kl; x++){
    swapped = false;
    for(int i = 1; i<kl; i++){
      //assigning the chars of the key
      int first = (int)tempKey[i-1];
      int second = (int)tempKey[i];
      //printf("%c , %c\n",tempKey[i-1],tempKey[i]);
      
      //comapring the size of the ascii values
      if(first > second){        
        tempKey = swap(i-1,i,tempKey,kl);
        order = swap(i-1,i,order,kl);
        swapped = true;
      }
    }
    //printf("tempKey %s\n",tempKey);
    //swapped = false;
  }
  strcopy_subStr(sortedKey, tempKey, kl);
  //printf("sortedkey: %s, %s\n",sortedKey, order);

  char *temp_row = (char*)malloc(sizeof(char)*kl);
  for(int i =0; i< kl+1; i++){
    //need to end the string properly with '\0' not auctomatic
    temp_row[i] = '\0'; 
  }
  while( before!= NULL){ 
    //printf("before row: %s\n", before->value);
    for(int i=0; i<kl; i++){
      //convert the ascii value from teh order to an int
      temp_row[i] = before->value[order[i]-48]; //0 int = 48 ascii
    }
    //printf("temp_row: %s\n",temp_row);
    char* row = (char*)malloc(sizeof(char)*kl);
    strcopy_subStr(row,temp_row,kl);
    //adds the new row to the result list
    after = append_Str(row,after);
    before = before->next;
  } 
  return after;
}

//displays the text from the cypher list back as one long list
char* getCypherText(struct STRList *cypherList, int kl, int messageLen){
  char* cypherText = (char*)malloc(sizeof(char)*messageLen);
  char* ptr = cypherText;
  //dont want to include the key
  cypherList = cypherList->next;
  while(cypherList != NULL){
    for(int i=0; i<kl; i++){
      *ptr = cypherList->value[i];
      ptr++;
    }
    cypherList = cypherList->next;
  }
  return cypherText;
}



int main(void) {
  char* message_file = "text.txt";
  char* message = readfile(message_file);

  char* key = "LOVELACE";
  int key_len = getLen_Str(key);
  int message_len = getLen_Str(message);

  struct STRList *grid_before = NULL;
  struct STRList *grid_after = NULL;

  grid_before = populate(key, key_len, message, grid_before);
  //printLinkedList_Str(grid_before,0);
  grid_after = ct_cypher(key_len,grid_before, grid_after);
  //printLinkedList_Str(grid_after,0);
  char* cypherText = getCypherText(grid_after,key_len, message_len);
  printf("%s",cypherText);

  return 0;

}
