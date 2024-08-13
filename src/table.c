#include "database.h"
#include "utilsTable.h"
#include "userInteraction.h"
#include <stdio.h>
#include <string.h>


extern Table tables[MAX_TABLES];
extern int numTables;

void createTableLogic(Table newTable) {
    int previousNumTables = numTables;
    if (numTables == MAX_TABLES) {
        printf("Erro: O número máximo de tabelas foi atingido.\n");
        return;
    }
    tables[numTables] = newTable;
    numTables++;
    if (previousNumTables < numTables) {
        printf("A tabela '%s' foi criada com sucesso.\n", newTable.name);
    } else {
        printf("A tabela '%s' não foi criada.\n", newTable.name);
    }
}

void createTable() {
    Table newTable = getNewTableData();
    if (newTable.numColumns == 0) {
        return; // O usuário decidiu sair
    }
    createTableLogic(newTable);
}

void deleteTableLogic(char* tableName) {
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("Erro: A tabela '%s' não foi encontrada.\n", tableName);
        return;
    }

    for (int i = tableIndex; i < numTables - 1; i++) {
        tables[i] = tables[i + 1];
    }

    numTables--;

    printf("A tabela '%s' foi deletada com sucesso.\n", tableName);
}

void listTables() {
    if (numTables == 0) {
        printf("Não há tabelas no banco de dados.\n");
        return;
    }
    for (int i = 0; i < numTables; i++) {
        printf("Tabela %d: %s\n", i + 1, tables[i].name);
    }
}

void listTableData(char* tableName) {
    int tableIndex = findTableIndex(tableName);

    Table* table = &tables[tableIndex];

    // Calculate maximum width for each column
    int columnWidths[MAX_COLUMNS];
    for (int i = 0; i < table->numColumns; i++) {
        columnWidths[i] = strlen(table->columns[i].name);
        for (int j = 0; j < table->numRows; j++) {
            int cellWidth;
            Cell cell = table->rows[j].cells[i];
            switch (cell.type) {
                case INT:
                    cellWidth = snprintf(NULL, 0, "%d", cell.value.intValue);
                    break;
                case FLOAT:
                    cellWidth = snprintf(NULL, 0, "%.2f", cell.value.floatValue);
                    break;
                case DOUBLE:
                    cellWidth = snprintf(NULL, 0, "%.2lf", cell.value.doubleValue);
                    break;
                case CHAR:
                    cellWidth = snprintf(NULL, 0, "%c", cell.value.charValue);
                    break;
                case STRING:
                    cellWidth = strlen(cell.value.stringValue);
                    break;
                case EMPTY:
                    cellWidth = 1;
                    break;
            }
            if (cellWidth > columnWidths[i]) {
                columnWidths[i] = cellWidth;
            }
        }
        columnWidths[i] += 2; // Add extra spaces for readability
    }

    // Calculate total width
    int totalWidth = 0;
    for (int i = 0; i < table->numColumns; i++) {
        totalWidth += columnWidths[i] + 3; // for data, "| ", and " "
    }
    totalWidth += 1; // for the last "|"

    // Print line above column headers
    for (int i = 0; i < totalWidth; i++) {
        printf("-");
    }
    printf("\n");

    // Print column headers
    for (int i = 0; i < table->numColumns; i++) {
        printf("| %-*s ", columnWidths[i], table->columns[i].name);
    }
    printf("|\n");

    // Print line below column headers
    for (int i = 0; i < totalWidth; i++) {
        printf("-");
    }
    printf("\n");

    // Print each row
    for (int i = 0; i < table->numRows; i++) {
        for (int j = 0; j < table->numColumns; j++) {
            Cell cell = table->rows[i].cells[j];
            printf("| ");
            switch (cell.type) {
                case INT:
                    printf("%-*d ", columnWidths[j], cell.value.intValue);
                    break;
                case FLOAT:
                    printf("%-*.*f ", columnWidths[j], 2, cell.value.floatValue);
                    break;
                case DOUBLE:
                    printf("%-*.*lf ", columnWidths[j], 2, cell.value.doubleValue);
                    break;
                case CHAR:
                    printf("%-*c ", columnWidths[j], cell.value.charValue);
                    break;
                case STRING:
                    printf("%-*s ", columnWidths[j], cell.value.stringValue);
                    break;
                case EMPTY:
                    printf("%-*s ", columnWidths[j], " ");
                    break;
            }
        }
        printf("|\n");
    }

    // Print line at the end of the table
    for (int i = 0; i < totalWidth; i++) {
        printf("-");
    }
    printf("\n");
}
