#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MarkovSupplies.h"


int main (void)
{
	//Just some test cases
	markovNode * currPoint = NULL;
	FILE* fp = fopen("alice.txt","r");
	char * string,buffer[512]={0},*subBuffer;
	markovChain* chain = makeChain();
	int count = 0;
	srand(time(NULL));
	while(!feof(fp))
	{
		fgets(buffer,511,fp);
		if(strlen(buffer)>1)
			subBuffer = strtok(buffer," \r\n");
		else
			subBuffer=NULL;
		while(subBuffer!=NULL)
		{
			if(strlen(subBuffer)>0)
			{
				if(findLink(chain,subBuffer)==NULL)
					count++;
				currPoint= addNode(chain,currPoint,subBuffer);
			}
			subBuffer=strtok(NULL," \r\n");
		}
		currPoint = NULL;
	}
	
	string = makeString(chain,128);
	printf("%s\n",string);
	free(string);
	string = makeString(chain,128);
	printf("%s\n",string);
	free(string);
	string = makeString(chain,128);
	printf("%s\n",string);
	free(string);
	fclose(fp);
	fprintf(stderr,"Total of %d unique strings\n",count);
	freeChain(chain);
	return 0;
}
