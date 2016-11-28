#include "bracketBuster.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearSpaces(char* string)
{
	int i, j=0,size = strlen(string),inside = 0;
	char* buffer = malloc(sizeof(char)*(size+1)),currValue;
	strcpy(buffer,string);
	for(i=0;i<=size;i++)
	{
		currValue = buffer[i];
		if(currValue=='<')
		{
			inside = 1;
		}
		else if(currValue=='>')
		{
			inside = 0;
		}
		else
		{
			if(!inside)
			{
				while(isspace(currValue))
				{
					i++;
					currValue = buffer[i];
				}
			}
			if(currValue=='<')
			{
				inside = 1;
			}
			else if(currValue=='>')
			{
				inside = 0;
			}
		}
		string[j]=currValue;
		j++;
	}
	string[j]=0;
	free(buffer);
}

void freeXML(xmlData * data)
{
	free(data->value);
	free(data->type);
	free(data->tagInfo);
	free(data);
}

xmlData * parseString(char* string)
{
	xmlData* xml;
	char *buffer,*tempLoc, *endPoint;
	int temp,size=strlen(string);

	if(string==NULL)
		return NULL;
	buffer=malloc(sizeof(char)*(size+1));
	xml = malloc(sizeof(xmlData));
	sscanf(string,"%s",buffer);
	temp = strlen(buffer);
	if(buffer[temp-1]=='>')
		buffer[strlen(buffer)-1]=0;
	xml->type = malloc(sizeof(char)*(temp+1));
	strcpy(xml->type,buffer);
	tempLoc = strchr(string,'>');
	*strrchr(string,'<') = 0;
	*tempLoc = 0;

	xml->value = malloc(sizeof(char)*strlen(tempLoc+1)+2);
	strcpy(xml->value,tempLoc+1);
	xml->tagInfo = malloc(sizeof(char)*(strlen(string + temp+1)+2));
	strcpy(xml->tagInfo,string + temp + 1);
	clearSpaces(xml->value);
	xml->offset = 0;
	free(buffer);
	return xml;
}

xmlData * getNextTagString(char* string, char* tag,int offset)
{
	char*buffer,*check, *end;
	xmlData* retVal=NULL;
	
	int size= strlen(string), tagSize = strlen(tag);
	if(size<2 || string == NULL || offset>=size)
		return NULL;
	//fprintf(stderr,"%512s\n",string);
	buffer= malloc(sizeof(char)*(size+1));
	strncpy(buffer,string+offset,size);
	check=strstr(buffer,tag);
	while(check!=NULL && *(check-1)!='<')
	{
		check = strstr(check+1,tag);
	}
	if(check==NULL)
	{
		free(buffer);
		return NULL;
	}
	end=strstr(check,tag);
	while(end!=NULL && *(end-1)!='/')
	{
		end = strstr(end+1,tag);
	}
	end = strchr(end,'>');
	if(end==NULL)
	{
		free(buffer);
		return NULL;
	}
	*(end+1)=0;
	
	retVal = parseString(check-1);
	retVal->offset=offset+end-buffer;
	free(buffer);
	return retVal;
	
}

xmlData * getNextTagFile(FILE * file, char* tag)
{
	char* buffer,subBuffer[512]={0},currValue = 0,*check = NULL;
	xmlData* xml = NULL;
	int size = 0,capacity = 512,found = 0,tagSize = strlen(tag);
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
		check=strstr(subBuffer,tag);
		if(check!=NULL &&(check[tagSize]==0 || check[tagSize]=='>') && *(check-1)=='<')
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
