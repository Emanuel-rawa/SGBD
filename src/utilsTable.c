#include "utilsTable.h"
#include "database.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

extern Table tables[MAX_TABLES];
extern int numTables;

int isValidName(char* name) {
    for (int i = 0; i < strlen(name); i++) {
        if (!isalnum((unsigned char)name[i]) && name[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int findColumnIndex(Table* table, char* columnName) {
    for (int i = 0; i < table->numColumns; i++) {
        if (strcmp(table->columns[i].name, columnName) == 0) {
            return i;
        }
    }
    return -1;
}

char* readNewTableName() {
    char* tableName = malloc(MAX_NAME_LENGTH * sizeof(char));

    while (1) {
        readString(tableName, MAX_NAME_LENGTH);

        if (strcmp(tableName, "q") == 0 || strcmp(tableName, "Q") == 0) {
            free(tableName);
            return NULL;
        }

        if (strlen(tableName) == 0 || strlen(tableName) > MAX_NAME_LENGTH) {
            printf("Erro: O nome da tabela deve ter no máximo %d caracteres. Digite novamente: ", MAX_NAME_LENGTH - 1);
            continue;
        }

        if (findTableIndex(tableName) != -1) {
            printf("Erro: A tabela '%s' já existe. Digite novamente: ", tableName);
            continue;
        }

        break;
    }

    return tableName;
}

char* readTableName() {
    char* tableName = malloc(MAX_NAME_LENGTH * sizeof(char));
    char* suggestedTableName;

    while (1) {
        readString(tableName, MAX_NAME_LENGTH);

        if (strcmp(tableName, "q") == 0 || strcmp(tableName, "Q") == 0) {
            free(tableName);
            return NULL;
        }

        if (strlen(tableName) == 0 || strlen(tableName) > MAX_NAME_LENGTH) {
            printf("Erro: O nome da tabela deve ter no máximo %d caracteres. Digite novamente: ", MAX_NAME_LENGTH - 1);
            continue;
        }

        int tableIndex = findTableIndex(tableName);
        if (tableIndex == -1) {
            char* existingTableNames[MAX_TABLES];
            for (int i = 0; i < numTables; i++) {
                existingTableNames[i] = tables[i].name;
            }
            suggestedTableName = suggestName(tableName, (const char **)existingTableNames, numTables);
            if (suggestedTableName != NULL) {
                printf("A tabela '%s' não existe. Você quis dizer '%s'? (s/n): ", tableName, suggestedTableName);
                char response = readChar();
                if (response == 's' || response == 'S') {
                    free(tableName);
                    tableName = suggestedTableName;
                } else {
                    printf("Deseja tentar novamente ou voltar ao menu? (t/m): ");
                    char retry = readChar();
                    if (retry == 'm' || retry == 'M') {
                        free(suggestedTableName);
                        free(tableName);
                        return NULL;
                    }
                    free(suggestedTableName);
                    continue;
                }
            }
        }

        break;
    }

    return tableName;
}

char* readColumnName(Table* table) {
    char* columnName = malloc(MAX_NAME_LENGTH * sizeof(char));

    while (1) {
        readString(columnName, MAX_NAME_LENGTH);

        if (strcmp(columnName, "q") == 0 || strcmp(columnName, "Q") == 0) {
            free(columnName);
            return NULL;
        }

        if (strlen(columnName) == 0 || strlen(columnName) > MAX_NAME_LENGTH) {
            printf("Erro: O nome da coluna deve ter no máximo %d caracteres. Digite novamente: ", MAX_NAME_LENGTH - 1);
            continue;
        }

        if (findColumnIndex(table, columnName) != -1) {
            printf("Erro: A coluna '%s' já existe na tabela. Digite novamente: ", columnName);
            continue;
        }

        break;
    }

    return columnName;
}

DataType readColumnType() {
    printf("Digite o tipo da coluna:\n");
    printf("1 - Inteiro\n");
    printf("2 - Float\n");
    printf("3 - Double\n");
    printf("4 - Char\n");
    printf("5 - String\n");
    printf("Digite o número correspondente ao tipo: ");
    int type = readInteger();

    while (type < 1 || type > 5) {
        printf("Tipo inválido. Digite novamente: ");
        type = readInteger();
    }

    switch (type) {
        case 1:
            return INT;
        case 2:
            return FLOAT;
        case 3:
            return DOUBLE;
        case 4:
            return CHAR;
        case 5:
            return STRING;
        default:
            return EMPTY;
    }
}

const char* printColumnType(DataType columnType) {
    switch (columnType) {
        case INT:
            return "Inteiro";
        case FLOAT:
            return "Float";
        case DOUBLE:
            return "Double";
        case CHAR:
            return "Char";
        case STRING:
            return "String";
        default:
            return "Desconhecido";
    }
}

int findTableIndex(char* tableName) {
    int i;
    for (i = 0; i < numTables; i++) {
        if (strcmp(tables[i].name, tableName) == 0) {
            return i;
        }
    }

    return -1;
}

int valueExistsInColumn(Table table, Column column, Cell cells[], int numRows, void* value, int* foundRow) {
    for (int i = 0; i < numRows; i++) {
        for(int j = 0; j < MAX_COLUMNS;j++)
        switch (column.type) {
            case INT:
                if (table.rows[i].cells[j].value.intValue == *((int*)value)) {
                    return 1; 
                    *foundRow = i;
                }
                break;
            case FLOAT:
                if (table.rows[i].cells[j].value.floatValue == *((float*)value)) {
                    return 1;
                    *foundRow = i;
                }
                break;
            case DOUBLE:
                if (table.rows[i].cells[j].value.doubleValue == *((double*)value)) {
                    return 1; 
                    *foundRow = i;
                }
                break;
            case CHAR:
                if (table.rows[i].cells[j].value.charValue == *((char*)value)) {
                    return 1;
                    *foundRow = i;
                }
                break;
            case STRING:
                if (strcmp(table.rows[i].cells[j].value.stringValue, (char*)value) == 0) {
                    return 1; 
                    *foundRow = i;
                }
                break;
            default:
                break;
        }
    }
    return 0; 
}

void valueNotFound(void *value, Table table, Column column) {
    int counterGreaterThan = 0;
    int counterLessThan = 0;
    int columnIndex = findColumnIndex(tables, column.name);
    char **nameArray[table.numRows];
    int c;

    if (column.type == INT || column.type == FLOAT || column.type == DOUBLE) {
        for (int i = 0; i < table.numRows; i++) {
            switch (column.type) {
                case INT:
                    if (table.rows[i].cells[columnIndex].value.intValue > *((int*)value)) {
                        counterGreaterThan++;
                    } else if (table.rows[i].cells[columnIndex].value.intValue < *((int*)value)) {
                        counterLessThan++;
                    }
                    break;
                case FLOAT:
                    if (table.rows[i].cells[columnIndex].value.floatValue > *((float*)value)) {
                        counterGreaterThan++;
                    } else if (table.rows[i].cells[columnIndex].value.floatValue < *((float*)value)) {
                        counterLessThan++;
                    }
                    break;
                case DOUBLE:
                    if (table.rows[i].cells[columnIndex].value.doubleValue > *((double*)value)) {
                        counterGreaterThan++;
                    } else if (table.rows[i].cells[columnIndex].value.doubleValue < *((double*)value)) {
                        counterLessThan++;
                    }
                    break;
            }
        }

        printf("Valor não encontrado. Existem %d valores maiores e %d valores menores que o solicitado. Gostaria de saber quais?\n", counterGreaterThan, counterLessThan);
        printf("Para valores maiores, digite G.\nPara valores menores, digite L.\n");

        char entrada;
        scanf(" %c", &entrada);
        while ((c = getchar()) != '\n' && c != EOF);
        entrada = toupper(entrada);

        if (entrada == 'G') {
            for (int i = 0; i < table.numRows; i++) {
                switch (column.type) {
                    case INT:
                        if (table.rows[i].cells[columnIndex].value.intValue > *((int*)value)) {
                            printf("%d ", table.rows[i].cells[columnIndex].value.intValue);
                        }
                        break;
                    case FLOAT:
                        if (table.rows[i].cells[columnIndex].value.floatValue > *((float*)value)) {
                            printf("%.2f ", table.rows[i].cells[columnIndex].value.floatValue);
                        }
                        break;
                    case DOUBLE:
                        if (table.rows[i].cells[columnIndex].value.doubleValue > *((double*)value)) {
                            printf("%.2lf ", table.rows[i].cells[columnIndex].value.doubleValue);
                        }
                        break;
                }
            }
        } else if (entrada == 'L') {
            for (int i = 0; i < table.numRows; i++) {
                switch (column.type) {
                    case INT:
                        if (table.rows[i].cells[columnIndex].value.intValue < *((int*)value)) {
                            printf("%d ", table.rows[i].cells[columnIndex].value.intValue);
                        }
                        break;
                    case FLOAT:
                        if (table.rows[i].cells[columnIndex].value.floatValue < *((float*)value)) {
                            printf("%.2f ", table.rows[i].cells[columnIndex].value.floatValue);
                        }
                        break;
                    case DOUBLE:
                        if (table.rows[i].cells[columnIndex].value.doubleValue < *((double*)value)) {
                            printf("%.2lf ", table.rows[i].cells[columnIndex].value.doubleValue);
                        }
                        break;
                }   
            }
        }
    }
    else if(column.type == STRING){
        char *nameArray[table.numRows];
        for(int i = 0; i < table.numRows; i++){
            nameArray[i] = strdup(table.rows[i].cells[columnIndex].value.stringValue);
        }

        char *suggestion = suggestName(value, (const char **)nameArray, table.numRows);
        printf("Valor não encontrado.\nVocê quis dizer: %s? [S|N]\n", suggestion);


        char entrada;
        scanf("%c", &entrada);
        while ((c = getchar()) != '\n' && c != EOF);
        entrada = toupper(entrada);
        if(entrada == 'S'){
            printf("Valor encontrado na coluna %s.\n", column.name);
        }
        else if(entrada == 'N'){
            printf("Valor não encontrado.\n");
        }


        for(int i = 0; i < table.numRows; i++){
            free(nameArray[i]);
        }
}

}
