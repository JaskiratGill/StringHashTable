#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node {
    char *key;
    unsigned long int value;
    struct node* next;
}node;

struct wc {
    node**HashTable;
    unsigned long int sizeTable;
};

unsigned long int hashFunction (char* word, unsigned long int tableSize){

    unsigned long int value=0;
    unsigned long int i = 0;
    unsigned long int word_len = strlen(word);

    for (; i < word_len ; ++i){
        value= value * 37 + word[i];
    }
    value = value % tableSize;
    //printf("HashIndex: %lu\n", value);
    return value;
}

void printString(char*string){
    unsigned long int i=0;
    for (;i<strlen(string);i++){
        printf("%c", string[i]);
    }
}


node* createPair(char*word){

    node* entry = malloc(sizeof(node) *1);
    entry -> key = malloc(strlen(word) +1);
    entry -> value = 1;

    strcpy(entry->key, word);
    entry -> next = NULL;
    return entry;

}

void wc_destroy(struct wc *wc){

    unsigned long int i=0;
    for (; i< wc->sizeTable; i++){
        node* current= wc -> HashTable[i];
        node* tmp= NULL;

        while (current != NULL){
            tmp = current;
            current = current -> next;
            free(tmp->key);
            tmp -> next = NULL;
            free(tmp);
        }
    }

    free (wc -> HashTable);
    free (wc);
    return;
}

void wc_output(struct wc *wc){
    unsigned long int i=0;
    for (; i< wc->sizeTable; i++){
        node* current= wc -> HashTable[i];
        while (current != NULL){
            printf(current -> key);
            printf(":%lu\n", current -> value);
            current = current ->next;
        }
    }
}

struct wc *wc_init(char *word_array,long size){

    //clean up the array

    char*word_array_new = malloc(sizeof(char) * (size+1));

    int iterator=0;

    for (;iterator < size; iterator++){

        if (word_array[iterator] == '\n' || word_array[iterator] == '\r' || word_array[iterator] == '\t')
            word_array_new[iterator] = ' ';
        else
            word_array_new[iterator] = word_array[iterator];
    }
    word_array_new[iterator] = '\0';

    unsigned long int size_new = strlen(word_array);
    //printf("new size %lu\n", size_new);


    unsigned long int tableSize= size_new/3;
    struct wc *wc = malloc(sizeof(struct wc));
    wc -> sizeTable = tableSize;
    wc -> HashTable = malloc(sizeof(node*) *(tableSize));

    unsigned long int t = 0;
    for (; t < (tableSize) ; t++){
        wc -> HashTable[t] = NULL;
        //printf("NULLED %lu\n", t);
    }

    //parse this array into separate words
    unsigned long int i = 0;
    char*word = malloc(sizeof(char));
    unsigned long int j = 0;
    for (; i <= size_new; i++){

        if ((isspace(word_array[i])) != 0 || i == size_new)  {
            word = realloc(word,(j+1)*sizeof(char));
            word[j] = '\0';
            j=0;

            //printString(word);
            if (strlen(word) != 0){
                //Final word recieved, store into hash table

                unsigned long int hashIndex = hashFunction(word, tableSize);

                //Insert word into hash, first need to check if the word already exists
                //If it doesnt Exist, declare it
                if (wc -> HashTable[hashIndex] == NULL){
                    wc -> HashTable[hashIndex] = createPair(word);
                }

                else {
                    node* current = wc -> HashTable[hashIndex];
                    unsigned long int wordFound = 0;
                    while (current-> next != NULL){
                        if (strcmp(current-> key, word) ==0){
                            current-> value = (current -> value + 1);
                            wordFound =1;
                            break;
                        }
                        current = current -> next;
                    }

                    if (strcmp(current-> key, word) ==0 && wordFound==0){
                            current-> value = (current -> value + 1);
                            wordFound =1;
                        }

                    if(wordFound == 0){
                        current -> next = createPair(word);
                    }

                }
            }
            free(word);
            word = malloc(sizeof(char));

       }

       else{
           word = realloc(word, (j+1)*sizeof(char));
           word[j] = word_array_new[i]; 
           j++;
       }

   }

    free(word);
    free(word_array_new);
    return wc;
}
