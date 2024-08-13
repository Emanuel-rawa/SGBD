#ifndef SGBD_UTILSFILEOPERATIONS_H
#define SGBD_UTILSFILEOPERATIONS_H

#include "database.h"
#include <stdbool.h>
#include <stdio.h>

char *processTable(char *line);
bool convertStringToCell(const char *string, Cell *cell, DataType columnType);
char *processRows(char *line, Table *table);
bool isValidFileName(char* filename);

#endif
