#ifndef SGBD_USERINTERACTION_H
#define SGBD_USERINTERACTION_H

#include "database.h"

Table getNewTableData();
void deleteTableUserInteraction();
void createRowUserInteraction();
Row getNewRowData(char* tableName);
void deleteRowUserInteraction();
void listTableDataUserInteraction();

#endif
