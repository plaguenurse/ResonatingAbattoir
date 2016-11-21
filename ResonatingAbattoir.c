#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MarkovSupplies.h"


int main (void)
{
	//Just some test cases
	markovNode * currPoint = NULL;
	char * string;
	markovChain* chain = makeChain();
	currPoint = addNode(chain,NULL,"I");
	currPoint = addNode(chain,currPoint,"am");
	currPoint = addNode(chain,currPoint,"still");
	currPoint = addNode(chain,currPoint,"working");
	currPoint = addNode(chain,currPoint,"on");
	currPoint = addNode(chain,currPoint,"this");
	currPoint = addNode(chain,currPoint,"\n");
 
	currPoint = addNode(chain,NULL,"Don't");
	currPoint = addNode(chain,currPoint,"I");
	currPoint = addNode(chain,currPoint,"make");
	currPoint = addNode(chain,currPoint,"progress");
	currPoint = addNode(chain,currPoint,"on");
	currPoint = addNode(chain,currPoint,"this");
	currPoint = addNode(chain,currPoint,"though?");
	currPoint = addNode(chain,currPoint,"\n");
	currPoint = addNode(chain,NULL,"Don't");
	currPoint = addNode(chain,currPoint,"I");
	currPoint = addNode(chain,currPoint,"make");
	currPoint = addNode(chain,currPoint,"progress");
	currPoint = addNode(chain,currPoint,"on");
	currPoint = addNode(chain,currPoint,"this");
	currPoint = addNode(chain,currPoint,"though?");
	currPoint = addNode(chain,currPoint,"\n");
 
	string = makeString(chain);
	
	printf("%s",string);
	free(string);
	freeChain(chain);
	return 0;
}
