#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MarkovSupplies.h"
#define PRIMEBABY 4099

unsigned int hashThatHash(char* string)
{
	unsigned int i =0, hash = 41;
	while(string[i]!=0)
	{
		hash*=PRIMEBABY;
		hash^=string[i];
		i++;
	}
	return hash;
}
void rehashTable(markovHashTable* table, int size)
{
	int i = 0,tempSize = table->size;
	markovNode ** temp, ** temp2;
	table->size = size;
	temp2 = calloc(size,sizeof(markovNode*));
	temp = table->table;
	table->table = temp2;
	for(i=0;i<tempSize;i++)
	{
		if(table->table[i]!=NULL)
			addValueHash(table,temp[i]);
	}
	free(temp);
}
markovHashTable* initializeTable(markovHashTable* table)
{
	if(table == NULL)
	{
		table = malloc(sizeof(markovHashTable));
		table->size = 151;
		table->capacity=0;
	}
	table->table = calloc(table->size,sizeof(markovNode*));
	return table;
}
void freeNode(markovNode* node)
{
	free(node->list);
	free(node->value);
	free(node->weightList);
	free(node);
}
void freeChain(markovChain* chain)
{
	int i = 0,speedCheck = 0;
	for(i=0;i<chain->lookupTable->size && speedCheck<chain->lookupTable->capacity;i++)
	{
		if(chain->lookupTable->table[i]!=NULL)
		{
			speedCheck++;
			freeNode(chain->lookupTable->table[i]);
			chain->lookupTable->table[i] = NULL;
		}
	}
	free(chain->lookupTable->table);
	free(chain->lookupTable);
	freeNode(chain->root);
	free(chain);
}


markovHashTable * addValueHash(markovHashTable* table, markovNode * value)
{
	int i=0;
	unsigned int hash = hashThatHash(value->value);
	if(table==NULL || table->table ==NULL)
	{
		table = initializeTable(table);
	}
	fprintf(stderr,"Adding %s to table...\n",value->value);
	if(table->capacity+1>table->size/2)
	{
		rehashTable(table,table->size*2+1);
	}
	while(table->table[(hash+i)%table->size]!=NULL)
		i++;
	table->table[(hash+i)%table->size]=value;
	table->capacity++;
	return table;
}


markovNode * findLink(markovChain * chain, char * value)
{
	fprintf(stderr,"Getting Keyfor %s\n",value);
	unsigned int key = hashThatHash(value);
	fprintf(stderr,"Key is %u\n",key);
	while(chain->lookupTable->table[key%chain->lookupTable->size] !=NULL)
	{
		fprintf(stderr,"Checking node %p\n",chain->lookupTable->table[key%chain->lookupTable->size]);
		if(strcmp(value,chain->lookupTable->table[key%chain->lookupTable->size]->value)==0)
			return chain->lookupTable->table[key%chain->lookupTable->size];
		else
			key++;
	}
	return NULL;
}


markovNode * addNode(markovChain* chain, markovNode* lastNode, char * value)
{
	markovNode* node = findLink(chain,value);
	int i = 0;
	if(lastNode==NULL)
	{
		lastNode = chain->root;
	}
	if(node==NULL)
	{
		node = malloc(sizeof(markovNode));
		node->value = malloc(sizeof(char)*(strlen(value)+1));
		strcpy(node->value,value);
		node->list = NULL;
		node->weightList = NULL;
		node->listLength = 0;
		node->listSize =0;
		lastNode->listLength++;
		lastNode->listSize++;
		lastNode->list=realloc(lastNode->list,(lastNode->listLength)*sizeof(markovNode*));
		lastNode->weightList=realloc(lastNode->weightList,(lastNode->listLength)*sizeof(int));
		lastNode->list[lastNode->listLength-1]=node;
		lastNode->weightList[lastNode->listLength-1]++;
		addValueHash(chain->lookupTable,node);
	}
	else
	{
		for(i=0;i<lastNode->listLength;i++)
		{
			if(strcmp(lastNode->list[i]->value,value)==0)
			{
				lastNode->weightList[i]++;
				return lastNode->list[i]; 
			}
		}
		lastNode->listLength++;
		lastNode->listSize++;
		lastNode->list=realloc(lastNode->list,(lastNode->listLength)*sizeof(markovNode*));
		lastNode->weightList=realloc(lastNode->weightList,(lastNode->listLength)*sizeof(int));
		lastNode->list[lastNode->listLength-1]=node;
		lastNode->weightList[lastNode->listLength-1]++;
		//else add it
	}
	return node;
}

char* makeString(markovChain * chain)
{
	int size = 151,capacity=0,position = 0, index = 0;
	char* returnString = NULL, *temp = NULL;
	markovNode * currNode = NULL;
	if(chain==NULL || chain->root==NULL)
		return returnString;
	currNode = chain->root;
	srand(time(NULL));
	returnString = calloc(size,sizeof(char));
	while(currNode->listSize>0)
	{
		index = 0;
		position = rand()%currNode->listSize;
		fprintf(stderr,"Node with %d children position %d\n",currNode->listSize,position);
		while(position>0)
		{
			position-=currNode->weightList[index];
			if(position>=0)
				index++;
		}
		fprintf(stderr,"Node index %d from position %d\n",index,position);
		if(currNode->list[index]->value!=NULL)
			capacity+=strlen(currNode->list[index]->value)+1;
		if(capacity>=size)
		{
			size*=2;
			size+=1;
			returnString = realloc(returnString,sizeof(char)*size);
		}
		strcat(returnString,currNode->list[index]->value);

		currNode=currNode->list[index];
		if(currNode->listSize>0)
			strcat(returnString," ");
	}
	return returnString;
}

markovChain * makeChain(void)
{
	markovChain * chain = malloc(sizeof(markovChain));
	chain->lookupTable = initializeTable(NULL);
	chain->root = malloc(sizeof(markovNode));
	chain->root->value = NULL;
	chain->root->list = NULL;
	chain->root->weightList = NULL;
	chain->root->listLength = 0;
	chain->root->listSize = 0;
	return chain;
}
