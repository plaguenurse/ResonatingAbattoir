#ifndef __MARKOVSUPPLIES_H
#define __MARKOVSUPPLIES_H

typedef struct markovNode
{
	char* value;
	struct markovNode ** list;
	int * weightList;
	int listLength;
	int listSize;//total size of list weights
} markovNode;

typedef struct markovHashTable
{
	int size;
	int capacity;
	markovNode ** table;
} markovHashTable;

typedef struct markovChain
{
	markovHashTable * lookupTable;
	markovNode * root;
} markovChain;

unsigned int hashThatHash(char* string);
void rehashTable(markovHashTable* table, int size);
markovHashTable* initializeTable(markovHashTable* table);
markovHashTable* addValueHash(markovHashTable* table, markovNode* value);

markovNode * findLink(markovChain * chain, char * value);
markovNode * addNode(markovChain * chain, markovNode * lastNode, char * value);
char* makeString(markovChain * chain); // markov chain root has no data, just all initial branches to random from

void freeChain(markovChain* chain);
void freeNode(markovNode* node);

markovChain * makeChain(void);
#endif
