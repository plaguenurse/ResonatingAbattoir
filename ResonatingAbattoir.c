#include "bracketBuster.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MarkovSupplies.h"

typedef struct MusicalClusters
{
	int size;
	int capacity;
	markovChain ** chains;
	xmlData ** types;
	char ** typeName;
} MusicalClusters;

int getMaxDiv(char * string)
{
	int max = 0,curr = 0;
	char* currPos = strstr(string,"<duration>");
	while(currPos!=NULL)
	{
		sscanf(currPos+10,"%d",&curr);
		if(curr>max)
			max = curr;
		currPos = strstr(currPos+1,"<duration>");
	}
	return max;
}

void setMaxDiv(char * string, int max)
{
	int curr = 0;
	char* currPos = strstr(string,"<duration>");
	while(currPos!=NULL)
	{
		sscanf(currPos+10,"%d",&curr);
		if(curr>10)
		{
			max = curr;
			currPos[10]=' ';
			currPos[11]='8';
		}
		currPos = strstr(currPos+1,"<duration>");
	}
}

void clearCluster(MusicalClusters* cluster)
{
	int i;
	for(i=0;i<cluster->capacity;i++)
	{
		freeXML(cluster->types[i]);
		freeChain(cluster->chains[i]);
		free(cluster->typeName[i]);
	}
	for(i=cluster->capacity;i<cluster->size;i++)
	{
		freeChain(cluster->chains[i]);
	}
	free(cluster->chains);
	free(cluster->types);
	free(cluster->typeName);
	free(cluster);
}


