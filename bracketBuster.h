#ifndef __BRACKETBUSTER_H
#define __BRACKETBUSTER_H

#include <stdio.h>

typedef struct xmlData
{
	char* value;
	char* type;
	char* tagInfo;
} xmlData;
xmlData * getNextTagFile(FILE * file, char* tag);
xmlData * getNextTagString(char* string, char* tag);

#endif
