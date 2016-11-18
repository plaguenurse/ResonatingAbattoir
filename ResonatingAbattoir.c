#include <stdio.h>
#include <string.h>
#include "MarkovSupplies.h"


int main (void)
{
	//Just some test cases
	markovNode * currPoint = NULL;
	markovChain* chain = makeChain();
	fprintf(stderr,"Adding...\n");
	currPoint = addNode(chain,NULL,"I");
	fprintf(stderr,"Adding...\n");
	currPoint = addNode(chain,currPoint,"am");
	fprintf(stderr,"Adding...\n");
	currPoint = addNode(chain,currPoint,"still");
	fprintf(stderr,"Adding...\n");
	currPoint = addNode(chain,currPoint,"working");
	fprintf(stderr,"Adding...\n");
	currPoint = addNode(chain,currPoint,"on");
	fprintf(stderr,"Adding...\n");
	currPoint = addNode(chain,currPoint,"this");
 
	return 0;
}
