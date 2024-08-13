#include "database.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Table tables[MAX_TABLES];
extern int numTables;


bool convertStringToCell(const char *string, Cell *cell, DataType columnType) {
    if (string == NULL || cell == NULL) {
        fprintf(stderr, "Erro: Ponteiro nulo\n");
        return false;
    }
    memset(cell, 0, sizeof(Cell));

    switch (columnType) {
        case INT:
            cell->value.intValue = atoi(string);
            cell->type = INT;
            break;
        case FLOAT:
            cell->value.floatValue = atof(string);
            cell->type = FLOAT;
            break;
        case DOUBLE:
            cell->value.doubleValue = atof(string);
            cell->type = DOUBLE;
            break;
        case CHAR:
            cell->value.charValue = string[0];
            cell->type = CHAR;
            break;
        case STRING:
            strcpy(cell->value.stringValue, string);
            cell->value.stringValue[MAX_CELL_LENGTH - 1] = '\0';
            cell->type = STRING;
            break;
        default:
            cell->type = EMPTY;
            cell->value.emptyValue = '\0';
    }

    return true;
}

char *processRows(char *line, Table *table) {
    for (unsigned int i = 0; i < table->numRows; i++) {
        if (line == NULL) {
            fprintf(stderr, "Erro: Número de linhas menor que o esperado\n");
            return NULL;
        }

        char restOfLine[MAX_LINE_LENGTH];
        sscanf(line, "Linha %*d: %[^\n]", restOfLine);

        char *cellStart = restOfLine;
        for (unsigned int j = 0; j < table->numColumns; j++) {
            char cellValue[MAX_CELL_LENGTH] = {0};
            int charsRead;

            int result =
                    sscanf(cellStart, "\"%[^\"]\"%*[,]%n", cellValue, &charsRead);

            if (result == 1) {
                convertStringToCell(cellValue, &table->rows[i].cells[j],
                                    table->columns[j].type);
                cellStart += charsRead + 1;
            } else {
                initCellAsEmpty(&table->rows[i].cells[j]);
            }

            if (j == 0) {
                table->rows[i].id = atoi(cellValue);
            }
        }

        line = strchr(line, '\n');
        if (line != NULL)
            line++;
    }
    return line;
}

char *processTable(char *line) {
    char tableName[MAX_NAME_LENGTH];
    unsigned int numColumns, numRows;

    sscanf(line,"Tabela %*d: \"%49[^\"]\", Quantidade de colunas: %u, Quantidade de linhas: %u", tableName, &numColumns, &numRows);

    Table newTable = {0};
    strcpy(newTable.name, tableName);
    newTable.numColumns = numColumns;
    newTable.numRows = numRows;

    line = strchr(line, '\n');
    if (line != NULL)
        line++;

    for (unsigned int i = 0; i < numColumns; i++) {
        if (line == NULL)
            break;

        char columnName[MAX_NAME_LENGTH];
        unsigned int columnType;

        char *endOfLine = strchr(line, '\n');
        if (endOfLine != NULL)
            *endOfLine = '\0';

        sscanf(line, "Coluna %*d: \"%49[^\"]\", Tipo: %u", columnName, &columnType);
        strcpy(newTable.columns[i].name, columnName);
        newTable.columns[i].type = columnType;

        if (endOfLine != NULL)
            line = endOfLine + 1;
    }

    line = processRows(line, &newTable);
    tables[numTables++] = newTable;

    if (line == NULL) return NULL;
    char *startOfNextTable = strstr(line, "Tabela");

    return startOfNextTable;
}

bool isValidFileName(char* filename) {
    char invalidChars[] = "<>:\"/\\|?*"; // Caracteres inválidos para nomes de arquivos na maioria dos sistemas de arquivos
    for (int i = 0; i < strlen(filename); i++) {
        if (strchr(invalidChars, filename[i]) != NULL) {
            return false; // O nome do arquivo contém um caractere inválido
        }
    }
    return true; // O nome do arquivo é válido
}
