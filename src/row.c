#include "database.h"
#include "utilsTable.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern Table tables[MAX_TABLES];
extern int numTables;

Cell readCell(DataType columnType) {
    Cell cell;
    cell.type = columnType;

    char input[MAX_CELL_LENGTH];
    readString(input, MAX_CELL_LENGTH);

    if (strcmp(input, "E") == 0 || strcmp(input, "e") == 0) {
        cell.type = EMPTY;
        return cell;
    }

    char *endptr;
    switch (columnType) {
        case INT: {
            cell.value.intValue = strtol(input, &endptr, 10);
            if (*endptr != '\0') {
                cell.type = EMPTY;
                cell.value.intValue = -1;
            }
            break;
        }
        case FLOAT: {
            cell.value.floatValue = strtof(input, &endptr);
            if (*endptr != '\0') {
                cell.type = EMPTY;
                cell.value.intValue = -1;
            }
            break;
        }
        case DOUBLE: {
            cell.value.doubleValue = strtod(input, &endptr);
            if (*endptr != '\0') {
                cell.type = EMPTY;
                cell.value.intValue = -1;
            }
            break;
        }
        case CHAR: {
            cell.value.charValue = input[0];
            break;
        }
        case STRING: {
            strcpy(cell.value.stringValue, input);
            break;
        }
        case EMPTY:
            break;
    }

    return cell;
}

int findRowIndex(Table* table, unsigned int rowId) {
    for (int i = 0; i < table->numRows; i++) {
        if (table->rows[i].id == rowId) {
            return i;
        }
    }
    return -1;
}

int rowExists(char* tableName, unsigned int rowId) {
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("Erro: A tabela '%s' não foi encontrada.\n", tableName);
        return 0;
    }

    Table* table = &tables[tableIndex];
    for (int i = 0; i < table->numRows; i++) {
        if (table->rows[i].id == rowId) {
            return 1;
        }
    }

    return 0;
}

void createRowLogic(Row newRow, char* tableName) {
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("Erro: A tabela '%s' não foi encontrada.\n", tableName);
        return;
    }

    Table* table = &tables[tableIndex];
    if (table->numRows >= MAX_ROWS) {
        printf("Erro: O número máximo de linhas foi atingido.\n");
        return;
    }

    table->rows[table->numRows] = newRow;
    table->numRows++;
    printf("A linha de ID %u foi criada com sucesso na tabela '%s'.\n", newRow.id, tableName);

}

void deleteRow(char* tableName, unsigned int rowId) {
    int tableIndex = findTableIndex(tableName);
    if (tableIndex == -1) {
        printf("Erro: A tabela '%s' não foi encontrada.\n", tableName);
        return;
    }

    Table* table = &tables[tableIndex];
    int rowIndex = findRowIndex(table, rowId);
    if (rowIndex == -1) {
        printf("Erro: A linha com ID %u não foi encontrada na tabela '%s'.\n", rowId, tableName);
        return;
    }


    for (int i = rowIndex; i < table->numRows - 1; i++) {
        table->rows[i] = table->rows[i + 1];
    }

    table->numRows--;
    printf("A linha com ID %u foi deletada com sucesso da tabela '%s'.\n", rowId, tableName);
}

void searchValue() {
    char tableName[MAX_NAME_LENGTH];
    char columnName[MAX_NAME_LENGTH];
    int found = 0;
    int foundRow = 0;
    int c;
    
    Column tempColumn;
    printf("Digite o nome da tabela: ");
    fgets(tableName, MAX_NAME_LENGTH, stdin);
    removeNewLine(tableName);
    int index = findTableIndex(tableName);
    

    if (index == -1) {
        printf("Tabela não encontrada.\n");
        searchValue();
        return;
    }
    Table tempTable = tables[index];

    printf("Escolha uma coluna para procurar o valor:\n");
    for (int i = 0; i < tempTable.numColumns; i++) {
        printf("%s\n", tempTable.columns[i].name);
    }
    fgets(columnName, MAX_NAME_LENGTH, stdin);
    removeNewLine(columnName);
    for (int i = 0; i < tempTable.numColumns; i++) {
        if (strcmp(tempTable.columns[i].name, columnName) == 0) {
            tempColumn = tempTable.columns[i];
            found = 1;
            break;
        }
    }

    if (found == 1) {
        printf("Digite o valor que está procurando: ");

        switch (tempColumn.type) {
            case INT: {
                int value;
                scanf("%d", &value);
                while ((c = getchar()) != '\n' && c != EOF);
                if (valueExistsInColumn(tempTable, tempColumn, tempTable.rows[0].cells, tempTable.numRows, &value, &foundRow)) {
                    printf("Valor encontrado na coluna %s. Linha: %d.\n", tempColumn.name, foundRow + 1);
                    break;
                } else {
                    valueNotFound(&value, tempTable, tempColumn);
                    break;
                }
            }
            case FLOAT: {
                float value;
                scanf("%f", &value);
                while ((c = getchar()) != '\n' && c != EOF);
                if (valueExistsInColumn(tempTable, tempColumn, tempTable.rows[0].cells, tempTable.numRows, &value, &foundRow)) {
                    printf("Valor encontrado na coluna %s. Linha: %d.\n", tempColumn.name, foundRow + 1);
                    break;
                } else {
                    valueNotFound(&value, tempTable, tempColumn);
                    break;
                }
            }
            case DOUBLE: {
                double value;
                scanf("%lf", &value);
                while ((c = getchar()) != '\n' && c != EOF);
                if (valueExistsInColumn(tempTable, tempColumn, tempTable.rows[0].cells, tempTable.numRows, &value, &foundRow)) {
                    printf("Valor encontrado na coluna %s. Linha: %d.\n", tempColumn.name, foundRow + 1);
                    break;
                } else {
                    valueNotFound(&value, tempTable, tempColumn);
                    break;
                }
            }
            case STRING: {
                char value[MAX_CELL_LENGTH];
                fgets(value, MAX_CELL_LENGTH, stdin);
                removeNewLine(value);
                if (valueExistsInColumn(tempTable, tempColumn, tempTable.rows[0].cells, tempTable.numRows, value, &foundRow)) {
                    printf("Valor encontrado na coluna %s. Linha: %d.\n", tempColumn.name, foundRow + 1);
                    break;
                } else {
                    valueNotFound(value, tempTable, tempColumn);
                    break;
                }
            }
            case CHAR: {
                char value;
                scanf(" %c", &value);
                while ((c = getchar()) != '\n' && c != EOF);
                if (valueExistsInColumn(tempTable, tempColumn, tempTable.rows[0].cells, tempTable.numRows, &value, &foundRow)) {
                    printf("Valor encontrado na coluna %s. Linha: %d.\n", tempColumn.name, foundRow + 1);
                    break;
                } else {
                    printf("Valor não encontrado. Valores da coluna %s:\n", tempColumn.name);
                    for(int i = 0; i < tempTable.numRows; i++){
                        printf("%c\n", tempTable.rows[i].cells[findColumnIndex(tables, tempColumn.name)].value.charValue);
                    }
                    break;

                }
                
            }
            default:
                break;
        }
    } else {
        printf("Coluna não encontrada.\n");
        searchValue();
    }
}
