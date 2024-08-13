#ifndef SGBD_UTILS_H
#define SGBD_UTILS_H

#include <stdio.h>
#include <stdbool.h>

int readInteger();
float readFloat();
double readDouble();
char readChar();
void readString(char *string, int maxLength);
void removeNewLine(char *string);
int calculateStringSimilarity(const char *string1, const char *string2); // Calcula a distância de Levenshtein entre duas strings
char* suggestName(const char *name, const char *names[], int numNames);

#endif
