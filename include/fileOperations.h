#ifndef SGBD_FILEOPERATIONS_H
#define SGBD_FILEOPERATIONS_H

#include <stdbool.h>
#include "database.h"

char* readFileName();
void processFileContent(char *fileContent);
bool saveDatabaseToFile(const char *filename);
bool loadFile(char *filename);


#endif
