// Implements a dictionary's functionality
// This implementation uses a Trie

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

#include "dictionary.h"

const int numLetters = 128;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;


typedef struct trieNode{

    bool isWord;
    struct trieNode *letters[numLetters];

} trieNode;

void insert(char *word, unsigned int hash);
void uppercase(char* upper, const char *word);
void deleteTrieNode(trieNode *node);


// Hash table
hashNode *hashList;
trieNode *words[numLetters] = {NULL};
unsigned int dictionarySize = 0;



// Returns true if word is in dictionary else false
bool check(const char *word)
{
    //printf("check: %s\n",word);

    // Determine if possessives exist -- Removing this as the specification indicates otherwise
    int len = strlen(word);
    // if(word[len-2] == '\'' && word[len-2] == 's'){
    //     // do nothing
    // } else {
    //     // shorten the length of upper
    //     len = len - 2;
    // }

    // look for the word
    trieNode *tmp = NULL;
    char upper[len];
    uppercase(upper,word);

    tmp = words[(int) upper[0]];

    if(tmp == NULL){
        //printf("check: returning isWord(%s) is 0\n",upper);
        return false;
    }

    // Iterate through words checking for the next letter.
    for(int i=1; i<len; i++){
        //printf("check: at letter - %c\n",upper[i]);
        tmp = tmp->letters[(int) upper[i]];
        // Return false when the letter doesn't exist
        if (tmp == NULL){
            //printf("check: returning isWord(%s) is 0\n",upper);
            return false;
        }
    }

    //printf("check: returning isWord(%s) is %d\n",upper, tmp->isWord);
    return tmp->isWord;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open file for reading.
    FILE *dict = fopen(dictionary, "r");
    if(dict == NULL){
        fclose(dict);
        //printf("Could not open dictionary");
        return false;
    }

    //initialize variables.
    char line[LENGTH];
    int read = 0;
    unsigned int h;

    read = fscanf(dict, "%s", line);
    char upper[strlen(line)];

    while(read > 0 ){

        uppercase(upper,line);
        //printf("here %s\n",line);
        h = (int) line[0];

        //printf("load: %s\n",upper);
        insert(upper,h);

        read = fscanf(dict, "%s", line);

    }

    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dictionarySize;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    //printf("unload: starting...\n");
    trieNode *tmp = NULL;
    trieNode *last = NULL;


    for(int i=0; i<numLetters; i++){
        deleteTrieNode(words[i]);
    }

    return true;
}

// Insert Node into Hash Map
void insert(char *word, unsigned int hash){
    //printf("Insert: %s\n",word);

    trieNode *tmp = NULL;

    if(word == NULL){
        return;
    }

    // Create Node in hash table if one doesn't exist
    if( words[hash] == NULL){
        //printf("Inserting new trieNode at %u\n",hash);
        trieNode *newNode = malloc(sizeof(trieNode));
        if(newNode == NULL){
            return;
        }
        words[hash] = newNode;
    }

    // Add word to Trie
    tmp = words[hash];
    int len = strlen(word);
    for(int i=1; i<len; i++){
        //printf("Letter: %c\n",word[i]);
        if(tmp->letters[(int) word[i]] == NULL){
            //printf("Malloc at i: %i\n",(int)word[i]);
            tmp->letters[(int)word[i]] = malloc(sizeof(trieNode));
        }
        tmp = tmp->letters[(int)word[i]];
    }
    tmp->isWord = true;

    // Increment size counter
    dictionarySize++;

}

//upper case letter
void uppercase(char upper[], const char *word){
    int len = strlen(word);

    for(int i=0; i<len; i++){
        upper[i] = toupper(word[i]);
    }

}

void deleteTrieNode(trieNode *node){
    // recursively delete trieNodes
    // 1. delete all of the trieNodes in the letters array
    // 2. delete existing node
    //printf("deleteTrieNode: starting...\n");

    // if the node is NULL nothing to delete
    if(node==NULL){
        return;
    }

    trieNode* tmp;
    for(int i=0; i<numLetters; i++){
        tmp = node->letters[i];
        //printf("deleteTrieNode: at %i\n",i);
        if(tmp != NULL){
            //printf("deleteTrieNode: deleting...%i\n",i);
            deleteTrieNode(tmp);
        }
    }
    //printf("deleteTrieNode: deleted self\n");
    if(node->isWord){
        dictionarySize--;
    }
    free(node);
    return;
}