char* makeMusic(markovChain * chain, int bars,int width)
{
	int size = 751,capacity=232,position = 0, count = 0;
	char* returnString = NULL;
	markovNode * currNode = NULL;
	if(chain==NULL || chain->root==NULL)
		return returnString;
	currNode = chain->root;
	
	returnString = calloc(size*sizeof(char),sizeof(char));
	
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

int main (int argc, char **argv)
{
	markovNode * currPoint = NULL;
	int size = 0, offset = 0,i,j,randList[5],q;
	int tempFileHolder,scorePartHolder;
	char* tempLoc = NULL, *endLoc = NULL,outputName[15];

	FILE* masterList,* file,* output;
	char * string,buffer[512]={0},subBuffer[512] = {0},fileName[512]={0};
	xmlData * data, *note,*temp,*part;
	MusicalClusters * clusters = malloc(sizeof(MusicalClusters));
	
	strcpy(outputName,"output000.xml");
	if(argc<=1)
		q = 1;
	else
	{
		q=atoi(argv[1]);
	}
	
	clusters->size= 20;
	clusters->capacity = 0;
	//fprintf(stderr,"TEST -- allocating cluster\n");	
	clusters->chains = malloc(sizeof(markovChain*)*clusters->size);
	clusters->typeName = calloc(clusters->size,sizeof(char*));
	clusters->types = calloc(clusters->size,sizeof(xmlData*));
	
	for(i=0;i<clusters->size;i++)
	{
		clusters->chains[i]=makeChain();
	}
	//fprintf(stderr,"TEST -- done allocating space\n");	
	
	srand(time(NULL));
	masterList = fopen("masterList.txt","r");
	
	while(!feof(masterList))
	{
		fgets(fileName,511,masterList);
		if(fileName[strlen(fileName)-1]=='\n')
			fileName[strlen(fileName)-1]=0;		
		file = fopen(fileName,"r");
		
		scorePartHolder=ftell(file);
		tempFileHolder=ftell(file);

		//get parts -- start
		
		tempFileHolder=ftell(file);
		fseek(file,scorePartHolder,SEEK_SET);
		part = getNextTagFile(file,"score-part");	
		if(part!=NULL)
		{
			temp=getNextTagString(part->value,"midi-program",0);
			//fprintf(stderr,"TEST -- checking %s\n",temp->value);
				
			for(i=0;i<clusters->capacity;i++)
			{
				if(strcmp(temp->value,clusters->typeName[i])==0)
				{
					freeXML(part);
					part = NULL;
					break;
				}
			}
			if(i>=clusters->capacity)
			{
				//fprintf(stderr,"TEST -- adding %s\n",temp->value);
				
				if(i>=clusters->size)
				{
					clusters->size*=2;
					clusters->chains = realloc(clusters->chains,sizeof(markovChain*)*clusters->size);
					clusters->typeName = realloc(clusters->typeName,clusters->size*sizeof(char*));
					clusters->types = realloc(clusters->types,clusters->size*sizeof(xmlData*));
					for(j=i;j<clusters->size;j++)
					{
						clusters->chains[j]=makeChain();
					}
				}
				clusters->typeName[i]=malloc(sizeof(char)*(strlen(temp->value)+1));
				strcpy(clusters->typeName[i],temp->value);
				clusters->types[i]=part;
				clusters->capacity++;
			}
			freeXML(temp);
		}
		scorePartHolder=ftell(file);
		fseek(file,tempFileHolder,SEEK_SET);
			
		data = getNextTagFile(file,"part");

			
			
		offset = 0;
		while(!feof(file) && data!=NULL)
		{
					
			note = getNextTagString(data->value,"note",offset);
			while(note!=NULL)
			{
				offset = note->offset;
				currPoint = addNode(clusters->chains[i],currPoint,note->value);
				freeXML(note);
				note = getNextTagString(data->value,"note",offset);
				
			}
			freeXML(data);
			data = getNextTagFile(file,"part");
		}
		currPoint = NULL;
		fclose(file);
	}
	fprintf(stderr,"TEST -- done making markov lists\n");	
	for(j=0;j<q;j++)
	{
		outputName[8]=j%10+'0';
		outputName[7]=(j/10)%10+'0';
		outputName[6]=(j/100)%10+'0';
		output = fopen(outputName,"w");

		//Standard File bobbins
		fprintf(output,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 2.0 Partwise//EN\" \"http://www.musicxml.org/dtds/partwise.dtd\">\n");
		fprintf(output,"<score-partwise version=\"2.0\">\n<movement-title>%s-%lu</movement-title>\n<identification>\n<creator type=\"composer\">PlagueNurse</creator>\n<encoding>\n","RandomRun",(unsigned long int)time(NULL));
		fprintf(output,"<software>ResonatingAbattoir</software>\n<encoding-date>2016-11-22</encoding-date>\n<software>ProxyMusic 2.0 c</software>\n</encoding>\n<source>http://github.com/</source>\n</identification>\n");
		fprintf(output,"<part-list>\n");

		for(i=0;i<3;i++)
		{
			randList[i]=rand()%clusters->capacity;
		}
		for(i=0;i<3;i++)
		{
			fprintf(output,"<%s ",clusters->types[randList[i]]->type);
			fprintf(output,"id = \"P%d\">",i+1);
			fprintf(output,"%s ",clusters->types[randList[i]]->value);
			fprintf(output,"</");
			fprintf(output,"%s>",clusters->types[randList[i]]->type);
		}

		fprintf(output,"</part-list>\n");
		for(i=0;i<3;i++)
		{
			fprintf(output,"<part id =\"P%d\">\n",i+1);
			fprintf(output,"<measure>\n");
			string = makeMusic(clusters->chains[randList[i]],250,8);
			setMaxDiv(string,8);	
			fprintf(output,"<attributes>\n<divisions>%d</divisions>\n<key><fifths>0</fifths><mode>minor</mode></key><time><beats>4</beats><beat-type>4</beat-type></time><clef><sign>F</sign><line></line></clef>\n</attributes>",8);
			
			//Make music and end file
			fprintf(output,"%s</measure>\n",string);
			fprintf(output,"</part>\n");
			free(string);
		}
		fprintf(output,"</score-partwise>");
		fflush(output);
		fclose(output);
	}
	//Close files
	clearCluster(clusters);
	fclose(masterList);

	
	return 0;
}
