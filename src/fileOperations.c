#include "fileOperations.h"
#include "utilsFileOperations.h"
#include "database.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Table tables[MAX_TABLES];
extern int numTables;

char* readFileName() {
    char* filename = (char*) malloc(MAX_NAME_LENGTH * sizeof(char));
    if (filename == NULL) {
        printf("Erro ao alocar memória para o nome do arquivo.\n");
        return NULL;
    }

    while (1) {
        printf("Digite o nome do arquivo (ou 'q' para voltar): ");
        readString(filename, MAX_NAME_LENGTH);

        if (strcmp(filename, "q") == 0 || strcmp(filename, "Q") == 0) {
            free(filename);
            return NULL;
        }

        if (isValidFileName(filename)) {
            break;
        } else {
            printf("Nome de arquivo inválido. Tente novamente.\n");
        }
    }

    return filename;
}

void processFileContent(char *fileContent) {
    char *currentPosition = fileContent;
    char *nextTablePosition;

    while (currentPosition != NULL && *currentPosition != '\0') {
        nextTablePosition = processTable(currentPosition);

        if (nextTablePosition == NULL)
            return;

        while (*nextTablePosition == '\n')
            nextTablePosition++;

        currentPosition = nextTablePosition;
    }
}

bool saveDatabaseToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return false;
    }

    for (int i = 0; i < numTables; i++) {
        Table *table = &tables[i];
        if (strlen(table->name) == 0) continue;

        fprintf(file, "Tabela %d: \"%s\", Quantidade de colunas: %d, Quantidade de linhas: %d\n", i + 1, table->name, table->numColumns, table->numRows);

        for (int j = 0; j < table->numColumns; j++) {
            fprintf(file, "Coluna %d: \"%s\", Tipo: %d\n", j + 1, table->columns[j].name, table->columns[j].type);
        }

        for (int j = 0; j < table->numRows; j++) {
            fprintf(file, "Linha %d: ", j + 1);
            for (int k = 0; k < table->numColumns; k++) {
                Cell *cell = &table->rows[j].cells[k];
                switch (cell->type) {
                    case INT:
                        fprintf(file, "\"%d\"", cell->value.intValue);
                        break;
                    case FLOAT:
                        fprintf(file, "\"%.2f\"", cell->value.floatValue);
                        break;
                    case DOUBLE:
                        fprintf(file, "\"%.2lf\"", cell->value.doubleValue);
                        break;
                    case CHAR:
                        fprintf(file, "\"%c\"", cell->value.charValue);
                        break;
                    case STRING:
                        fprintf(file, "\"%s\"", cell->value.stringValue);
                        break;
                    default:
                        fprintf(file, "\"\"");
                }
                if (k < table->numColumns - 1) fprintf(file, ", ");
            }
            fprintf(file, "\n");
        }
        fprintf(file, "\n");
    }

    fclose(file);

    return true;
}

bool loadFile(char* filename) {
    initDatabase();

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro: O arquivo %s não pôde ser aberto. Verifique se o arquivo existe e se você tem permissão para lê-lo.\n", filename);
        return false;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    if (fsize == 0) {
        printf("Erro: O arquivo %s está vazio.\n", filename);
        fclose(file);
        return false;
    }
    fseek(file, 0, SEEK_SET);

    char *fileContent = malloc(fsize + 1);
    if (fileContent == NULL) {
        printf("Erro: Não foi possível alocar memória para ler o arquivo %s.\n", filename);
        fclose(file);
        return false;
    }

    fread(fileContent, 1, fsize, file);
    fclose(file);

    fileContent[fsize] = '\0';

    processFileContent(fileContent);
    free(fileContent);
    printf("Arquivo %s carregado com sucesso.\n", filename);
    return true;
}
