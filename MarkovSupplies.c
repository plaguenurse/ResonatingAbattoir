#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "MarkovSupplies.h"
#define PRIMEBABY 4099

int hashThatHash(char* string)
{
	int i =0, hash = 41;
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
	table->table = calloc(table->capacity,sizeof(markovNode*));
	return table;
}
markovHashTable * addValueHash(markovHashTable* table, markovNode * value)
{
	int i=0, hash = hashThatHash(value->value);
	if(table==NULL || table->table ==NULL)
	{
		table = initializeTable(table);
	}
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
	int key = hashThatHash(value);
	while(chain->lookupTable->table[key%chain->lookupTable->size] !=NULL)
	{
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
		lastNode->list=realloc(lastNode->list,++(lastNode->listLength)*sizeof(markovNode*));
		lastNode->weightList=realloc(lastNode->weightList,lastNode->listLength*sizeof(int));
		
		lastNode->list[lastNode->listLength-1]=node;
		lastNode->weightList[lastNode->listLength-1]++;
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
		//see if it is in prev node
		//if so increment it
		//else add it
	}
	return node;
}

char* makeString(markovChain * chain)
{
	return NULL;
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
