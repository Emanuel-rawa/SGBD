#include "database.h"
#include "utilsFileOperations.h"
#include <string.h>

Table tables[MAX_TABLES] = {0};
int numTables = 0;

void initDatabase() {
    for (int i = 0; i < MAX_TABLES; i++) {
        memset(tables[i].name, 0, sizeof(tables[i].name));
        tables[i].numColumns = 0;
        tables[i].idColumn = 0;
        tables[i].numRows = 0;
        for (int j = 0; j < MAX_COLUMNS; j++) {
            memset(tables[i].columns[j].name, 0, sizeof(tables[i].columns[j].name));
            tables[i].columns[j].type = EMPTY;
        }
        for (int j = 0; j < MAX_ROWS; j++) {
            tables[i].rows[j].id = 0;
            for (int k = 0; k < MAX_COLUMNS; k++) {
                initCellAsEmpty(&tables[i].rows[j].cells[k]);
            }
        }
    }
    numTables = 0;
}

void initCellAsEmpty(Cell *cell) {
    if (cell == NULL) {
        fprintf(stderr, "Erro: Ponteiro nulo para initCellAsEmpty\n");
        return;
    }

    cell->type = EMPTY;
    cell->value.emptyValue = '\0';
}
