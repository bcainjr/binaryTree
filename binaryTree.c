/*
* Author: Bruce Cain
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int (*pSorter)(char *left, char *right);

typedef struct Node
{
    char *word;
    struct Node *left;
    struct Node *right;
} Node;

void printListForward(Node *root);
void addWord(Node **root, char *word);
void sort(Node *node, Node *nextNode, pSorter *sorter);
int asciiSort(char *left, char *right);
int lengthSort(char *left, char *right);

int main(int argc, char *argv[])
{
    
    FILE *pFile;
    Node *root = NULL;
    char *line = NULL, *word, *nextWord, sep[11] = " \t\n\v\f\r";
    size_t size = 0;

    pFile = fopen(argv[1], "r");

    while(getline(&line, &size, pFile) != -1)
    {
        for(word = strtok_r(line, sep, &nextWord); word;
                word = strtok_r(NULL, sep, &nextWord))
        {
            addWord(&root, word);
        }
        
    }

    fclose(pFile);


    printListForward(root);

    return 0;
}

void printListForward(Node *root)
{
    if(root->left)
    {
        printListForward(root->left);
    }

    printf("%s\n", root->word);
    
    if(root->right)
    {
        printListForward(root->right);
    }
}

void addWord(Node **root, char *word)
{
    Node *newNode = calloc(1, sizeof(**root));
    static pSorter sorter = lengthSort;

    if(!newNode)
    {
        printf("New node didn't malloc\n");
        exit(1);
    }

    newNode->word = calloc(strlen(word) + 1, sizeof(char));

    if(!(newNode->word))
    {
        printf("New node word didn't malloc\n");
        exit(1);
    }

    strcpy(newNode->word, word);
    newNode->left = NULL;
    newNode->right = NULL;

    if(!(*root))
    {
        *root = newNode;
        return;
    }


    sort(*root, newNode, &sorter);
}

void sort(Node *node, Node *nextNode, pSorter *sorter)
{
    if(!nextNode->word)
    {
        printf("next word is null\n");
    }

    int compareResult = (*sorter)(node->word, nextNode->word);


    if(compareResult == 0)
    {
        if(!(asciiSort(node->word, nextNode->word)))
        {
            free(nextNode);
            return;
        }
    }
    
    if(compareResult >= 0)
    {
        if(node->left)
        {
            sort(node->left, nextNode, sorter);
        }
        else
        {
            node->left = nextNode;
        }
    }
    else if(compareResult < 0)
    {
        if(node->right)
        {
            sort(node->right, nextNode, sorter);
        }
        else
        {
            node->right = nextNode;
        }
    }

}

int asciiSort(char *left, char *right)
{
    return strcmp(left, right);
}

int lengthSort(char *left, char *right)
{
    return strlen(left) - strlen(right);
}
