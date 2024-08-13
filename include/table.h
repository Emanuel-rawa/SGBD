#ifndef SGBD_TABLE_H
#define SGBD_TABLE_H

#include "database.h"
#include <stdbool.h>

void createTableLogic(Table newTable);
void deleteTableLogic(char* tableName);
void createTable();
void listTables();
void listTableData(char* tableName);

#endif
