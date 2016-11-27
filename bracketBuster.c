#include "bracketBuster.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

xmlData * parseString(char* string)
{
	xmlData* xml;
	char buffer[512]={0},*tempLoc, *endPoint;
	int temp;
	if(string==NULL)
		return NULL;
	xml = malloc(sizeof(xmlData));
	sscanf(string,"<%s",buffer);
	temp = strlen(buffer);
	if(buffer[temp-1]=='>')
		buffer[strlen(buffer)-1]=0;
	xml->type = malloc(sizeof(char)*(temp+1));
	strcpy(xml->type,buffer);
	
	tempLoc = strchr(string,'>');
	*strrchr(string,'<') = 0;
	*tempLoc = 0;

	xml->value = malloc(sizeof(char)*strlen(tempLoc+1)+1);
	strcpy(xml->value,tempLoc+1);
	xml->tagInfo = malloc(sizeof(char)*(strlen(string + temp+1)+1));
	strcpy(xml->tagInfo,string + temp + 1);
	return xml;
}

xmlData * getNextTagFile(FILE * file, char* tag)
{
	char* buffer,subBuffer[512]={0},currValue = 0;
	xmlData* xml = NULL;
	int size = 0,capacity = 512,found = 0;
	int polarity = 0;
	buffer = malloc(sizeof(char)*capacity);
	while(!found)
	{
		size = 0;
		while(currValue!='<' && !feof(file))
			currValue=fgetc(file);
		if(feof(file))
		{
			free(buffer);
			return NULL;
		}
		fscanf(file,"%s",subBuffer);
		if(strstr(subBuffer,tag)!=NULL)
		{
			if(subBuffer[0]!='<')
			{
				buffer[0] = '<';
				size++;
			}
			buffer[size]=0;
			strcat(buffer,subBuffer);
			size+=strlen(subBuffer);
			while(found==0 && !feof(file))
			{
				currValue=fgetc(file);
				
				if(size>=capacity-1)
				{
					capacity*=2;
					buffer = realloc(buffer,sizeof(char)*capacity);
				}
				if(!iscntrl(currValue))
				{
					buffer[size] = currValue;
					size++;
				}
				if(currValue=='/')
				{
					fscanf(file,"%s",subBuffer);
					if(strstr(subBuffer,tag)!=NULL)
					{
						found = 1;
					}
					buffer[size]=0;
					size+=strlen(subBuffer);
					if(size>=capacity-1)
					{
						capacity*=2;
						buffer = realloc(buffer,sizeof(char)*capacity);
					}
					strcat(buffer,subBuffer);

				}
			}

		}
	}
	xml = parseString(buffer);
	free(buffer);
	return xml;
}
