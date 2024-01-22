#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define HASHTABLESIZE 20 //defining the size of the hash tabel as a constant
//-----------------------string methods--------------------------
//checks if a char is in an array, returns true is found
bool charInArray(char val, char array[],int size){
  for (int i = 0; i<size; i++){
    if(array[i] == val){
      return true;
    }
  }
  return false;
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
//returns the length of a string
int getLen_Str(char* val){
  int len = 0;
  while(*val != '\0'){
    len ++;
    val ++;
  }
  return len;
}
//converts all of the chars of a string to lowercase letters
char* toLower(char* val, int len){
  char* lower = (char*)malloc((len+1)*sizeof(char)); 
  char* start = lower;
  for(int i = 0; i<len;i++){   
    int ascii = (int) *val;
    //if a lower case letter
    if(ascii>=65 && ascii<=90){
      *lower = (char)ascii+32;
    }
    //otherwise will just copy the original char
    else{ 
      *lower = *val;
    }
    lower ++;
    val++;
  }
  return start; 
}

//---------------------------list methods----------------------------
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

//displays the contents of the linked list
void printLinkedList_Str(struct STRList *list, int index){

  if(list!= NULL){ 
    //uses the index to determine the position in the linked list so far
    //only used when printing it out, fo rth visuals
     printf("value: %s index: %d\n", list->value,index);
      index++;   
    printLinkedList_Str(list->next, index);
  }
  else{
    //when reaches the end of the list, or list is empty
  }
}


//-------------------------hash methods---------------------------
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
 //---------------------populate hash and list-------------------

//populates the list with all the words from tetx file to be redacted
struct STRList* populateList(char* fileName, struct STRList *list){
  int maxWordSize = 20;
  char tempWord[maxWordSize];
  tempWord[0] = ' '; // giving the first element an inital value
  //offset is used so that the same counter can be used for the length of a word and the length of the whole file
  int offset = 0;
  int NumOfWords = 0;
  int wordSize = 0;

  int numOfSpecials = 32;  
  char specialChars[] = {'.','~','!', '@','%','^','&','*','+','=','|','/',':',';','?','<','>',',','(',')','[',']','\"','\'','\\','#','$','%','^','&','_'}; //removed '-' so can have hypehanted words

  char lettersAndNums[] = {'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m','Q','W','E','R','T','Y','U','I','O','P','A','S','D','F','G','H','J','K','L','Z','X','C','V','B','N','M','1','2','3','4','5','6','7','8','9','0'};
  
  //------reading in from file
  int maxChar = 50000; 
  FILE *file;
  file = fopen(fileName, "r");
  bool fileEnd = false;

  int c = 0; // the while loop counter
  //looping through all the chars in the file
  while(fileEnd == false){
    char wholeChr = fgetc(file);
    //checking for the end of the file
    if(feof(file)){
      fileEnd = true;
    }else{//while NOT END OF FILE

       //if punctuation, new line or space;  store as a seperate word 
      if(charInArray(wholeChr,specialChars,numOfSpecials) == true || wholeChr == '\n' || wholeChr == '\r'|| wholeChr ==' '){
        
        //check that the word isnt empty
        if( tempWord[0] != ' '){
          //ends the current word and stores it in the list
          offset = c+1; 
          char* TempWord2 = (char*)malloc((wordSize+1)*sizeof(char)); 
          strcopy_subStr(TempWord2, tempWord, wordSize);
          //printf("TempWord2: %s\n",TempWord2); //for testing
          //appends word to the list
          list = append_Str(TempWord2,list);
          NumOfWords ++;

          //initalizing for the next word
          for(int i = 0; i<wordSize; i++){tempWord[i] = ' ';}  
          wordSize = 0; 
         
          //then appends the special char just found onto the end of the list
          offset = c+1;
          //the string is only supposed to hold one char
          char* tempChar = (char*)malloc(sizeof(char)); 
          //makes sure that only have the singualr char and not any extra
          //replaces newline with an n
           if ( wholeChr == '\n'|| wholeChr == '\r'){
            char* newLine = "n";
            strcopy_subStr(tempChar, newLine, 1);
          }
          //replaces a space with an s
          else if(wholeChr == ' '){
              char* newLine = "s";
              strcopy_subStr(tempChar, newLine, 1);

          }
          //otherwise just adds the special char
          else{
            strcopy_subStr(tempChar,&wholeChr,1); 
          }          
          list = append_Str(tempChar,list);
        }
        //if no current word that needs to be ended, but still needs to add in the spcial char to the list
        else{
            offset = c+1;        
            char* tempChar = (char*)malloc(sizeof(char)); 
            //makes sure that only have the singualr char and not any extra
            //replaces new line with n
            if ( wholeChr == '\n'|| wholeChr == '\r'){
              //*tempChar = 'n';
              char* newLine = "n";
              strcopy_subStr(tempChar, newLine, 1);
            }
            //replaces space with an s
            else if(wholeChr == ' '){
              char* newLine = "s";
              strcopy_subStr(tempChar, newLine, 1);

            }
            //otherwise adds the special char
            else{
              strcopy_subStr(tempChar,&wholeChr,1); 
            }          
            list = append_Str(tempChar,list);
          } 
      }
      //add char to current word - populate tempWord
      //BUT only if the char read in is apla numeric, dont want to read in any other strange chars
      else if(charInArray(wholeChr,lettersAndNums, 62) == true){    
        tempWord[c-offset] = wholeChr;
        //printf("temp word: %s\n", tempWord);
        wordSize ++;
      }
      
    }    
  c++;
  }
  return list;
}


//populates the hash table with redact words read from the file
//file must have the format where each new word is on a new line
int populateHash(char* fileName, struct STRList *ht[]){  
  int maxnwordsize = 15; //max length of the name

  char tempWord[maxnwordsize];
  tempWord[0] = ' '; //needto ad aned char try null
  //offset is used so that the same counter can be used for the length of a word and the length of the whole file
  int offset = 0; 
  int numOfWords = 0;
  int wordSize = 0;
  //max number of chars can read in from the file
  int maxChar = 50000; 
  FILE *file;
  file = fopen(fileName, "r");
  bool fileEnd = false;

  int c = 0; // the while loop counter
  //looping through all the chars in the file
  while(fileEnd == false){
    char wholeChr = fgetc(file);
    //checking for the end of the file
    if(feof(file)){
      fileEnd = true;
    }else{//while NOT END OF FILE 

      //end of the word
      //printf("wholeChr: %c\n",wholeChr);
      if(wholeChr == '\n'){ 
        //take into account the space between the wordSize
        offset = c+1; 
        if( tempWord[0] != ' '){ //checks its not empty
          //printf("tempWord: %s,\n",tempWord);
          //creates a list element to store the word into the has
          struct STRList wordElement;
          wordElement.value = (char*)malloc((wordSize+1)*sizeof(char));
          strcopy_subStr(wordElement.value,tempWord,wordSize);
          //printf("val: %s\n",wordElement.value);
          //adds the word to the hash
          add(&wordElement,ht);          
          numOfWords ++;

          //initalizing for the next word
          for(int i = 0; i<wordSize; i++){tempWord[i] = ' ';}  
          wordSize = 0; 
        }
      } 
      else{ 
          //appends the char from the file to the word       
          tempWord[c-offset] = wholeChr;
          wordSize ++;
      }
    } 
    c++;  
  }
  return numOfWords;
}


//------------------------------redacting the words----------------------------------------

//creates a string of stars the same length of the word to be redacted
char* createStars(int len){
  char* stars = (char*)malloc((len)*sizeof(char));
  char* ptr = stars;
  for(int i =0; i< len; i++){
    *ptr = '*';
    ptr++;
  }
  return stars;
}

//redacts the words from the list that are found in the redcats hash table
struct STRList* redact(struct STRList *result, struct STRList *whole,   struct STRList *redacts[]){
  //points to the start of the list
  struct STRList *ptr = whole;
  //while not the end of the list
  while(ptr != NULL){
    //convert the string read from the list to lower case
    char* lower = toLower(ptr->value,getLen_Str(ptr->value));
    //tests if the wrod from the list can be found in the redact has table (also checks if the lower case version of the word can be found either)    
    if(search(ptr->value,redacts) == true || search(lower,redacts) == true){ 
      //printf("redacting: %s\n",ptr->value); //for testing
      int wordLen = getLen_Str(ptr->value);
      char* newWord = createStars(wordLen);
      //replaces the word with stars
      result = append_Str(newWord,result);
    }
    else{
      result = append_Str(ptr->value,result);
    }
    ptr = ptr->next;
  }
  return result;
}

//displays the values in the list in the same format as it was read in from the file
void displayText(struct STRList *list){

  if(list!= NULL){
    if(isEqual_Str(list->value,"s")){
      printf(" ");
    }else if(isEqual_Str(list->value, "n")){
      printf("\n");
    }else{
      printf("%s",list->value);
    }
    displayText(list->next);
  }
  
}


int main(void) {
  int index = 0; 
  
  //just a pointers to the start of the list not actually the start
  struct STRList *whole_pointer = NULL;
  struct STRList *reult_pointer = NULL;
  
  //array of pointers, NOT a pointer to an array
  struct STRList *redactable_hashTable[HASHTABLESIZE]; 
  //initaliaizng the hash table
  for(int i = 0; i<HASHTABLESIZE; i++){
    redactable_hashTable[i] = NULL;
  }
  char* fileWhole = "debate.txt";
  char* fileRedact = "redactQ5.txt";

  //reads the text file and converts its contents into a list
  whole_pointer = populateList(fileWhole,whole_pointer); 
  //printLinkedList_Str(whole_pointer,0);
  //can print out the orignal text too
  //displayText(whole_pointer);
  
  //populates the hash table with the words read from the redact file
  int num_of_redacts = populateHash(fileRedact,redactable_hashTable); 
  //displays the hash
  /*
  printf("\n HASH TABLE:\n");
  for(int i = 0; i<HASHTABLESIZE; i++){
    printLinkedList_Str(redactable_hashTable[i],0);
  }
  printf("\n");
  */
  
  //redacts the names from the list, based on the owrds int the hash table
  reult_pointer = redact(reult_pointer,whole_pointer,redactable_hashTable);
  //printLinkedList_Str(reult_pointer,0);
  displayText(reult_pointer);
  
  return 0;
}
