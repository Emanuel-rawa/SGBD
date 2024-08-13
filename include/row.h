#ifndef SGBD_ROW_H
#define SGBD_ROW_H

#include "database.h"

Cell readCell();
int findRowIndex(Table* table, unsigned int rowId);
int rowExists(char* tableName, unsigned int rowId);
void createRowLogic(Row newRow, char* tableName);
void deleteRow(char* tableName, unsigned int rowId);
void searchValue();


#endif
