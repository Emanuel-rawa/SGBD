#ifndef SGBD_UTILSTABLE_H
#define SGBD_UTILSTABLE_H

#include "database.h"
#include <ctype.h>

int isValidName(char* name);
char* readNewTableName();
char* readTableName();
char* readColumnName(Table* table);
DataType readColumnType();
const char* printColumnType(DataType columnType);
int findTableIndex(char* tableName);
int findColumnIndex(Table* table, char* columnName);
int valueExistsInColumn(Table table, Column column, Cell cells[], int numRows, void* value, int* foundRow);
void valueNotFound(void *value, Table table, Column column);


#endif
