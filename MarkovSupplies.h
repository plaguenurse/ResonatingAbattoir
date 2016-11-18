#ifndef __MARKOVSUPPLIES_H
#define __MARKOVSUPPLIES_H

typedef struct markovNode
{
	char* value;
	struct MarkovNode * list;
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

int hashThatHash(char* string);
void rehashTable(markovHashTable* table, int size);
markovHashTable* initializeTable(markovHashTable* table);
markovHashTable* addValueHash(markovHashTable* table, markovNode* value);

markovNode * findLink(markovChain * chain, char * value);
void addNode(markovChain * chain, markovNode * node, char * value);
char* makeString(markovChain * chain); // markov chain root has no data, just all initial branches to random from

markovChain * makeChain(void);
#endif
