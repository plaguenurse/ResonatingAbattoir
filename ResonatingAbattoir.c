#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MarkovSupplies.h"

char* makeMusic(markovChain * chain, int bars,int width)
{
	int size = 751,capacity=232,position = 0, count = 0;
	char* returnString = NULL;
	markovNode * currNode = NULL;
	if(chain==NULL || chain->root==NULL)
		return returnString;
	currNode = chain->root;
	
	returnString = calloc(size*sizeof(char),sizeof(char));
	
	strcat(returnString,"<measure>\n");
	strcat(returnString,"<attributes>\n<divisions>8</divisions>\n<key><fifths>0</fifths><mode>minor</mode></key><time><beats>4</beats><beat-type>4</beat-type></time><clef><sign>F</sign><line></line></clef>\n</attributes>");
	while(currNode!=NULL && currNode->listLength>0 && count<bars*width)
	{
		count++;
		position = rand()%currNode->listLength;
		if(currNode->list[position]->value!=NULL)
		{
			capacity+=strlen(currNode->list[position]->value)+3;
			capacity+=13;
		}
		if(count%width==0)
			capacity+=strlen("</measure>\n<measure>\n")+1;
		while(capacity>=size)
		{
			size*=2;
			size+=1;
			returnString = realloc(returnString,sizeof(char)*size);
		}

		strcat(returnString,"\n");
		if(count%width==0)
			strcat(returnString,"</measure>\n<measure>\n");
		strcat(returnString,"<note>");
		strcat(returnString,currNode->list[position]->value);
		strcat(returnString,"</note>");
		
		
		currNode=currNode->list[position];
		if(currNode->listLength>0)
			strcat(returnString," ");
	}
	return returnString;
}

int main (void)
{
	markovNode * currPoint = NULL;
	int size = 0;
	char* firstLoc = NULL, *endLoc = NULL;
	FILE* masterList,* file,* output;
	char * string,buffer[512]={0},subBuffer[512] = {0},fileName[512]={0};
	markovChain* chain = makeChain();
	srand(time(NULL));
	masterList = fopen("masterList.txt","r");
	while(!feof(masterList))
	{
		fgets(fileName,511,masterList);
		if(fileName[strlen(fileName)-1]=='\n')
			fileName[strlen(fileName)-1]=0;		
		file = fopen(fileName,"r");
		while(!feof(file))
		{
			fscanf(file,"%s",buffer);
			firstLoc = strstr(buffer,"<note");
			if(firstLoc!=NULL)
			{
				firstLoc = strstr(firstLoc,">");
				
				fscanf(file,"%s",buffer);
				firstLoc = strstr(buffer,">");
				while(firstLoc==NULL)
				{
					fscanf(file,"%s",buffer);
					firstLoc = strstr(buffer,">");
				}
				memmove(buffer,firstLoc+1,511);
				strncat(buffer," ",2);
			
				fscanf(file,"%s",subBuffer);
				endLoc = strstr(buffer,"</note>");
				while(endLoc==NULL)
				{
					strncat(buffer,subBuffer,511);
					strcat(buffer," ");
					fscanf(file,"%s",subBuffer);
					endLoc = strstr(subBuffer,"</note>");
				}
				currPoint = addNode(chain,currPoint,buffer);
				size++;
			}
			

		}
		currPoint = NULL;
		fclose(file);
	}
	
	output = fopen("output.xml","w");
	
	//Standard File bobbins
	fprintf(output,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 2.0 Partwise//EN\" \"http://www.musicxml.org/dtds/partwise.dtd\">\n");
	fprintf(output,"<score-partwise version=\"2.0\">\n<movement-title>%s-%u</movement-title>\n<identification>\n<creator type=\"composer\">PlagueNurse</creator>\n<encoding>\n","RandomRun",time(NULL));
	fprintf(output,"<software>ResonatingAbattoir</software>\n<encoding-date>2016-11-22</encoding-date>\n<software>ProxyMusic 2.0 c</software>\n</encoding>\n<source>http://github.com/</source>\n</identification>\n");
	fprintf(output,"<part-list>\n<score-part id=\"P1\">\n<part-name>pan flute</part-name>\n<score-instrument id=\"P1-I3\">\n<instrument-name>pan flute</instrument-name>\n</score-instrument>\n<midi-instrument id=\"P1-I3\">\n<midi-channel>1</midi-channel>\n<midi-program>76</midi-program>\n<pan>0</pan>\n</midi-instrument>\n</score-part>\n");
	fprintf(output,"</part-list>\n<part id=\"P1\">");

	//Make music and end file
	string = makeMusic(chain,250,8);
	fprintf(output,"%s</measure>\n",string);
	fprintf(output,"</part></score-partwise>");
	free(string);
	//Close files
	fclose(masterList);
	fflush(output);
	fclose(output);
	freeChain(chain);
	return 0;
}
